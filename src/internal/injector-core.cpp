/*
 * %injeqt copyright begin%
 * Copyright 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * %injeqt copyright end%
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "injector-core.h"

#include <injeqt/exception/ambiguous-types.h>
#include <injeqt/exception/unavailable-required-types.h>
#include <injeqt/exception/unknown-type.h>
#include <injeqt/module.h>

#include "action-method.h"
#include "containers.h"
#include "instantiate-type.h"
#include "interfaces-utils.h"
#include "provider-by-default-constructor.h"
#include "provider-ready.h"
#include "provider.h"
#include "module-impl.h"
#include "required-to-satisfy.h"
#include "resolve-dependencies.h"
#include "resolved-dependency.h"

#include <cassert>

namespace injeqt { namespace internal {

injector_core::injector_core()
{
}

injector_core::injector_core(types_by_name known_types, std::vector<std::unique_ptr<provider>> &&all_providers) :
	_known_types{std::move(known_types)}
{
	auto all_providers_size = all_providers.size();
	_available_providers = providers{std::move(all_providers)};

	// some types were removed, because of duplication
	if (_available_providers.size() != all_providers_size)
		throw exception::ambiguous_types{}; // TODO: find a way to extract type names

	_types_model = create_types_model();

	auto required_types = std::vector<type>{};
	for (auto &&p : _available_providers)
		for (auto &&r : p->required_types())
			required_types.push_back(r);

	auto unavailable_required_types = match(types{required_types}, _types_model.available_types()).unmatched_1;
	if (!unavailable_required_types.empty())
	{
		auto message = std::string{};
		for (auto &&t : unavailable_required_types)
		{
			message.append(t.name());
			message.append("\n");
		}
		throw exception::unavailable_required_types{message};
	}

	instantiate_all_immediate();
}

injector_core::~injector_core()
{
	for (auto &&resolved_object : _resolved_objects)
		call_done_methods(resolved_object.object());
}

types_model injector_core::create_types_model() const
{
	auto all_types = std::vector<type>{};
	auto need_dependencies = std::vector<type>{};
	for (auto &&p : _available_providers)
	{
		all_types.push_back(p->provided_type());
		if (p->require_resolving())
		{
			auto interfaces = extract_interfaces(p->provided_type());
			std::copy(std::begin(interfaces), std::end(interfaces), std::back_inserter(need_dependencies));
		}
	}
	return make_types_model(_known_types, all_types, need_dependencies);
}

std::vector<type> injector_core::provided_types() const
{
	auto result = std::vector<type>{};
	std::transform(std::begin(_available_providers), std::end(_available_providers), std::back_inserter(result), type_from_provider);
	return result;
}

QObject * injector_core::get(const type &interface_type)
{
	assert(!interface_type.is_empty());
	assert(!interface_type.is_qobject());

	auto object_it = _objects.get(interface_type);
	if (object_it != end(_objects))
		return object_it->object();

	instantiate(interface_type);
	return _objects.get(interface_type)->object();
}

void injector_core::instantiate(const type &interface_type)
{
	auto result = objects_with_interface_type(_objects, interface_type);
	_objects = result.first;

	for (auto &&resolved_object : result.second)
		call_init_methods(resolved_object.object());
}

std::pair<implementations, implementations> injector_core::objects_with_interface_type(implementations objects, const type &interface_type)
{
	auto implementation_type_it = _types_model.available_types().get(interface_type);
	if (implementation_type_it == end(_types_model.available_types()))
		throw exception::unknown_type{interface_type.name()};

	return objects_with_implementation_type(objects, implementation_type_it->implementation_type());
}

std::pair<implementations, implementations> injector_core::objects_with_implementation_type(implementations objects, const type &implementation_type)
{
	auto implementation_type_dependencies = _types_model.mapped_dependencies().contains_key(implementation_type)
			? _types_model.mapped_dependencies().get(implementation_type)->dependency_list()
			: dependencies{};

	auto types_to_instantiate = required_to_satisfy(implementation_type_dependencies, _types_model, objects);
	types_to_instantiate.add(implementation_type);
	return objects_with(objects, types_to_instantiate);
}

std::pair<implementations, implementations> injector_core::objects_with(implementations objects, const types &types_to_instantiate)
{
	instantiate_required_types_for(types_to_instantiate);

	auto objects_to_resolve = std::vector<implementation>{};
	auto objects_to_store = std::vector<implementation>{};

	for (auto &&type_to_instantiate : types_to_instantiate)
	{
		if (objects.get(type_to_instantiate) != end(objects))
			continue;

		auto provider_it = _available_providers.get(type_to_instantiate);
		auto instance = provider_it->get()->provide(*this);

		auto i = make_implementation(type_to_instantiate, instance);
		if (provider_it->get()->require_resolving())
			objects_to_resolve.emplace_back(i);

		auto interfaces = extract_interfaces(type_to_instantiate);
		auto matched = match(interfaces, _types_model.available_types()).matched;
		for (auto &&m : matched)
		{
			auto i = implementation{m.first, instance}; // no need to check preconditions again with make_implementation
			objects_to_store.emplace_back(i);
		}
	}

	objects.merge(implementations{objects_to_store});
	_resolved_objects.merge(implementations{objects_to_resolve});
	resolve_objects(objects_to_resolve, objects);

	return std::make_pair(objects, implementations{objects_to_resolve});
}

void injector_core::instantiate_required_types_for(const types &types_to_instantiate)
{
	for (auto &&provider : providers_for(types_to_instantiate))
		for (auto &&required_type : provider->required_types())
			instantiate(required_type);
}

std::vector<provider *> injector_core::providers_for(const types &for_types) const
{
	auto result = std::vector<provider *>{};
	result.reserve(for_types.size());
	for (auto &&for_type : for_types)
	{
		auto provider_it = _available_providers.get(for_type);
		assert(provider_it != end(_available_providers));

		result.push_back(provider_it->get());
	}

	return result;
}

void injector_core::resolve_objects(const std::vector<implementation> &objects, const implementations &with)
{
	for (auto &&object : objects)
		resolve_object(object, with);
}

void injector_core::resolve_object(const implementation &object, const implementations &with)
{
	auto to_resolve = _types_model.mapped_dependencies().get(object.interface_type())->dependency_list();
	auto resolved_dependencies = resolve_dependencies(to_resolve, with);
	assert(resolved_dependencies.unresolved.empty());

	for (auto &&resolved : resolved_dependencies.resolved)
	{
		assert(implements(object.interface_type(), resolved.setter().object_type()));
		resolved.apply_on(object.object());
	}
}

void injector_core::inject_into(QObject *object)
{
	auto object_type = type{object->metaObject()};
	auto dependencies = extract_dependencies(_known_types, object_type);
	auto types_to_instantiate = required_to_satisfy(dependencies, _types_model, _objects);
	auto result = objects_with(_objects, types_to_instantiate);
	_objects = result.first;

	for (auto &&resolved_object : result.second)
		call_init_methods(resolved_object.object());

	auto resolved_dependencies = resolve_dependencies(dependencies, _objects);
	for (auto &&resolved : resolved_dependencies.resolved)
	{
		assert(implements(object_type, resolved.setter().object_type()));
		resolved.apply_on(object);
	}

	call_init_methods(object);
}

void injector_core::instantiate_all_immediate()
{
	for (auto &&provider : _available_providers)
	{
		auto type = provider->provided_type();
		if (get_instantiate_type(type) == instantiate_type::immediate)
			get(type);
	}
}

void injector_core::call_init_methods(QObject *object)
{
	for (auto action : extract_actions("INJEQT_INIT", type{object->metaObject()}))
		action.invoke(object);
}

void injector_core::call_done_methods(QObject *object)
{
	auto done_actions = extract_actions("INJEQT_DONE", type{object->metaObject()});
	for (auto i = done_actions.rbegin(), e = done_actions.rend(); i != e; ++i)
		i->invoke(object);
}

}}

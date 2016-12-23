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
#include "interfaces-utils.h"
#include "provided-object.h"
#include "provider-by-default-constructor.h"
#include "provider-ready.h"
#include "provider.h"
#include "module-impl.h"
#include "required-to-satisfy.h"
#include "resolve-dependencies.h"
#include "resolved-dependency.h"
#include "type-role.h"

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

void injector_core::instantiate(const type &interface_type)
{
	assert(!interface_type.is_empty());
	assert(!interface_type.is_qobject());

	auto object_it = _objects.get(interface_type);
	if (object_it == end(_objects))
		instantiate_interface(interface_type);
}

void injector_core::instantiate_all_with_type_role(const std::string &type_role)
{
	for (auto &&provider : _available_providers)
	{
		auto type = provider->provided_type();
		if (has_type_role(type, type_role))
			instantiate_interface(type);
	}
}

QObject * injector_core::get(const type &interface_type)
{
	assert(!interface_type.is_empty());
	assert(!interface_type.is_qobject());

	instantiate(interface_type);
	return _objects.get(interface_type)->object();
}

std::vector<QObject *> injector_core::get_all_with_type_role(const std::string &type_role)
{
	auto result = std::vector<QObject *>{};
	for (auto &&provider : _available_providers)
	{
		auto type = provider->provided_type();
		if (has_type_role(type, type_role))
			result.push_back(get(type));
	}

	return result;
}

void injector_core::instantiate_interface(const type &interface_type)
{
	assert(!interface_type.is_empty());
	assert(!interface_type.is_qobject());

	instantiate_implementation(implementation_for(interface_type));
}

type injector_core::implementation_for(const type &interface_type) const
{
	assert(!interface_type.is_empty());
	assert(!interface_type.is_qobject());

	auto implementation_type_it = _types_model.available_types().get(interface_type);
	if (implementation_type_it == end(_types_model.available_types()))
		throw exception::unknown_type{interface_type.name()};
	return implementation_type_it->implementation_type();
}

void injector_core::instantiate_implementation(const type &implementation_type)
{
	assert(!implementation_type.is_empty());
	assert(!implementation_type.is_qobject());

	auto types_to_instantiate = required_to_satisfy(implementation_type_dependencies(implementation_type), _types_model, _objects);
	types_to_instantiate.add(implementation_type);
	instantiate_all(types_to_instantiate);
}

dependencies injector_core::implementation_type_dependencies(const type &implementation_type) const
{
	assert(!implementation_type.is_empty());
	assert(!implementation_type.is_qobject());

	return _types_model.mapped_dependencies().contains_key(implementation_type)
			? _types_model.mapped_dependencies().get(implementation_type)->dependency_list()
			: dependencies{};
}

void injector_core::instantiate_all(const types &interface_types)
{
	instantiate_required_types_for(interface_types);

	auto provided_objects = provide_objects(providers_for(non_instantiated(interface_types)));
	_objects.merge(implementations{objects_to_store(extract_implementations(provided_objects))});
	resolve_objects(objects_to_resolve(provided_objects));
}

void injector_core::instantiate_required_types_for(const types &types_to_instantiate)
{
	for (auto &&provider : providers_for(types_to_instantiate))
		for (auto &&required_type : provider->required_types())
			instantiate_interface(required_type);
}

std::vector<type> injector_core::non_instantiated(const types &to_filter) const
{
	auto result = std::vector<type>{};
	result.reserve(to_filter.size());
	for (auto &&type : to_filter)
		if (_objects.get(type) == end(_objects))
			result.push_back(type);
	return result;
}

std::vector<provided_object> injector_core::provide_objects(const std::vector<provider *> &providers)
{
	auto result = std::vector<provided_object>{};
	result.reserve(providers.size());
	for (auto &&provider : providers)
	{
		auto instance = provider->provide(*this);
		auto i = make_implementation(provider->provided_type(), instance);
		result.push_back(provided_object{provider, i});
	}
	return result;
}

std::vector<implementation> injector_core::objects_to_resolve(const std::vector<provided_object> &provided_objects) const
{
	auto result = std::vector<implementation>{};
	result.reserve(provided_objects.size());
	for (auto &&provided_object : provided_objects)
		if (provided_object.provided_by()->require_resolving())
			result.push_back(provided_object.object());
	return result;
}

std::vector<implementation> injector_core::extract_implementations(const std::vector<provided_object> &provided_objects) const
{
	auto result = std::vector<implementation>{};
	result.reserve(provided_objects.size());
	for (auto &&provided_object : provided_objects)
		result.push_back(provided_object.object());
	return result;
}

std::vector<implementation> injector_core::objects_to_store(const std::vector<implementation> &objects) const
{
	auto result = std::vector<implementation>{};
	for (auto &&object : objects)
	{
		auto interfaces = extract_interfaces(object.interface_type());
		auto matched = match(interfaces, _types_model.available_types()).matched;
		for (auto &&m : matched)
		{
			auto i = implementation{m.first, object.object()}; // no need to check preconditions again with make_implementation
			result.emplace_back(i);
		}
	}
	return result;
}

void injector_core::resolve_objects(const std::vector<implementation> &objects)
{
	for (auto &&object : objects)
		resolve_object(object);
	for (auto &&object : objects)
		call_init_methods(object.object());
	_resolved_objects.merge(implementations{objects});
}

void injector_core::resolve_object(const implementation &object) const
{
	auto object_dependencies = implementation_type_dependencies(object.interface_type());
	resolve_object(object_dependencies, object);
}

void injector_core::resolve_object(const dependencies &object_dependencies, const implementation &object) const
{
	auto resolved_dependencies = resolve_dependencies(object_dependencies, _objects);
	assert(resolved_dependencies.unresolved.empty());

	for (auto &&resolved : resolved_dependencies.resolved)
	{
		assert(implements(object.interface_type(), resolved.setter().object_type()));
		resolved.apply_on(object.object());
	}
}

void injector_core::inject_into(QObject *object)
{
	auto object_implementation = implementation{type{object->metaObject()}, object};
	auto dependencies = extract_dependencies(_known_types, object_implementation.interface_type());
	auto types_to_instantiate = required_to_satisfy(dependencies, _types_model, _objects);
	instantiate_all(types_to_instantiate);
	resolve_object(dependencies, object_implementation);
	call_init_methods(object);
}

void injector_core::call_init_methods(QObject *object) const
{
	for (auto action : extract_actions("INJEQT_INIT", type{object->metaObject()}))
		action.invoke(object);
}

void injector_core::call_done_methods(QObject *object) const
{
	auto done_actions = extract_actions("INJEQT_DONE", type{object->metaObject()});
	for (auto i = done_actions.rbegin(), e = done_actions.rend(); i != e; ++i)
		i->invoke(object);
}

}}

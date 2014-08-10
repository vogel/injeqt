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

#include "scope.h"

#include "provider-by-default-constructor.h"
#include "provider.h"
#include "default-constructor-method.h"
#include "dependencies.h"
#include "extract-interfaces.h"
#include "required-to-instantiate.h"
#include "resolved-dependency.h"
#include "resolve-dependencies.h"
#include "type-relations.h"

#include <memory>
#include <QtCore/QObject>

namespace injeqt { namespace internal {

scope::scope(providers available_providers, type_relations available_types) :
	_available_providers{std::move(available_providers)},
	_state{std::move(available_types), {}}
{
}

const providers & scope::available_providers() const
{
	return _available_providers;
}

const instantiation_state & scope::state() const
{
	return _state;
}

QObject * scope::get(const type &interface_type)
{
	auto implementation_type_it = _state.available_types().unique().get(interface_type);
	if (implementation_type_it == end(_state.available_types().unique()))
		throw type_not_in_scope_exception{interface_type.name()};

	auto implementation_type = implementation_type_it->implementation_type();
	auto object_it = _state.objects().get(implementation_type);
	if (object_it != end(_state.objects()))
		return object_it->object();

	_state = state_with(_state, implementation_type);

	// TODO: if we dont have the object, throw exception
	auto object_it_2 = _state.objects().get(implementation_type);
	if (object_it_2 != end(_state.objects()))
		return object_it_2->object();
	else
		return nullptr;
}

instantiation_state scope::state_with(instantiation_state state, const type &implementation_type)
{
	auto types_to_instantiate = required_to_instantiate(implementation_type, state);
	return state_with(state, types_to_instantiate);
}

instantiation_state scope::state_with(instantiation_state state, const types &types_to_instantiate)
{
	auto objects_to_resolve = std::vector<implementation>{};
	for (auto &&type_to_instantiate : types_to_instantiate)
	{
		auto provider_it = _available_providers.get(type_to_instantiate);
		if (provider_it == end(_available_providers))
			return state; // TODO: throw exception

		for (auto &&required_type : provider_it->get()->required_types())
			state = state_with(state, required_type);
	}

	for (auto &&type_to_instantiate : types_to_instantiate)
		if (state.objects().get(type_to_instantiate) == end(state.objects()))
		{
			auto provider_it = _available_providers.get(type_to_instantiate);
			auto instance = provider_it->get()->create(*this);

			if (instance)
				objects_to_resolve.emplace_back(type_to_instantiate, instance);
			else {} // THROW
		}

	auto objects = state.objects();
	objects.merge(implementations{objects_to_resolve});

	for (auto &&object_to_resolve : objects_to_resolve)
	{
		auto to_resolve = make_dependencies(object_to_resolve.interface_type());
		auto resolved_dependencies = resolve_dependencies(to_resolve, objects);
		if (!resolved_dependencies.unresolved.empty())
			throw unresolved_dependencies_exception{};

		for (auto &&resolved : resolved_dependencies.resolved)
			resolved.apply_on(object_to_resolve.object());
	}

	return instantiation_state{state.available_types(), objects};
}

bool operator == (const scope &x, const scope &y)
{
	if (x.state() != y.state())
		return false;

	return true;
}

bool operator != (const scope &x, const scope &y)
{
	return !(x == y);
}

scope make_scope(providers available_providers, const std::vector<type> &scope_types)
{
	return scope{std::move(available_providers), make_type_relations(scope_types)};
}

}}

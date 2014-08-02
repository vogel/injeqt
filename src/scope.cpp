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

#include "extract-dependencies.h"
#include "extract-interfaces.h"
#include "required-to-instantiate.h"
#include "resolved-dependency.h"
#include "resolve-dependencies.h"

namespace injeqt { namespace v1 {

scope::scope(implemented_by_mapping available_types, implementations objects) :
	_available_types{std::move(available_types)},
	_objects{std::move(objects)}
{
}

implemented_by_mapping scope::available_types() const
{
	return _available_types;
}

implementations scope::objects() const
{
	return _objects;
}

QObject * scope::get(const type &t)
{
	auto implementation_type_it = _available_types.get(t);
	if (implementation_type_it == end(_available_types))
		throw type_not_in_scope_exception{};

	auto implementation_type = implementation_type_it->implementation_type();
	auto object_it = _objects.get(implementation_type);
	if (object_it != end(_objects))
		return object_it->object();

	auto types_to_instantiate = required_to_instantiate(implementation_type, _available_types, _objects);

	auto new_objects = std::vector<implementation>{};
	auto objects_to_resolve = std::vector<implementation>{};
	for (auto &&type_to_instantiate : types_to_instantiate)
	{
		auto unique_ptr_instance = std::unique_ptr<QObject>{type_to_instantiate.meta_object()->newInstance()};
		auto instance = unique_ptr_instance.get();

		_owned_objects.push_back(std::move(unique_ptr_instance));

		auto interfaces = extract_interfaces(type_to_instantiate);
		objects_to_resolve.emplace_back(type_to_instantiate, instance);
		for (auto &&interface : interfaces)
			new_objects.emplace_back(interface, instance);
	}

	auto merged_objects = _objects;
	merged_objects.merge(implementations{new_objects});

	for (auto &&object_to_resolve : objects_to_resolve)
	{
		auto to_resolve = extract_dependencies(object_to_resolve.interface_type());
		auto resolved_dependencies = resolve_dependencies(to_resolve, merged_objects);
		if (!resolved_dependencies.unresolved.empty())
			throw unresolved_dependencies_exception{};

		for (auto &&resolved : resolved_dependencies.resolved)
			resolved.apply_on(object_to_resolve.object());
	}

	_objects = merged_objects;
	return _objects.get(implementation_type)->object();
}

bool operator == (const scope &x, const scope &y)
{
	if (std::addressof(x) == std::addressof(y))
		return true;

	if (x.available_types() != y.available_types())
		return false;

	if (x.objects() != y.objects())
		return false;

	return true;
}

bool operator != (const scope &x, const scope &y)
{
	return !(x == y);
}

}}

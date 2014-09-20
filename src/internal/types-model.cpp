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

#include "types-model.h"

#include "type-relations.h"

namespace injeqt { namespace internal {

types_model::types_model()
{
}

types_model::types_model(implemented_by_mapping available_types, types_dependencies mapped_dependencies) :
	_available_types{std::move(available_types)},
	_mapped_dependencies{std::move(mapped_dependencies)}
{
}

const implemented_by_mapping & types_model::available_types() const
{
	return _available_types;
}

const types_dependencies & types_model::mapped_dependencies() const
{
	return _mapped_dependencies;
}

type_dependencies types_model::get_dependencies(const type &interface_type) const
{
	auto available_it = _available_types.get(interface_type);
	if (available_it == end(_available_types))
		throw type_not_in_types_model_exception{interface_type.name()};

	return *_mapped_dependencies.get(available_it->implementation_type());
}

bool operator == (const types_model &x, const types_model &y)
{
	if (x.available_types() != y.available_types())
		return false;

	if (x.mapped_dependencies() != y.mapped_dependencies())
		return false;

	return true;
}

bool operator != (const types_model &x, const types_model &y)
{
	return !(x == y);
}

types_model make_types_model(const std::vector<type> &all_types)
{
	auto relations = make_type_relations(all_types);

	for (auto &&t : all_types)
		if (relations.ambiguous().contains(t))
			throw ambiguous_type_exception{t.name()};

	auto all_dependencies = std::vector<type_dependencies>{};
	std::transform(std::begin(relations.unique()), std::end(relations.unique()), std::back_inserter(all_dependencies),
		[](const implemented_by &ib){ return make_type_dependencies(ib.implementation_type()); });

	auto available_types = relations.unique();
	auto mapped_dependencies = types_dependencies{all_dependencies};

	for (auto &&mapped_type_dependency : mapped_dependencies)
		for (auto &&dependency : mapped_type_dependency.dependency_list())
			if (!available_types.contains_key(dependency.required_type()))
				throw unresolvable_dependency_exception{dependency.required_type().name()};

	return types_model(available_types, mapped_dependencies);
}

}}

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

#include <injeqt/exception/ambiguous-types.h>
#include <injeqt/exception/unresolvable-dependencies.h>

#include "type-relations.h"

#include <cassert>

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

bool types_model::contains(const type &interface_type) const
{
	return _available_types.get(interface_type) != end(_available_types);
}

std::vector<dependency> types_model::get_unresolvable_dependencies() const
{
	auto result = std::vector<dependency>{};
	for (auto &&mapped_type_dependency : _mapped_dependencies)
		for (auto &&dependency : mapped_type_dependency.dependency_list())
			if (!contains(dependency.required_type()))
				result.push_back(dependency);
	return result;
}

types_model make_types_model(const types_by_name &known_types, const std::vector<type> &all_types, const std::vector<type> &need_dependencies)
{
	auto relations = make_type_relations(all_types);

	for (auto &&t : all_types)
	{
		auto message = std::string{};
		if (relations.ambiguous().contains(t))
		{
			message.append(t.name());
			message.append("\n");
		}

		if (!message.empty())
			throw exception::ambiguous_types{message};
	}

	auto all_dependencies = std::vector<type_dependencies>{};
	std::transform(std::begin(need_dependencies), std::end(need_dependencies), std::back_inserter(all_dependencies),
		[&](const type &t){ return make_type_dependencies(known_types, t); });

	auto available_types = relations.unique();
	auto mapped_dependencies = types_dependencies{all_dependencies};
	auto result = types_model(available_types, mapped_dependencies);
	auto unresolvable_dependencies = result.get_unresolvable_dependencies();

	if (!unresolvable_dependencies.empty())
	{
		auto message = std::string{};
		for (auto &&unresolvable_dependency : unresolvable_dependencies)
		{
			message.append(unresolvable_dependency.required_type().name());
			message.append(": ");
			message.append(unresolvable_dependency.setter().signature());
			message.append("\n");
		}
		throw exception::unresolvable_dependencies{message};
	}

	return result;
}

}}

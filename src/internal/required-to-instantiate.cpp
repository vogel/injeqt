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

#include "required-to-instantiate.h"

#include "dependencies.h"
#include "extract-interfaces.h"
#include "implemented-by.h"

#include <set>

namespace injeqt { namespace internal {

types required_to_instantiate(const type &type_to_instantiate, const model &types_model, const implementations &objects)
{
	auto result = std::vector<type>{};
	auto future_available = std::set<type>{};

	auto interfaces_to_check = std::vector<type>{type_to_instantiate};
	while (!interfaces_to_check.empty())
	{
		auto current_interface_type = interfaces_to_check.back();
		interfaces_to_check.pop_back();

		auto available_it = types_model.available_types().get(current_interface_type);
		if (available_it == end(types_model.available_types()))
			throw type_not_mapped_exception{};

		auto current_implementation_type = available_it->implementation_type();
		if (objects.contains_key(current_implementation_type))
			continue;

		if (future_available.find(current_implementation_type) != std::end(future_available))
			continue;
		future_available.insert(current_implementation_type);

		result.push_back(current_implementation_type);

		auto dependencies = types_model.mapped_dependencies().get(current_implementation_type)->dependency_list();
		for (auto &&dependency : dependencies)
			interfaces_to_check.push_back(dependency.required_type());
	}

	return types{result};
}

}}

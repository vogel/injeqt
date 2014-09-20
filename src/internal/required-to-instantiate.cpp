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

types required_to_instantiate(const type &type_to_instantiate, const types_model &model, const implementations &objects)
{
	auto result = std::vector<type>{};
	auto ready = std::set<type>{};
	std::transform(std::begin(objects), std::end(objects), std::inserter(ready, std::end(ready)),
		[](const implementation &i){ return i.interface_type(); });

	auto interfaces_to_check = std::vector<type>{type_to_instantiate};
	while (!interfaces_to_check.empty())
	{
		auto current_interface_type = interfaces_to_check.back();
		interfaces_to_check.pop_back();

		auto dependencies = model.get_dependencies(current_interface_type);
		auto current_implementation_type = dependencies.dependent_type();

		if (ready.find(current_implementation_type) != std::end(ready))
			continue;
		ready.insert(current_implementation_type);

		result.push_back(current_implementation_type);

		for (auto &&dependency : dependencies.dependency_list())
			interfaces_to_check.push_back(dependency.required_type());
	}

	return types{result};
}

}}

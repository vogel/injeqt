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

#include "required-to-satisfy.h"

#include "dependencies.h"
#include "implemented-by.h"
#include "interfaces-utils.h"

#include <cassert>
#include <set>

namespace injeqt { namespace internal {

types required_to_satisfy(const dependencies &dependencies_to_satisfy, const types_model &model, const implementations &objects)
{
	assert(model.get_unresolvable_dependencies().empty());

	auto result = std::vector<type>{};
	auto ready = std::set<type>{};
	std::transform(std::begin(objects), std::end(objects), std::inserter(ready, std::end(ready)),
		[](const implementation &i){ return i.interface_type(); });

	auto interfaces_to_check = std::vector<type>{};
	std::transform(std::begin(dependencies_to_satisfy), std::end(dependencies_to_satisfy), std::back_inserter(interfaces_to_check),
		[](const dependency &d){ return d.required_type(); });

	while (!interfaces_to_check.empty())
	{
		auto current_interface_type = interfaces_to_check.back();
		interfaces_to_check.pop_back();

		auto current_implementation_type_it = model.available_types().get(current_interface_type);
		if (current_implementation_type_it == std::end(model.available_types()))
			continue;

		auto current_implementation_type = current_implementation_type_it->implementation_type();
		if (ready.find(current_implementation_type) != std::end(ready))
			continue;
		ready.insert(current_implementation_type);
		result.push_back(current_implementation_type);

		if (model.mapped_dependencies().contains_key(current_implementation_type))
		{
			auto dependencies = model.mapped_dependencies().get(current_implementation_type)->dependency_list();
			std::transform(std::begin(dependencies), std::end(dependencies), std::back_inserter(interfaces_to_check),
				[](const dependency &d){ return d.required_type(); });
		}
	}

	return types{result};
}

}}

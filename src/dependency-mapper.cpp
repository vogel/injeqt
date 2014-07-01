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

#include "dependency-mapper.h"

#include "injeqt-object.h"

namespace injeqt { namespace v1 {

std::tuple<std::unordered_map<dependency, const injeqt_object *>, std::vector<dependency>> dependency_mapper::map_dependencies(
	const std::vector<dependency> &dependencies,
	const std::vector<const injeqt_object *> &objects) const
{
	auto result_mapped = std::unordered_map<dependency, const injeqt_object *>{};
	auto result_unmapped = std::vector<dependency>{};
	for (auto &&dependency : dependencies)
	{
		auto mapped = get_dependency(dependency, objects);
		if (mapped)
			result_mapped.emplace(dependency, mapped);
		else
			result_unmapped.push_back(dependency);
	}
	return std::make_tuple(std::move(result_mapped), std::move(result_unmapped));
}

const injeqt_object * dependency_mapper::get_dependency(
	const dependency & dependency,
	const std::vector<const injeqt_object *> &objects) const
{
	auto it = std::find_if(std::begin(objects), std::end(objects),
		[&dependency](const injeqt_object *object){ return object->meta().implements(dependency.object()); }
	);
	return it == std::end(objects)
		? nullptr
		: *it;
}


}}

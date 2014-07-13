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

#include "dependency-resolver.h"

#include "object-with-meta.h"
#include "resolved-dependency.h"

namespace injeqt { namespace v1 {

resolve_dependencies_result dependency_resolver::resolve_dependencies(
	const dependencies &to_resolve,
	const objects_with_meta &objects) const
{
	auto unresolved = std::vector<dependency>{};
	auto resolved = std::vector<resolved_dependency>{};
	for (auto &&dependency : to_resolve)
	{
		auto mapped = resolve_dependency(dependency, objects);
		if (mapped)
			resolved.emplace_back(dependency, *mapped);
		else
			unresolved.emplace_back(dependency);
	}
	return {unresolved, resolved};
}

const object_with_meta * dependency_resolver::resolve_dependency(
	const dependency & dependency,
	const objects_with_meta &objects) const
{
	auto it = std::find_if(begin(objects), end(objects),
		[&dependency](const object_with_meta *object){ return object->meta().implements(dependency.type()); }
	);
	return it == end(objects)
		? nullptr
		: *it;
}

}}

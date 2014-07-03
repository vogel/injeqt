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

#include "injeqt-object.h"
#include "resolved-dependency.h"

namespace injeqt { namespace v1 {

resolve_dependencies_result dependency_resolver::resolve_dependencies(
	const std::vector<dependency> &dependencies,
	const std::vector<const injeqt_object *> &objects) const
{
	auto unresolved = std::vector<dependency>{};
	auto resolved = std::vector<resolved_dependency>{};
	for (auto &&dependency : dependencies)
	{
		auto mapped = resolve_dependency(dependency, objects);
		if (mapped)
			resolved.emplace_back(dependency, *mapped);
		else
			unresolved.emplace_back(dependency);
	}
	return {unresolved, resolved};
}

const injeqt_object * dependency_resolver::resolve_dependency(
	const dependency & dependency,
	const std::vector<const injeqt_object *> &objects) const
{
	auto it = std::find_if(std::begin(objects), std::end(objects),
		[&dependency](const injeqt_object *object){ return object->meta().implements(dependency.type()); }
	);
	return it == std::end(objects)
		? nullptr
		: *it;
}

}}

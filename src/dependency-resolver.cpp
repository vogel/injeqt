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

	auto to_resolve_it = begin(to_resolve);
	auto to_resolve_end = end(to_resolve);
	auto objects_it = begin(objects);
	auto objects_end = end(objects);

	while (to_resolve_it != to_resolve_end && objects_it != objects_end)
	{
		auto to_resolve_address = to_resolve_it->type();
		auto objects_address = std::addressof((*objects_it)->meta().type());
		if (to_resolve_address == objects_address)
		{
			resolved.emplace_back(*to_resolve_it, **objects_it);
			++to_resolve_it;
			++objects_it;
		}
		else if (to_resolve_address < objects_address)
		{
			unresolved.emplace_back(*to_resolve_it);
			++to_resolve_it;
		}
		else
			++objects_it;
	}

	while (to_resolve_it != to_resolve_end)
	{
		unresolved.emplace_back(*to_resolve_it);
		to_resolve_it++;
	}

	return {unresolved, resolved};
}


}}

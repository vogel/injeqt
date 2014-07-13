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

#pragma once

#include "dependencies.h"
#include "injeqt-global.h"
#include "objects-with-meta.h"

#include <tuple>
#include <unordered_map>
#include <vector>

namespace injeqt { namespace v1 {

class object_with_meta;
class resolved_dependency;

struct resolve_dependencies_result
{
	dependencies unresolved;
	std::vector<resolved_dependency> resolved;
};

// TODO: if required can be speed up by chaning vectors of dependencies
// and objects to sorted vectors and to require objects to have item
// for each class in hierarchy, not only the most specific one
// that would change resolve time from n*n to n
class dependency_resolver final
{

public:
	resolve_dependencies_result resolve_dependencies(
		const dependencies &to_resolve,
		const objects_with_meta &objects) const;
	const object_with_meta * resolve_dependency(
		const dependency &dependency,
		const objects_with_meta &objects) const;

};

}}

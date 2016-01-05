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

#include <injeqt/injeqt.h>

#include "dependencies.h"
#include "implementations.h"
#include "internal.h"

#include <vector>

/**
 * @file
 * @brief Contains classes and functions for resolving dependencies.
 */

namespace injeqt { namespace internal {

class resolved_dependency;

/**
 * @brief Structure for holding result of resolving dependencies. *
 */
struct resolve_dependencies_result
{
	/**
	 * @brief All dependencies that were not resolved.
	 */
	dependencies unresolved;

	/**
	 * @brief All dependencies that were resolved.
	 *
	 * There is no need to keep the result in sorted_unique_vector based class - resolve_dependencies
	 * function is guaranteed to return only unique result and that field is not matched against any
	 * other collection. If that condition changes this can be easily turned into sorted_unique_vector.
	 */
	std::vector<resolved_dependency> resolved;
};

/**
 * @brief Resolve set of dependencies with set of implementations objects.
 *
 * This function matches each dependency with implementation with the same same type. If any dependency does not
 * have corresponding object - it is added to resolve_dependencies_result::unresolved field. All matching dependency -
 * implementation pairs are added to resolve_dependencies_result::resolved field.
 *
 * This function requires that all items in both sets are valid. In other case its behavior is undefined.
 * This function returns only valid objects.
 */
INJEQT_INTERNAL_API resolve_dependencies_result resolve_dependencies(const dependencies &to_resolve, const implementations &resolve_with);

}}

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
#include <injeqt/type.h>

#include "dependency.h"
#include "types-by-name.h"

/**
 * @file
 * @brief Contains classes and functions for representing set of Injeqt dependencies.
 */

namespace injeqt { namespace internal {

/**
 * @brief Extract type from dependency for storting purposes.
 */
inline type type_from_dependency(const dependency &d)
{
	return d.required_type();
}

/**
 * @brief Abstraction of Injeqt set of dependencies.
 *
 * This set is used to represent all dependencies of one class. It is not possible
 * to store two dependnecies with the same dependency::required_type() values in it.
 * Set is sorted by dependency::required_type() and it can be matched with other
 * type based sets (like implementations) using match() function.
 *
 * The best way to create instance of this type is to call make_validated_dependencies(const type &).
 */
using dependencies = sorted_unique_vector<type, dependency, type_from_dependency>;

/**
 * @brief Extract set of dependencies from type.
 * @param for_type type to extract dependencies from.
 * @pre !for_type.is_empty()
 * @throw dependency_duplicated when one type occurs twice as a dependency.
 * @throw dependency_on_self when type depends on self.
 * @throw dependency_on_subtype when type depends on own supertype.
 * @throw dependency_on_subtype when type depends on own subtype.
 * @throw invalid_setter if any tagged setter has parameter that is not a QObject-derived pointer
 * @throw invalid_setter if any tagged setter has parameter that is a QObject pointer
 * @throw invalid_setter if any tagged setter has other number of parameters than one
 *
 * This function computes and returns set of all dependencies of a given type. All slots methods tagged
 * with INJEQT_SET are describing dependnecies. If all dependnecies are valid, there is no duplication,
 * and type does not depends on self, subtype or supertype, a result is returned. Otherwise one of many
 * exceptions can be thrown.
 */
dependencies extract_dependencies(const types_by_name &known_types, const type &for_type);

}}

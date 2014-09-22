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

#include "implemented-by.h"
#include "injeqt.h"
#include "sorted-unique-vector.h"

/**
 * @file
 * @brief Contains classes and functions for representing set of Injeqt implemented_by objects.
 */

namespace injeqt { namespace internal {

/**
 * @brief Extract interface type from implemented_by for storting purposes.
 */
inline type type_from_implemented_by(const implemented_by &i)
{
	return i.interface_type();
}

/**
 * @brief Abstraction of Injeqt set of implemented_by objects.
 *
 * This set is used to represent all currently available types and its mapping to configured types.
 * All available types are interfaces that are implemented by configured type and are not conflicting
 * (interfaces are conflicting when two or more configured types implements them). Only types in
 * this class are available for injector to return objects of. Relations object are sorted by
 * interface types that are unique.
 *
 * This class is mostly used in type_relations to store all available types.
 */
using implemented_by_mapping = sorted_unique_vector<type, implemented_by, type_from_implemented_by>;

}}

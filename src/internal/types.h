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

#include <injeqt/type.h>
#include <injeqt/injeqt.h>

#include "sorted-unique-vector.h"

/**
 * @file
 * @brief Contains classes and functions for representing set of Injeqt types.
 */

namespace injeqt { namespace internal {

/**
 * @brief Identity function.
 */
inline type type_from_type(const type &i)
{
	return i;
}

/**
 * @brief Set of type objects.
 */
using types = sorted_unique_vector<type, type, type_from_type>;

}}

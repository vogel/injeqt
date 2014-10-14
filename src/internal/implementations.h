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

#include "implementation.h"
#include "sorted-unique-vector.h"

/**
 * @file
 * @brief Contains classes and functions for representing set of Injeqt implementation objects.
 */

namespace injeqt { namespace internal {

/**
 * @brief Extract type from implementation for storting purposes.
 */
inline type type_from_implementation(const implementation &i)
{
	return i.interface_type();
}

/**
 * @brief Abstraction of Injeqt set of implementation objects.
 *
 * This set is used to represent all currently available implementations. Implementations are sorted
 * by types and this class only allows one implementation of given type to exists. Set is sorted
 * by implementation::interface_type() and it can be matched with other type based sets (like dependencies)
 * using match() function.
 *
 * This class is mostly used by injector_impl to store objects created by injector or provided to it
 * by module::add_ready_object<T>(QObject *).
 */
using implementations = sorted_unique_vector<type, implementation, type_from_implementation>;

}}

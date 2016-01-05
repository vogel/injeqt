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

#include "internal.h"
#include "types.h"

/**
 * @file
 * @brief Contains functions for extracting and checking interfaces from a type.
 */

namespace injeqt { namespace internal {

/**
 * @brief Return list of interfaces that given type implements.
 * @pre !for_type.is_empty()
 *
 * QObject meta type system supports only single inheritance. This method
 * gets all QObject-based ancestors of for_type (including for_type itself,
 * excluding QObject) and returns it as a types collection. If for_type
 * object is not valid an empty collection is returned.
 */
INJEQT_INTERNAL_API types extract_interfaces(const type &for_type);

/**
 * @brief Return true if @p implementation implements @p interface
 * @pre !implementation.is_empty()
 * @pre !interface.is_empty()
 */
INJEQT_INTERNAL_API bool implements(const type &implementation, const type &interface);

}}

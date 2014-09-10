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

#include "injeqt.h"

/**
 * @file
 * @brief Various utils.
 */

namespace injeqt { namespace v1 {

/**
 * @brief Helper function for creating valid objects.
 * @tparam T type of object to create
 * @param args argumetns to T constructor
 *
 * In injeqt most of the object can be created both in valid and invalid state. It is assumed
 * that each object that comes or is constructor from objects from outside of library can be
 * invalid and must be checked by validate<T>(T) free function that throws exception when that
 * case occurs. Objects created inside library from other valid objects must be valid (it is
 * developer responsibility to ensure that this is true).
 *
 * For simplicity make_validated function was added. It created new object with provided arguments
 * and calls validate<T>(T) free function on it. It can only return objects that are valid -
 * otherwise an exception is thrown.
 */
template<typename T, typename ...Args>
T make_validated(Args&& ...args)
{
	T result{std::forward<Args>(args)...};
	validate(result);
	return result;
}

}}

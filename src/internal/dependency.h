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
#include "setter-method.h"
#include "type.h"

/**
 * @file
 * @brief Contains classes and functions for abstractions of Injeqt dependencies.
 */

namespace injeqt { namespace internal {

/**
 * @brief Abstraction of Injeqt dependency.
 *
 * Current version of Injeqt supports only setter-based dependencies between classes.
 * Dependency is based on setter_method class - it contains one object of setter_method
 * type to represent method used to resolve dependency. It also has helper function
 * required_type() to get a type that of object can be accepted as a dependency.
 *
 * Objects of this type are valid only when backing setter_method is valid.
 */
class dependency final
{

public:
	/**
	 * @brief Create dependency instance that uses setter method to be resolved.
	 * @param setter setter method used to resolve dependency
	 * @pre !setter.is_empty()
	 */
	explicit dependency(setter_method setter);

	/**
	 * @return type that is required to resolve dependency
	 *
	 * Objects that resolve must be either of type returned by this method or
	 * of type that derives from it. This type is the same as setter().parameter_type().
	 */
	const type & required_type() const;

	/**
	 * @return setter method that must be used to resolve dependency
	 *
	 * Returned method can be invoked on object that is of type that contains this
	 * dependency to resolve it.
	 */
	const setter_method & setter() const;

private:
	setter_method _setter;

};

bool operator == (const dependency &x, const dependency &y);
bool operator != (const dependency &x, const dependency &y);

}}

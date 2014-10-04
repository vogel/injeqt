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
#include "injeqt.h"
#include "type.h"

#include <cassert>

/**
 * @file
 * @brief Contains classes and functions for representing type and its dependencies.
 */

namespace injeqt { namespace internal {

/**
 * @brief Type and its dependencies.
 *
 * This class is used to represent type with connection of all its dependencies. Instances created
 * with make_type_dependencies(type) function are guaranteed to be valid.
 *
 * Used in model class to store all dependnecies and in injector_impl to resolve them.
 */
class type_dependencies final
{

public:
	/**
	 * @brief Create new instance of type_dependencies.
	 * @param dependent_type type that contains dependencies described with dependency_list
	 * @param dependency_list list of dependencies of dependent_type
	 */
	explicit type_dependencies(type dependent_type, dependencies dependency_list);

	/**
	 * @return type that contains dependencies described with dependency_list
	 */
	const type & dependent_type() const;

	/**
	 * @return list of dependencies of dependent_type()
	 */
	const dependencies & dependency_list() const;

private:
	type _dependent_type;
	dependencies _dependency_list;

};

bool operator == (const type_dependencies &x, const type_dependencies &y);
bool operator != (const type_dependencies &x, const type_dependencies &y);

/**
 * @brief Create valid type_dependencies from given type.
 * @param dependent_type type to get dependecies from.
 * @pre !dependent_type.is_empty()
 * @throw dependency_duplicated_exception when one type occurs twice as a dependency
 * @throw dependency_on_self_exception when type depends on self
 * @throw dependency_on_subtype_exception when type depends on own supertype
 * @throw dependency_on_subtype_exception when type depends on own subtype
 * @throw invalid_setter_exception if any tagged setter has parameter that is not a QObject-derived pointer
 * @throw invalid_setter_exception if any tagged setter has parameter that is not a QObject pointer
 * @throw invalid_setter_exception if any tagged setter has other number of parameters than one
 */
inline type_dependencies make_type_dependencies(type dependent_type)
{
	assert(!dependent_type.is_empty());

	return type_dependencies{dependent_type, extract_dependencies(dependent_type)};
}

}}

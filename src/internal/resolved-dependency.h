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

#include "implementation.h"
#include "injeqt.h"
#include "setter-method.h"

/**
 * @file
 * @brief Contains classes and functions for resolved dependencies.
 */

using namespace injeqt::v1;

namespace injeqt { namespace internal {

INJEQT_EXCEPTION(invalid_resolved_dependency_exception, injeqt_exception);
INJEQT_EXCEPTION(non_matching_setter_exception, invalid_resolved_dependency_exception);
INJEQT_EXCEPTION(inavalid_apply_on_object_exception, invalid_resolved_dependency_exception);

/**
 * @brief Abstraction of Injeqt resolved dependency.
 *
 * Resolved dependency consists of implementation and a setter_method. To resolve dependency
 * on an object call apply_on(QObject *) method. To check if resolved dependency is valid
 * call validate(const resolved_dependency &). To get instances of resolved_dependency call
 * resolve_dependencies(const dependencies &, const implementations &). This class is currently
 * only used in injector_impl.
 */
class resolved_dependency final
{

public:
	/**
	 * @brief Create new resolved_dependency object.
	 * @param resolved_with object that resolve dependency
	 * @param setter setter_method that can be used to resolve dependency on an object instance
	 */
	explicit resolved_dependency(implementation resolved_with, setter_method setter);

	/**
	 * @return object that resolve dependency.
	 */
	const implementation & resolved_with() const;

	/**
	 * @return setter method used to resolve dependency on an object instance.
	 */
	const setter_method & setter() const;

	/**
	 * @brief Apply resolved dependency on an object instance.
	 * @param on object to apply dependency on
	 * @throw inavalid_apply_on_object_exception when @p on parameter is null
	 * @throw inavalid_apply_on_object_exception when @p on parameter is of invalid type
	 *
	 * This method can only be called on valid resolved_dependency object. If @p on parameter is
	 * null or is of different type than setter_method::object_type() of setter() object an
	 * exception will be thrown.
	 */
	bool apply_on(QObject *on);

private:
	implementation _resolved_with;
	setter_method _setter;

};

/**
 * @brief Throws an exception if resolved_dependency rd is not valid.
 * @param rd resolved_dependency to validate
 * @throw invalid_implementation_availability_exception when backing implementation object is invalid
 * @throw invalid_interface_type_exceptionwhen when backing implementation object is invalid
 * @throw bad_number_of_parameters_setter_exception when backing setter_method object is invalid
 * @throw invalid_setter_exception when backing setter_method object is invalid
 * @throw invalid_type_exception when backing setter_method object is invalid
 * @throw invalid_type_exception when backing setter_method object is invalid
 * @throw non_matching_setter_exception when type of setter_method parameter is not the same as type of implementation object
 *
 * Call to validate resolved_dependency rd.
 */
void validate(const resolved_dependency &rd);

bool operator == (const resolved_dependency &x, const resolved_dependency &y);
bool operator != (const resolved_dependency &x, const resolved_dependency &y);

}}

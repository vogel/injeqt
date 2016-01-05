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
#include "internal.h"
#include "setter-method.h"

/**
 * @file
 * @brief Contains classes and functions for resolved dependencies.
 */

namespace injeqt { namespace internal {

/**
 * @brief Abstraction of Injeqt resolved dependency.
 *
 * Resolved dependency consists of implementation and a setter_method. To resolve dependency
 * on an object call apply_on(QObject *) method. To get instances of resolved_dependency call
 * resolve_dependencies(const dependencies &, const implementations &). This class is currently
 * only used in injector_core.
 */
class INJEQT_INTERNAL_API resolved_dependency final
{

public:
	/**
	 * @brief Create new resolved_dependency object.
	 * @param resolved_with object that resolve dependency
	 * @param setter setter_method that can be used to resolve dependency on an object instance
	 * @pre !setter.is_empty()
	 * @pre resolved_with.interface_type() == setter.parameter_type()
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
	 * @pre on != nullptr
	 * @pre type{on->metaObject()} == setter().object_type()
	 *
	 * This method can only be called on valid resolved_dependency object.
	 */
	bool apply_on(QObject *on);

private:
	implementation _resolved_with;
	setter_method _setter;

};

INJEQT_INTERNAL_API bool operator == (const resolved_dependency &x, const resolved_dependency &y);
INJEQT_INTERNAL_API bool operator != (const resolved_dependency &x, const resolved_dependency &y);

}}

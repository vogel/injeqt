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

#include "exception/exception.h"
#include "injeqt.h"
#include "type.h"

/**
 * @file
 * @brief Contains classes and functions for defining implementation of type.
 */

class QObject;

namespace injeqt { namespace internal {

/**
 * @brief Connects type with object that implements it.
 *
 * This class is used to connect type with object that implements that type.
 * To ensure that type and object matched call free function validate(const implementation&).
 * To create valid object call free function make_validated<implementation>(type, QObject *). Both
 * of these will throw if object is null or if type and object does not match.
 *
 * This class is mostly used in internal code to store objects in cache and to pass
 * them as resolved dependencies. The only place where this class is created from outside
 * data is module::add_ready_object<T>(QObject *) where a validation is used to ensure
 * that user passed valid data.
 */
class implementation final
{

public:
	/**
	 * @brief Create new instance.
	 * @param interface_type type that object implements
	 * @param object object that should implement interface_type
	 * @pre !interface_type.is_empty()
	 * @pre !interface_type.is_qobject()
	 * @pre object != nullptr
	 * @pre object->metaObject() != nullptr
	 * @pre extract_interfaces(type{object->metaObject()}).contains(interface_type)
	 *
	 * Always succedes. To check if preconditions of class are meet
	 * call validate(const implementation&)
	 */
	explicit implementation(type interface_type, QObject *object);

	const type & interface_type() const;
	QObject * object() const;

private:
	type _interface_type;
	QObject *_object;

};

bool operator == (const implementation &x, const implementation &y);
bool operator != (const implementation &x, const implementation &y);

/**
 * @brief Check if @p interface_type and @p object met implementation preconditions and return new instance if true
 * @param interface_type type that object implements
 * @param object object that should implement interface_type
 * @throw empty_type_exception when passed type @p interface_type is an empty type
 * @throw qobject_type_exception when passed type @p interface_type represents QObject
 * @throw invalid_qobject_exception when passed @p object is nullptr or does not have valid QMetaType
 * @throw interface_not_implemented_exception when passed @p object does not implements type @p interface_type
 */
implementation make_implementation(type interface_type, QObject *object);

}}

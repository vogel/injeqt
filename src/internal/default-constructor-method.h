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

#include <memory>
#include <QtCore/QMetaMethod>

/**
 * @file
 * @brief Contains classes and functions for representing default constructors.
 */

class QObject;

namespace injeqt { namespace internal {

/**
 * @brief Any exception that can occur when validating or creating default_constructor_method objects.
 */
INJEQT_EXCEPTION(default_constructor_exception, ::injeqt::v1::exception::exception);

/**
 * @brief Exception thrown when default constructor was not found in given type.
 */
INJEQT_EXCEPTION(no_default_constructor_exception, default_constructor_exception);

/**
 * @brief Abstraction of default constructor.
 *
 * This class is used for creating objects of types configured with module::add_type<T>().
 * Default constructor is crated from Qt type QMetaMethod. As Qt only creates QMetaMethod
 * objects for slots, signals and all methods marked with Q_INVOKABLE tag - Injeqt
 * requires that default constructor is also marked with that tag. It is also not allowed
 * to have any parameters without default values.
 *
 * Example of valid default constructor:
 *
 *     class with_default_constructor : public QObject
 *     {
 *         Q_OBJECT
 *     public:
 *         Q_INVOKABLE with_default_constructor(QObject * parent = nullptr) {}
 *     };
 *
 * To extract constructor from a type call make_default_constructor_method<T>().
 */
class default_constructor_method final
{

public:
	/**
	 * @brief Create empty default_constructor_method.
	 */
	default_constructor_method();

	/**
	 * @brief Create object from QMetaMethod definition.
	 * @param meta_method Qt meta method that should be a default constructor
	 * @pre meta_method.methodType() == QMetaMethod::Constructor
	 * @pre meta_method.parameterCount() == 0
	 * @pre meta_method.enclosingMetaObject() != nullptr
	 */
	explicit default_constructor_method(QMetaMethod meta_method);

	/**
	 * @return true if default_constructor_method is empty and does not represent constructor
	 */
	bool is_empty() const;

	/**
	 * @return Type of objects created by default constructor.
	 * @pre !is_empty()
	 */
	const type & object_type() const;

	/**
	 * @return Qt representation of default constructor.
	 *
	 * May return empty value if object is empty.
	 */
	const QMetaMethod & meta_method() const;

	/**
	 * @return New instance of type of default constructor.
	 * @pre !is_empty()
	 */
	std::unique_ptr<QObject> invoke() const;

private:
	type _object_type;
	QMetaMethod _meta_method;

};

bool operator == (const default_constructor_method &x, const default_constructor_method &y);
bool operator != (const default_constructor_method &x, const default_constructor_method &y);

/**
 * @brief Extract default constructor method from given type T.
 * @tparam t type to extract constructor from
 * @pre !t.is_empty() && !t.is_qobject()
 *
 * This method looks for all constructors of type t tagged with Q_INVOKABLE and returns
 * default_constructor_method based on one without any parameters or with all parameters
 * with default values. If no such constructor is found - empty default_constructor_method
 * is returned.
 */
default_constructor_method make_default_constructor_method(const type &t);

}}

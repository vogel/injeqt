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

#include "injeqt-exception.h"
#include "injeqt.h"
#include "type.h"

#include <memory>
#include <QtCore/QMetaMethod>

/**
 * @file
 * @brief Contains classes and functions for representing default constructors.
 */

class QObject;

using namespace injeqt::v1;

namespace injeqt { namespace internal {

INJEQT_EXCEPTION(default_constructor_exception, injeqt_exception);
INJEQT_EXCEPTION(no_default_constructor_exception, default_constructor_exception);
INJEQT_EXCEPTION(invalid_default_constructor_exception, default_constructor_exception);

/**
 * @brief Abstraction of default constructor.
 *
 * This class is used for creating objects of types configured with module::add_type<T>().
 * Default constructor is crated from Qt type QMetaMethod. As Qt only creates QMetaMethod
 * objects for slots, signals and all methods marked with Q_INVOKABLE tag - Injeqt
 * requires that default constructor is also marked with that tag. It is also not allowed
 * to have any parameters - even with default values.
 *
 * Example of valid default constructor:
 *
 *     class with_default_constructor : public QObject
 *     {
 *         Q_OBJECT
 *     public:
 *         Q_INVOKABLE with_default_constructor() {}
 *     };
 *
 * To check if object is valid call validate(const default_constructor_method &).
 * To extract constructor from a type call make_default_constructor_method<T>().
 */
class default_constructor_method final
{

public:
	/**
	 * @brief Create object from QMetaMethod definition.
	 * @param meta_method Qt meta method that should be a default constructor
	 */
	explicit default_constructor_method(QMetaMethod meta_method);

	/**
	 * @return Type of objects created by default constructor.
	 *
	 * May return invalid type if QMetaMethod passed in constructor was no valid.
	 */
	const type & object_type() const;

	/**
	 * @return Qt representation of default constructor.
	 *
	 * May return empty value if QMetaMethod passed in constructor was no valid.
	 */
	const QMetaMethod & meta_method() const;

	/**
	 * @return New instance of type of default constructor.
	 *
	 * This method can be only called on valid objects. To ensure that default
	 * constructor is valid call validate(const default_constructor_method &).
	 */
	std::unique_ptr<QObject> invoke() const;

private:
	type _object_type;
	QMetaMethod _meta_method;

};

/**
 * @brief Throws an exception if default_constructor_method d is not valid.
 * @param d default_constructor_method to validate
 * @throws invalid_default_constructor_exception if backing QMetaMethod is not a constructor
 * @throws invalid_default_constructor_exception if backing QMetaMethod contains paramers
 * @throws invalid_type_exception if backing QMetaMethod comes from invalid type
 *
 * Call to validate default_constructor_method d. If backing QMetaMethod is invalid, is not
 * a constructor or requires paramters - an exception is thrown.
 */
void validate(const default_constructor_method &d);

bool operator == (const default_constructor_method &x, const default_constructor_method &y);
bool operator != (const default_constructor_method &x, const default_constructor_method &y);

/**
 * @see make_default_constructor_method<T>()
 */
default_constructor_method make_default_constructor_method(const type &t);

/**
 * @brief Extract default constructor method from given type T.
 * @tparam T type to extract constructor from
 * @throw no_default_constructor_exception if no default constructor is found.
 *
 * This method looks for all constructors of type T tagged with Q_INVOKABLE and returns
 * default_constructor_method based on one without any parameters (even with default
 * values). If no such constructor is found - no_default_constructor_exception is thrown.
 */
template<typename T>
inline default_constructor_method make_default_constructor_method()
{
	return make_default_constructor_method(make_type<T>());
}

}}

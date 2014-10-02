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

#include <QtCore/QMetaMethod>

/**
 * @file
 * @brief Contains classes and functions for representing setter methods.
 */

namespace injeqt { namespace internal {

/**
 * @brief Any exception that can occur when validating or creating setter_method objects or invoking setter_method.
 */
INJEQT_EXCEPTION(setter_exception, ::injeqt::v1::exception::exception);

/**
 * @brief Exception thrown when setter was invoked with parameter of different type than setter_method::parameter_type().
 */
INJEQT_EXCEPTION(invoked_with_wrong_object_exception, setter_exception);

/**
 * @brief Abstraction of setter method.
 *
 * This class is used for defining setter based dependencies and for invoking these setter methods
 * to resolve dependencies. Setter method is crated from Qt type QMetaMethod. As Qt only creates QMetaMethod
 * objects for slots, signals and all methods marked with Q_INVOKABLE tag and Injeqt requires
 * all its setter methods to be tagged with INJEQT_SETTER - setter method must be a slot. This method should return
 * void is allowed to have only one parameter of pointer to type inherited from QObject. This type must
 * be also configured in one of module to be able to be resolved.
 *
 * Example of valid setter method:
 *
 *     class set_object : public QObject
 *     {
 *         Q_OBJECT
 *     };
 *
 *     class with_setter : public QObject
 *     {
 *         Q_OBJECT
 *     public slots:
 *         INJEQT_SETTER void setter(set_object *obj) { ... }
 *     };
 *
 * Object with setter method must not take ownership of passed object.
 */
class setter_method final
{

public:
	/**
	 * @brief Create empty setter_method.
	 */
	setter_method();

	/**
	 * @brief Create object from QMetaMethod definition.
	 * @param meta_method Qt meta method that should be a setter method
	 * @pre meta_method.methodType() == QMetaMethod::Slot
	 * @pre meta_method.parameterCount() == 1
	 * @pre meta_method.enclosingMetaObject() != nullptr
	 * @pre !type{QMetaType::metaObjectForType(meta_method.parameterType(0))}.is_empty()
	 */
	explicit setter_method(QMetaMethod meta_method);

	/**
	 * @return true if setter_method is empty and does not represent valie setter method
	 */
	bool is_empty() const;

	/**
	 * @return Type of objects that owns this setter method.
	 * @pre !is_empty()
	 *
	 * May return invalid type if QMetaMethod passed in constructor was invalid.
	 */
	const type & object_type() const;

	/**
	 * @return Type of objects parameter accepted by setter method.
	 * @pre !is_empty()
	 *
	 * May return invalid type if QMetaMethod passed in constructor was invalid.
	 */
	const type & parameter_type() const;

	/**
	 * @return Qt representation of setter method.
	 *
	 * May return empty value if QMetaMethod passed in constructor was invalid.
	 */
	const QMetaMethod & meta_method() const;

	/**
	 * @return String signature of setter method.
	 *
	 * May return empty value if QMetaMethod passed in constructor was invalid.
	 */
	std::string signature() const;

	/**
	 * @param on object to call this method on
	 * @param parameter parmeter to be passed in invocation
	 * @return true if invoke was successfull
	 * @pre !is_empty()
	 * @pre on != nullptr
	 * @pre type{on->metaObject()} == object_type()
	 * @throw invoked_with_wrong_object_exception if @p parameter is null
	 * @throw invoked_with_wrong_object_exception if @p parameter type does not implement parameter_type()
	 *
	 * This method can be only called on valid objects with @p on parameter being
	 * the same type as object_type() returns and @p parameter of type that implements parameter_type().
	 *
	 * Calling this on invalid object with result in undefined behavior.
	 */
	bool invoke(QObject *on, QObject *parameter) const;

private:
	type _object_type;
	type _parameter_type;
	QMetaMethod _meta_method;

};

bool operator == (const setter_method &x, const setter_method &y);
bool operator != (const setter_method &x, const setter_method &y);

template<typename T>
inline setter_method make_setter_method(const std::string &signature)
{
	return setter_method{T::staticMetaObject.method(T::staticMetaObject.indexOfMethod(signature.data()))};
}

}}

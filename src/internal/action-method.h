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

#include "internal.h"

#include <injeqt/exception/exception.h>
#include <injeqt/injeqt.h>
#include <injeqt/type.h>

#include <QtCore/QMetaMethod>
#include <string>

/**
 * @file
 * @brief Contains classes and functions for representing setter methods.
 */

namespace injeqt { namespace internal {

/**
 * @brief Abstraction of setter method.
 *
 * This class is used for defining setter based dependencies and for invoking these setter methods
 * to resolve dependencies. Setter method is crated from Qt type QMetaMethod. As Qt only creates QMetaMethod
 * objects for slots, signals and all methods marked with Q_INVOKABLE tag and Injeqt requires
 * all its setter methods to be tagged with INJEQT_SET - setter method must be a slot. This method should return
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
 *         INJEQT_SET void setter(set_object *obj) { ... }
 *     };
 *
 * Object with setter method must not take ownership of passed object.
 */
class INJEQT_INTERNAL_API action_method final
{

public:
	static bool is_action_init_tag(const std::string &tag);
	static bool is_action_done_tag(const std::string &tag);

	static bool validate_action_method(const QMetaMethod &meta_method);

	/**
	 * @brief Create empty action_method.
	 */
	action_method();

	/**
	 * @brief Create object from QMetaMethod definition.
	 * @param parameter_type Type of parameter of setter @p meta_method
	 * @param meta_method Qt meta method that should be a setter method
	 * @note Qt QMetaType system limitations with plugins disallow use of QMetaType to retreive parameter type from QMetaMethod.
	 * @pre meta_method.methodType() == QMetaMethod::Slot
	 * @pre meta_method.parameterCount() == 1
	 * @pre meta_method.enclosingMetaObject() != nullptr
	 * @pre !parameter_type.is_empty()
	 * @pre parameter_type.name() + "*" == std::string{meta_method.parameterTypes()[0].data()}
	 */
	explicit action_method(QMetaMethod meta_method);

	/**
	 * @return true if action_method is empty and does not represent valie setter method
	 */
	bool is_empty() const;

	/**
	 * @return Type of objects that owns this setter method.
	 *
	 * May return invalid type if QMetaMethod passed in constructor was invalid.
	 */
	const type & object_type() const;

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
	 * @pre parameter != nullptr
	 * @pre !type{parameter->metaObject()}.is_empty()
	 * @pre implements(type{parameter->metaObject()}, _parameter_type)
	 *
	 * This method can be only called on valid objects with @p on parameter being
	 * the same type as object_type() returns and @p parameter of type that implements parameter_type().
	 *
	 * Calling this on invalid object with result in undefined behavior.
	 */
	bool invoke(QObject *on) const;

private:
	type _object_type;
	QMetaMethod _meta_method;

};

bool operator == (const action_method &x, const action_method &y);
bool operator != (const action_method &x, const action_method &y);

INJEQT_INTERNAL_API action_method make_action_method(const QMetaMethod &meta_method);
std::vector<action_method> extract_actions(const std::string &action_tag, const type &for_type);

}}

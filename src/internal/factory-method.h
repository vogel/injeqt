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
 * @brief Contains classes and functions for representing factory methods.
 */

class QObject;

namespace injeqt { namespace internal {

/**
 * @brief Any exception that can occur when validating or creating factory_method objects.
 */
INJEQT_EXCEPTION(factory_method_exception, ::injeqt::v1::exception::exception);

/**
 * @brief Exception that can occur when validating factory_method objects. Objects created with make_factory_method<T, F>
 *        will never cause that exception to be thrown.
 */
INJEQT_EXCEPTION(invalid_factory_method_exception, factory_method_exception);

/**
 * @brief Exception thrown when factory method returning given type was not found in factory type.
 */
INJEQT_EXCEPTION(no_factory_method_exception, factory_method_exception);

/**
 * @brief Exception thrown when more than one factory method returning given type was found in an factory type.
 */
INJEQT_EXCEPTION(non_unique_factory_exception, factory_method_exception);

/**
 * @brief Abstraction of factory method.
 *
 * This class is used for creating objects of types configured with module::add_factory<T, F>().
 * Factory method is created from Qt type QMetaMethod. As Qt only creates QMetaMethod
 * objects for slots, signals and all methods marked with Q_INVOKABLE tag - Injeqt
 * requires that factory method is marked with that tag or is a slot. This method must return
 * pointer of type inherited from QObject and is not allowed only to have parameters with
 * default values.
 *
 * Example of valid factory method:
 *
 *     class created_object : public QObject
 *     {
 *         Q_OBJECT
 *     };
 *
 *     class with_factory_method : public QObject
 *     {
 *         Q_OBJECT
 *     public:
 *         Q_INVOKABLE created_object * create() { ... }
 *     };
 *
 * To check if object is valid call validate(const factory_method &).
 * To extract constructor from a type call make_factory_method<T, F>().
 * For above example a call to make_factory_method<with_factory_method, created_object>()
 * would return valid object.
 *
 * Note that in Injeqt factory should not own created object - invoking
 * injector own lifetime of it. Method invoke(QObject *) call factory method
 * and immediately wraps returned QObject * in a std::unique_ptr that it later
 * returns.
 */
class factory_method final
{

public:
	/**
	 * @brief Create empty factory_method.
	 */
	factory_method();

	/**
	 * @brief Create object from QMetaMethod definition.
	 * @param meta_method Qt meta method that should be a factory method
	 * @pre meta_method.methodType() == QMetaMethod::Method || meta_method.methodType() == QMetaMethod::Slot
	 * @pre meta_method.parameterCount() == 0
	 * @pre meta_method.enclosingMetaObject() != nullptr
	 */
	explicit factory_method(QMetaMethod meta_method);

	/**
	 * @return true if factory_method is empty and does not represent method
	 */
	bool is_empty() const;

	/**
	 * @return Type of objects that owns this factory method.
	 * @pre !is_empty()
	 */
	const type & object_type() const;

	/**
	 * @return Type of objects created by factory method.
	 * @pre !is_empty()
	 */
	const type & result_type() const;

	/**
	 * @return Qt representation of factory method.
	 *
	 * May return empty value if is_empty()
	 */
	const QMetaMethod & meta_method() const;

	/**
	 * @param on object to call this method on
	 * @return Result on factory method called on given object.
	 * @pre !is_empty()
	 * @pre on != nullptr
	 * @pre meta_method().enclosingMetaObject() is equal to @p on type
	 *
	 * This method can be only called on valid objects with @p on parameter being
	 * the same type as object_type() returns. Invalid invocation with result
	 * in undefined behavior.
	 */
	std::unique_ptr<QObject> invoke(QObject *on) const;

private:
	type _object_type;
	type _result_type;
	QMetaMethod _meta_method;

};

bool operator == (const factory_method &x, const factory_method &y);
bool operator != (const factory_method &x, const factory_method &y);

/**
 * @brief Extract factory method from given type F that returns pointers to T.
 * @tparam t factory method must return T *
 * @tparam f type to extract constructor from
 * @pre !t.is_empty() && !t.is_qobject()
 * @pre !f.is_empty() && !f.is_qobject()
 *
 * This function looks for all methods of type F that are tagged with Q_INVOKABLE, does not
 * accepts argumetns and returns T* or pointer to type derived from T. If only one such method
 * is found it is wrapped in factory_method type and returned. In other cases an empty
 * factory_method is returned.
 */
factory_method make_factory_method(const type &t, const type &f);

}}

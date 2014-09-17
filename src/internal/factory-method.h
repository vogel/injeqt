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
 * @brief Contains classes and functions for representing factory methods.
 */

class QObject;

using namespace injeqt::v1;

namespace injeqt { namespace internal {

/**
 * @brief Any exception that can occur when validating or creating factory_method objects.
 */
INJEQT_EXCEPTION(factory_method_exception, injeqt_exception);

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
 * requires that factory method is marked with that tag or is a slot. This method is must return
 * pointer of type inherited from QObject and is not allowed to have any parameters - even
 * with default values.
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
	 * @brief Create object from QMetaMethod definition.
	 * @param meta_method Qt meta method that should be a factory method
	 */
	explicit factory_method(QMetaMethod meta_method);

	/**
	 * @return Type of objects that owns this factory method.
	 *
	 * May return invalid type if QMetaMethod passed in constructor was no valid.
	 */
	const type & object_type() const;

	/**
	 * @return Type of objects created by factory method.
	 *
	 * May return invalid type if QMetaMethod passed in constructor was no valid.
	 */
	const type & result_type() const;

	/**
	 * @return Qt representation of factory method.
	 *
	 * May return empty value if QMetaMethod passed in constructor was no valid.
	 */
	const QMetaMethod & meta_method() const;

	/**
	 * @param on object to call this method on
	 * @return Result on factory method called on given object.
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

/**
 * @brief Throws an exception if factory_method fm is not valid.
 * @param fm factory_method to validate
 * @throws invalid_factory_method_exception if backing QMetaMethod is not a method or slot
 * @throws invalid_factory_method_exception if backing QMetaMethod contains paramers
 * @throws invalid_factory_method_exception if backing QMetaMethod comes from invalid type
 * @throws invalid_factory_method_exception if backing QMetaMethod does not return valid type
 *
 * Call to validate factory_method fm. If backing QMetaMethod is not valid factory method
 * an exception will be thrown.
 */
void validate(const factory_method &fm);

bool operator == (const factory_method &x, const factory_method &y);
bool operator != (const factory_method &x, const factory_method &y);

/**
 * @see make_default_constructor_method<T, F>()
 */
factory_method make_factory_method(const type &t, const type &f);

/**
 * @brief Extract factory method from given type F that returns pointers to T.
 * @tparam T factory method must return T *
 * @tparam F type to extract constructor from
 * @throw no_factory_method_exception if no factory method returning T* is found.
 * @throw non_unique_factory_exception if more than one factory methods returning T* are found.
 *
 * This function looks for all methods of type F that are tagged with Q_INVOKABLE, does not
 * accepts argumetns and returns T* or pointer to type derived from T. If only one such method
 * is found it is wrapped in factory_method type and returned. In other cases exception is thrown.
 *
 * This function is guaranted to return valid object or throw.
 */
template<typename T, typename F>
inline factory_method make_factory_method()
{
	return make_factory_method(make_type<T>(), make_type<F>());
}

}}

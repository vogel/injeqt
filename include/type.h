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
#include "injeqt-utils.h"
#include "injeqt.h"

#include <QtCore/QMetaType>

/**
 * @file
 * @brief Contains classes and functions for abstractions of C++ types.
 */

/**
 * @brief
 */
namespace injeqt
{

/**
 * @brief
 */
namespace v1
{

/**
 * @brief Exception thrown if type passed to validate(type) function is not valid.
 */
INJEQT_EXCEPTION(invalid_type_exception, ::injeqt::v1::exception::exception);

/**
 * @brief Abstraction of C++ type.
 *
 * Represents a C++ type. Currently only types inheriting from QObject class can
 * be represented. Object is created with QMetaObject instance. If provided QMetaObject
 * is nullptr or designates QObject class, then resulting type is invalid. Otherwise
 * resulting type is valid.
 *
 * Direct instantiations of this class should not be needed in user code.
 *
 * Use convienance free function make_type&lt;T&gt; that also registers type T
 * in Qt meta-type that is required for Injeqt to function properly.
 *
 * To check type validity use validate(type) free function. Internal code assumes
 * that all passes type instances are valid. All public API functions always check
 * user-provided types validity and throw an invalid_type_exception if neccessary.
 *
 * Type name can be retrieived by name() member. Original QMetaObject can be read
 * using meta_object() member.
 */
class INJEQT_API type final
{

public:
	/**
	 * @param meta_object QMetaObject representing QObject-based type.<br>
	 *                    New object does not take ownership of meta_object. QMetaObject
	 *                    pointers in Qt are usually pointers to static members.
	 *
	 * Create new type instance. If passed meta_object is nullptr or designates
	 * QObject class, then new type is invalid.
	 */
	explicit type(const QMetaObject * meta_object);

	/**
	 * Return unique name of type. Requires type to be valid.
	 */
	std::string name() const;

	/**
	 * Return base QMetaObject type. If type is not valid, result may be nullptr.
	 */
	const QMetaObject * meta_object() const;

private:
	const QMetaObject * _meta_object;

};

/**
 * @brief Throws an exception if type t is not valid.
 * @param t type to validate
 * @throws invalid_type_exception if type is not derived from QObject.
 *
 * Call to validate type t. If t does not represent a QObject-derived type, an
 * exception of type invalid_type_exception is thrown. If t is valid, this
 * function returns.
 *
 * Note that QObject itself is also an invalid Injeqt type.
 */
INJEQT_API void validate(const type &t);

INJEQT_API bool operator == (const type &x, const type &y);
INJEQT_API bool operator != (const type &x, const type &y);
INJEQT_API bool operator < (const type &x, const type &y);
INJEQT_API bool operator > (const type &x, const type &y);
INJEQT_API bool operator <= (const type &x, const type &y);
INJEQT_API bool operator >= (const type &x, const type &y);

/**
 * @brief Create valid instance of type class.
 * @tparam T type to be represented by new object
 * @throws invalid_type_exception if type is not derived from QObject.
 *
 * Call to create type object abstracting T. T must be a class based on
 * QObject with Q_OBJECT macro defined. In other cases invalid_type_exception is
 * thrown.
 *
 * This function registers T in Qt meta type systems, because sometimes automatic
 * registration does not occur as soon as it is required by Injeqt.
 *
 * Example usage:
 *
 *     class injectable : public QObject
 *     {
 *         Q_OBJECT
 *     };
 *
 *     auto injectable_type = make_validated_type<injectable>();
 */
template<typename T>
inline type make_validated_type()
{
	qRegisterMetaType<T *>();
	return make_validated<type>(&T::staticMetaObject);
}

}

}

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
#include "injeqt-global.h"

class QMetaObject;

namespace injeqt { namespace v1 {

DEFINE_EXCEPTION(type_exception, injeqt_exception);
DEFINE_EXCEPTION(invalid_type_exception, type_exception);

class INJEQT_API type final
{

public:
	explicit type(const QMetaObject * meta_object);

	const QMetaObject * meta_object() const;

private:
	const QMetaObject * _meta_object;

};

INJEQT_API bool operator == (const type &x, const type &y);
INJEQT_API bool operator != (const type &x, const type &y);
INJEQT_API bool operator < (const type &x, const type &y);
INJEQT_API bool operator > (const type &x, const type &y);
INJEQT_API bool operator <= (const type &x, const type &y);
INJEQT_API bool operator >= (const type &x, const type &y);

template<typename T>
inline type make_type()
{
	return type{std::addressof(T::staticMetaObject)};
}

}}

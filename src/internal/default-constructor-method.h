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
#include "type.h"

#include <memory>
#include <QtCore/QMetaMethod>

class QObject;

using namespace injeqt::v1;

namespace injeqt { namespace internal {

DEFINE_EXCEPTION(default_constructor_exception, injeqt_exception);
DEFINE_EXCEPTION(no_default_constructor_exception, default_constructor_exception);
DEFINE_EXCEPTION(invalid_default_constructor_exception, default_constructor_exception);

class default_constructor_method final
{

public:
	explicit default_constructor_method(QMetaMethod meta_method);

	const type & object_type() const;

	std::unique_ptr<QObject> invoke() const;

private:
	type _object_type;
	QMetaMethod _meta_method;

};

bool operator == (const default_constructor_method &x, const default_constructor_method &y);
bool operator != (const default_constructor_method &x, const default_constructor_method &y);

default_constructor_method make_default_constructor_method(const type &t);

template<typename T>
inline default_constructor_method make_default_constructor_method()
{
	return make_default_constructor_method(make_type<T>());
}

}}

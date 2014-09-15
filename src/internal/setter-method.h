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

#include <QtCore/QMetaMethod>

using namespace injeqt::v1;

namespace injeqt { namespace internal {

DEFINE_EXCEPTION(setter_exception, injeqt_exception);
DEFINE_EXCEPTION(invalid_setter_exception, setter_exception);
DEFINE_EXCEPTION(bad_number_of_parameters_setter_exception, invalid_setter_exception);
DEFINE_EXCEPTION(invoked_on_wrong_object_exception, setter_exception);
DEFINE_EXCEPTION(invoked_with_wrong_object_exception, setter_exception);

class setter_method final
{

public:
	explicit setter_method(QMetaMethod meta_method);

	const type & object_type() const;
	const type & parameter_type() const;
	const QMetaMethod & meta_method() const;

	std::string signature() const;

	bool invoke(QObject *on, QObject *parameter) const;

private:
	type _object_type;
	type _parameter_type;
	QMetaMethod _meta_method;

};

void validate(const setter_method &s);

bool operator == (const setter_method &x, const setter_method &y);
bool operator != (const setter_method &x, const setter_method &y);

template<typename T>
inline setter_method make_setter_method(const std::string &signature)
{
	return make_validated<setter_method>(T::staticMetaObject.method(T::staticMetaObject.indexOfMethod(signature.data())));
}

}}

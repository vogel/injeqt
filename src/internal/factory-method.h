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

class QObject;

using namespace injeqt::v1;

namespace injeqt { namespace internal {

INJEQT_EXCEPTION(factory_method_exception, injeqt_exception);
INJEQT_EXCEPTION(no_factory_method_exception, factory_method_exception);
INJEQT_EXCEPTION(invalid_factory_method_exception, factory_method_exception);
INJEQT_EXCEPTION(non_unique_factory_exception, factory_method_exception);

class factory_method final
{

public:
	explicit factory_method(QMetaMethod meta_method);

	const type & object_type() const;
	const type & result_type() const;
	const QMetaMethod & meta_method() const;

	std::unique_ptr<QObject> invoke(QObject *on) const;

private:
	type _object_type;
	type _result_type;
	QMetaMethod _meta_method;

};

void validate(const factory_method &fm);

bool operator == (const factory_method &x, const factory_method &y);
bool operator != (const factory_method &x, const factory_method &y);

factory_method make_factory_method(const type &t, const type &f);

template<typename F, typename T>
inline factory_method make_factory_method()
{
	return make_factory_method(make_type<F>(), make_type<T>());
}

}}

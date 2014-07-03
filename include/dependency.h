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

#include "injeqt-global.h"

#include <QtCore/QMetaMethod>

class QMetaObject;

namespace injeqt { namespace v1 {

enum class dependency_apply_method;

class dependency final
{

public:
	dependency(dependency_apply_method apply_method, const QMetaObject &type, QMetaMethod setter_method);

	dependency_apply_method apply_method() const;
	const QMetaObject & type() const;
	QMetaMethod setter_method() const;

private:
	dependency_apply_method _apply_method;
	const QMetaObject &_type;
	QMetaMethod _setter_method;

};

bool operator == (const dependency &first, const dependency &second);
bool operator != (const dependency &first, const dependency &second);

}}

namespace std {

template<>
struct hash<injeqt::v1::dependency>
{
	size_t operator() (const injeqt::v1::dependency& dependency) const;
};

}

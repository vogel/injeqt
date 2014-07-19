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
#include "type.h"

#include <QtCore/QMetaMethod>

namespace injeqt { namespace v1 {

class object_with_meta;

class setter_method final
{

public:
	explicit setter_method(type object_type, type parameter_type, QMetaMethod meta_method);

	type object_type() const;
	type parameter_type() const;
	QMetaMethod meta_method() const;
	std::string signature() const;

	bool invoke(const object_with_meta &on, const object_with_meta &parameter) const;

private:
	type _object_type;
	type _parameter_type;
	QMetaMethod _meta_method;

};

bool operator == (const setter_method &x, const setter_method &y);
bool operator != (const setter_method &x, const setter_method &y);

}}

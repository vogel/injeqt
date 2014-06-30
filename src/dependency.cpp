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

#include "dependency.h"

#include <QtCore/QMetaMethod>

namespace injeqt { namespace internal {

dependency::dependency(dependency_type type, const QMetaObject &object, QMetaMethod setter_method) :
	_type{type},
	_object(object),
	_setter_method{std::move(setter_method)}
{
}

dependency_type dependency::type() const
{
	return _type;
}

const QMetaObject & dependency::object() const
{
	return _object;
}

QMetaMethod dependency::setter_method() const
{
	return _setter_method;
}

bool operator == (const dependency &first, const dependency &second)
{
	if (std::addressof(first) == std::addressof(second))
		return true;

	if (first.type() != second.type())
		return false;

	if (std::addressof(first.object()) != std::addressof(second.object()))
		return false;

	if (first.setter_method() != second.setter_method())
		return false;

	return true;
}

bool operator != (const dependency &first, const dependency &second)
{
	return !(first == second);
}

}}

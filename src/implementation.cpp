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

#include "implementation.h"

#include "extract-interfaces.h"

#include <QtCore/QObject>

namespace injeqt { namespace v1 {

implementation::implementation(type interface_type, QObject *object) :
	_interface_type{std::move(interface_type)},
	_object{object}
{
	if (!object)
		throw invalid_implementation_availability_exception{};

	if (object)
	{
		auto implements = extract_interfaces(type{object->metaObject()});
		if (!implements.contains(interface_type))
			throw invalid_interface_type_exception{};
	}
}

type implementation::interface_type() const
{
	return _interface_type;
}

QObject * implementation::object() const
{
	return _object;
}

bool operator == (const implementation &x, const implementation &y)
{
	if (x.interface_type() != y.interface_type())
		return false;

	if (x.object() != y.object())
		return false;

	return true;
}

bool operator != (const implementation &x, const implementation &y)
{
	return !(x == y);
}

}}

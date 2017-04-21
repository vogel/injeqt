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

#include <injeqt/exception/empty-type.h>
#include <injeqt/exception/interface-not-implemented.h>
#include <injeqt/exception/invalid-qobject.h>
#include <injeqt/exception/qobject-type.h>

#include "interfaces-utils.h"

#include <QtCore/QObject>
#include <cassert>

namespace injeqt { namespace internal {

implementation::implementation(type interface_type, QObject *object) :
	_interface_type{std::move(interface_type)},
	_object{object}
{
	assert(!_interface_type.is_empty());
	assert(!_interface_type.is_qobject());
	assert(_object != nullptr);
	assert(_object->metaObject() != nullptr);
	assert(extract_interfaces(type{_object->metaObject()}).contains(_interface_type));
}

const type & implementation::interface_type() const
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

bool operator < (const implementation &x, const implementation &y)
{
	if (x.interface_type() < y.interface_type())
		return true;
	if (x.interface_type() > y.interface_type())
		return false;

	return x.object() < y.object();
}

bool operator > (const implementation &x, const implementation &y)
{
	return y < x;
}

bool operator <= (const implementation &x, const implementation &y)
{
	return !(y < x);
}

bool operator >= (const implementation &x, const implementation &y)
{
	return !(x < y);
}

implementation make_implementation(type interface_type, QObject *object)
{
	if (interface_type.is_empty())
		throw exception::empty_type{};
	if (interface_type.is_qobject())
		throw exception::qobject_type();
	if (!object || !object->metaObject())
		throw exception::invalid_qobject{};
	if (!implements(type{object->metaObject()}, interface_type))
		throw exception::interface_not_implemented{type{object->metaObject()}.name() + ": " + interface_type.name()};

	return implementation{interface_type, object};
}

}}

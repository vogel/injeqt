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

#include "implementation-availability.h"
#include "implements-extractor.h"

#include <QtCore/QObject>

namespace injeqt { namespace v1 {

implementation::implementation(type interface_type, implementation_availability availability, QObject *object) :
	_interface_type{std::move(interface_type)},
	_availability{availability},
	_object{object}
{
	if (availability == implementation_availability::ambiguous && object)
		throw invalid_implementation_availability_exception{};

	if (availability == implementation_availability::available && !object)
		throw invalid_implementation_availability_exception{};

	if (object)
	{
		auto implements = implements_extractor{}.extract_implements(type{object->metaObject()});
		if (implements.find(interface_type) == std::end(implements))
			throw invalid_interface_type_exception{};
	}
}

type implementation::interface_type() const
{
	return _interface_type;
}

implementation_availability implementation::availability() const
{
	return _availability;
}

QObject * implementation::object() const
{
	return _object;
}

bool operator == (const implementation &x, const implementation &y)
{
	if (std::addressof(x) == std::addressof(y))
		return true;

	if (x.interface_type() != y.interface_type())
		return false;

	if (x.availability() != y.availability())
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

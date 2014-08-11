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

namespace injeqt { namespace internal {

dependency::dependency(setter_method setter) :
	_setter{std::move(setter)}
{
}

const type & dependency::required_type() const
{
	return _setter.parameter_type();
}

const setter_method & dependency::setter() const
{
	return _setter;
}

bool operator == (const dependency &x, const dependency &y)
{
	if (x.setter() != y.setter())
		return false;

	return true;
}

bool operator != (const dependency &x, const dependency &y)
{
	return !(x == y);
}

}}

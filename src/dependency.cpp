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

namespace injeqt { namespace v1 {

dependency::dependency(type required_type, dependency_apply_method apply_method, method setter_method) :
	_required_type{std::move(required_type)},
	_apply_method{apply_method},
	_setter_method{std::move(setter_method)}
{
}

type dependency::required_type() const
{
	return _required_type;
}

dependency_apply_method dependency::apply_method() const
{
	return _apply_method;
}

method dependency::setter_method() const
{
	return _setter_method;
}

bool operator == (const dependency &first, const dependency &second)
{
	if (std::addressof(first) == std::addressof(second))
		return true;

	if (first.required_type() != second.required_type())
		return false;

	if (first.apply_method() != second.apply_method())
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

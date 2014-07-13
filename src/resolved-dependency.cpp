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

#include "resolved-dependency.h"

namespace injeqt { namespace v1 {

resolved_dependency::resolved_dependency(dependency resolved, const object_with_meta &object) :
	_resolved{std::move(resolved)},
	_object(object)
{
}

dependency resolved_dependency::resolved() const
{
	return _resolved;
}

const object_with_meta & resolved_dependency::object() const
{
	return _object;
}

bool operator == (const resolved_dependency &first, const resolved_dependency &second)
{
	if (std::addressof(first) == std::addressof(second))
		return true;

	if (first.resolved() != second.resolved())
		return false;

	if (std::addressof(first.object()) != std::addressof(second.object()))
		return false;

	return true;
}

bool operator != (const resolved_dependency &first, const resolved_dependency &second)
{
	return !(first == second);
}

}}

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

#include "implementation-availability.h"

namespace injeqt { namespace v1 {

resolved_dependency::resolved_dependency(implementation resolved_with, setter_method setter) :
	_resolved_with{std::move(resolved_with)},
	_setter{std::move(setter)}
{
	if (_resolved_with.availability() == implementation_availability::ambiguous)
		throw ambiguous_resolved_dependency_exception{};

	if (_resolved_with.implemented_type() != setter.parameter_type())
		throw non_matching_setter_exception{};
}

implementation resolved_dependency::resolved_with() const
{
	return _resolved_with;
}

setter_method resolved_dependency::setter() const
{
	return _setter;
}

bool resolved_dependency::apply_on(QObject *on)
{
	if (!on)
		throw inavalid_apply_on_object_exception{};

	try
	{
		_setter.invoke(on, _resolved_with.object());
	}
	catch (invoked_on_wrong_object_exception &)
	{
		throw inavalid_apply_on_object_exception{};
	}
}

bool operator == (const resolved_dependency &first, const resolved_dependency &second)
{
	if (std::addressof(first) == std::addressof(second))
		return true;

	if (first.resolved_with() != second.resolved_with())
		return false;

	if (first.setter() != second.setter())
		return false;

	return true;
}

bool operator != (const resolved_dependency &first, const resolved_dependency &second)
{
	return !(first == second);
}

}}

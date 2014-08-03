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

#include "instantiation-state.h"

#include "extract-interfaces.h"

namespace injeqt { namespace v1 {

instantiation_state::instantiation_state(type_relations available_types, implementations objects) :
	_available_types{std::move(available_types)},
	_objects{std::move(objects)}
{
	for (auto &&object : _objects)
	{
		auto available_interface_type_it = _available_types.unique().get(object.interface_type());
		if (available_interface_type_it == end(_available_types.unique()))
			throw type_not_in_mapping_exception{};

		auto implementation_type = available_interface_type_it->implementation_type();
		if (implementation_type != object.interface_type())
			throw type_implementation_inconsistent_exception{};
	}
}

const type_relations & instantiation_state::available_types() const
{
	return _available_types;
}

const implementations & instantiation_state::objects() const
{
	return _objects;
}

bool operator == (const instantiation_state &x, const instantiation_state &y)
{
	if (x.available_types() != y.available_types())
		return false;

	if (x.objects() != y.objects())
		return false;

	return true;
}

bool operator != (const instantiation_state &x, const instantiation_state &y)
{
	return !(x == y);
}

}}

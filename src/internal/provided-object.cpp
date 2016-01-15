/*
 * %injeqt copyright begin%
 * Copyright 2016 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "provided-object.h"

#include "provider.h"

#include <cassert>

namespace injeqt { namespace internal {

provided_object::provided_object(provider *provided_by, implementation object) :
	_provided_by{provided_by},
	_object{std::move(object)}
{
	assert(_provided_by);
	assert(!_provided_by->provided_type().is_empty());
	assert(!_provided_by->provided_type().is_qobject());
	assert(_provided_by->provided_type() == object.interface_type());
}

provider * provided_object::provided_by() const
{
	return _provided_by;
}

implementation provided_object::object() const
{
	return _object;
}

}}

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

#include "provider-by-default-constructor.h"

#include <injeqt/exception/instantiation-failed.h>

#include <cassert>

namespace injeqt { namespace internal {

provider_by_default_constructor::provider_by_default_constructor(default_constructor_method constructor) :
	_constructor{std::move(constructor)}
{
	assert(!_constructor.is_empty());
}

provider_by_default_constructor::~provider_by_default_constructor()
{
}

const type & provider_by_default_constructor::provided_type() const
{
	return _constructor.object_type();
}

const default_constructor_method & provider_by_default_constructor::constructor() const
{
	return _constructor;
}

QObject * provider_by_default_constructor::provide(injector_core &)
{
	if (!_object)
	{
		_object = _constructor.invoke();
		if (!_object)
			throw exception::instantiation_failed{provided_type().name()};
	}
	return _object.get();
}

}}

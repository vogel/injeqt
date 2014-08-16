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

namespace injeqt { namespace internal {
// TODO: tests
provider_by_default_constructor::provider_by_default_constructor(default_constructor_method constructor) :
	_constructor{std::move(constructor)}
{
	validate(_constructor);
}

const type & provider_by_default_constructor::created_type() const
{
	return _constructor.object_type();
}

const default_constructor_method & provider_by_default_constructor::constructor() const
{
	return _constructor;
}

QObject * provider_by_default_constructor::create(injector_impl &)
{
	// todo: thread safety
	if (!_object)
		_object = _constructor.invoke();
	return _object.get();
}

void validate(const provider_by_default_constructor &pbdc)
{
	validate(pbdc.constructor());
}

}}

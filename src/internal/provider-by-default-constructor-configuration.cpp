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

#include "provider-by-default-constructor-configuration.h"

#include <injeqt/exception/default-constructor-not-found.h>

#include "default-constructor-method.h"
#include "provider-by-default-constructor.h"

#include <cassert>

namespace injeqt { namespace internal {

provider_by_default_constructor_configuration::provider_by_default_constructor_configuration(type object_type) :
	_object_type{std::move(object_type)}
{
	assert(!_object_type.is_empty());
}

provider_by_default_constructor_configuration::~provider_by_default_constructor_configuration()
{
}

std::vector<type> provider_by_default_constructor_configuration::types() const
{
	return {_object_type};
}

std::unique_ptr<provider> provider_by_default_constructor_configuration::create_provider(const types_by_name &) const
{
	auto c = make_default_constructor_method(_object_type);
	if (c.is_empty())
		throw exception::default_constructor_not_found{_object_type.name()};

	return std::unique_ptr<provider_by_default_constructor>{new provider_by_default_constructor{std::move(c)}};
}

}}

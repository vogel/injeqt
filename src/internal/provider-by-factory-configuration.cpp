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

#include "provider-by-factory-configuration.h"

#include <injeqt/exception/qobject-type.h>
#include <injeqt/exception/unique-factory-method-not-found.h>

#include "factory-method.h"
#include "provider-by-factory.h"

#include <cassert>

namespace injeqt { namespace internal {

provider_by_factory_configuration::provider_by_factory_configuration(type object_type, type factory_type) :
	_object_type{std::move(object_type)},
	_factory_type{std::move(factory_type)}
{
	assert(!_object_type.is_empty());
	assert(!_factory_type.is_empty());
}

provider_by_factory_configuration::~provider_by_factory_configuration()
{
}

std::vector<type> provider_by_factory_configuration::types() const
{
	return {_object_type, _factory_type};
}

std::unique_ptr<provider> provider_by_factory_configuration::create_provider(const types_by_name &) const
{
	if (_object_type.is_qobject())
		throw exception::qobject_type();
	if (_factory_type.is_qobject())
		throw exception::qobject_type();

	auto fm = internal::make_factory_method(_object_type, _factory_type);
	if (fm.is_empty())
		throw exception::unique_factory_method_not_found{_object_type.name() + " in " + _factory_type.name()};

	return std::unique_ptr<provider_by_factory>{new provider_by_factory{std::move(fm)}};
}

}}

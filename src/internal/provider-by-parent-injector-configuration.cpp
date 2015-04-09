/*
 * %injeqt copyright begin%
 * Copyright 2015 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "provider-by-parent-injector-configuration.h"

#include "provider-by-parent-injector.h"

#include <cassert>

namespace injeqt { namespace internal {

provider_by_parent_injector_configuration::provider_by_parent_injector_configuration(injector_impl *parent_injector, type object_type) :
	_parent_injector{parent_injector},
	_object_type{std::move(object_type)}
{
	assert(_parent_injector);
	assert(!_object_type.is_empty());
}

provider_by_parent_injector_configuration::~provider_by_parent_injector_configuration()
{
}

std::vector<type> provider_by_parent_injector_configuration::types() const
{
	return {_object_type};
}

std::unique_ptr<provider> provider_by_parent_injector_configuration::create_provider(const types_by_name &) const
{
	return std::unique_ptr<provider>{new provider_by_parent_injector{_parent_injector, _object_type}};
}

}}

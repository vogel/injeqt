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

#include "provider-by-parent-injector.h"

namespace injeqt { namespace internal {

provider_by_parent_injector::provider_by_parent_injector(injector_impl *parent_injector, type provided_type) :
	_parent_injector{parent_injector},
	_provided_type{std::move(provided_type)}
{
	assert(_parent_injector);
}

const type & provider_by_parent_injector::provided_type() const
{
	return _provided_type;
}

QObject * provider_by_parent_injector::provide(injector_core &)
{
	return _parent_injector->get(_provided_type);
}

bool provider_by_parent_injector::require_resolving() const
{
	return false;
}

}}

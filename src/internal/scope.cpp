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

#include "scope.h"

namespace injeqt { namespace internal {

scope::scope()
{
}

scope::scope(providers available_providers, type_relations available_types) :
	_available_providers{std::move(available_providers)},
	_available_types{std::move(available_types)}
{
}

const providers & scope::available_providers() const
{
	return _available_providers;
}

const type_relations & scope::available_types() const
{
	return _available_types;
}

bool operator == (const scope &x, const scope &y)
{
	if (x.available_providers() != y.available_providers())
		return false;
	if (x.available_types() != y.available_types())
		return false;
	return true;
}

bool operator != (const scope &x, const scope &y)
{
	return !(x == y);
}

scope make_scope(providers available_providers, const std::vector<type> &scope_types)
{
	return scope{std::move(available_providers), make_type_relations(scope_types)};
}

}}

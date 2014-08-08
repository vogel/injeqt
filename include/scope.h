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

#pragma once

#include "providers.h"
#include "instantiation-state.h"
#include "injeqt-exception.h"
#include "injeqt-global.h"

class QObject;

namespace injeqt { namespace v1 {

DEFINE_EXCEPTION(scope_exception, injeqt_exception);
DEFINE_EXCEPTION(type_not_in_scope_exception, scope_exception);
DEFINE_EXCEPTION(unresolved_dependencies_exception, scope_exception);

class INJEQT_API scope final
{

public:
	explicit scope(providers available_providers, type_relations available_types);

	const providers & available_providers() const;
	const instantiation_state & state() const;

	QObject * get(const type &interface_type);

private:
	providers _available_providers;
	instantiation_state _state;

	instantiation_state state_with(instantiation_state old_state, const type &implementation_type);
	instantiation_state state_with(instantiation_state old_state, const types &implementation_types);

};

INJEQT_API bool operator == (const scope &x, const scope &y);
INJEQT_API bool operator != (const scope &x, const scope &y);

INJEQT_API scope make_scope(providers available_providers, const std::vector<type> &scope_types);

}}

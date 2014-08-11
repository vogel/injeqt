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

#include "implementations.h"
#include "injeqt-exception.h"
#include "injeqt.h"
#include "providers.h"
#include "type-relations.h"

class QObject;

using namespace injeqt::v1;

namespace injeqt { namespace internal {

DEFINE_EXCEPTION(scope_exception, injeqt_exception);
DEFINE_EXCEPTION(type_not_in_scope_exception, scope_exception);
DEFINE_EXCEPTION(unresolved_dependencies_exception, scope_exception);

class scope final
{

public:
	scope();
	explicit scope(providers available_providers, type_relations available_types);

	const providers & available_providers() const;
	const type_relations & available_types() const;
	const implementations & objects() const;

	QObject * get(const type &interface_type);

private:
	providers _available_providers;
	type_relations _available_types;
	implementations _objects;

	implementations objects_with(implementations objects, const type &implementation_type);
	implementations objects_with(implementations objects, const types &implementation_types);

};

bool operator == (const scope &x, const scope &y);
bool operator != (const scope &x, const scope &y);

scope make_scope(providers available_providers, const std::vector<type> &scope_types);

}}

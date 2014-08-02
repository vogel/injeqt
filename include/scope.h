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
#include "implemented-by-mapping.h"
#include "injeqt-exception.h"
#include "injeqt-global.h"

#include <memory>

namespace injeqt { namespace v1 {

DEFINE_EXCEPTION(scope_exception, injeqt_exception);
DEFINE_EXCEPTION(type_not_in_scope_exception, scope_exception);
DEFINE_EXCEPTION(unresolved_dependencies_exception, scope_exception);

class INJEQT_API scope final
{

public:
	explicit scope(implemented_by_mapping available_types, implementations objects);

	implemented_by_mapping available_types() const;
	implementations objects() const;

	QObject * get(const type &t);

private:
	implemented_by_mapping _available_types;
	implementations _objects;

	std::vector<std::unique_ptr<QObject>> _owned_objects;

};

INJEQT_API bool operator == (const scope &x, const scope &y);
INJEQT_API bool operator != (const scope &x, const scope &y);

}}

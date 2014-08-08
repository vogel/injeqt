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
#include "injeqt-global.h"
#include "type-relations.h"

namespace injeqt { namespace v1 {

DEFINE_EXCEPTION(invalid_instantiation_state_exception, injeqt_exception);
DEFINE_EXCEPTION(type_not_in_mapping_exception, invalid_instantiation_state_exception);
DEFINE_EXCEPTION(type_implementation_inconsistent_exception, invalid_instantiation_state_exception);

class instantiation_state final
{

public:
	explicit instantiation_state(type_relations available_types, implementations objects);

	const type_relations & available_types() const;
	const implementations & objects() const;

private:
	type_relations _available_types;
	implementations _objects;

};

bool operator == (const instantiation_state &x, const instantiation_state &y);
bool operator != (const instantiation_state &x, const instantiation_state &y);

}}

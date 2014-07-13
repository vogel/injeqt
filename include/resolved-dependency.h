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

#include "dependency.h"
#include "injeqt-global.h"

namespace injeqt { namespace v1 {

class object_with_meta;

class resolved_dependency final
{

public:
	resolved_dependency(dependency resolved, const object_with_meta &object);

	dependency resolved() const;
	const object_with_meta & object() const;

private:
	dependency _resolved;
	const object_with_meta &_object;

};

bool operator == (const resolved_dependency &first, const resolved_dependency &second);
bool operator != (const resolved_dependency &first, const resolved_dependency &second);

}}

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

#include "injeqt.h"
#include "setter-method.h"
#include "type.h"

using namespace injeqt::v1;

namespace injeqt { namespace internal {

class dependency final
{

public:
	explicit dependency(setter_method setter);

	const type & required_type() const;
	const setter_method & setter() const;

private:
	setter_method _setter;

};

void validate(const dependency &d);

bool operator == (const dependency &x, const dependency &y);
bool operator != (const dependency &x, const dependency &y);

}}

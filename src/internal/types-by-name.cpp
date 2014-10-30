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

#include "types-by-name.h"

namespace injeqt { namespace internal {

type type_by_pointer(const types_by_name &known_types, const std::string &pointer_name)
{
	if (pointer_name.length() < 2)
		return type{};
	if (pointer_name[pointer_name.length() - 1] != '*')
		return type{};
	auto name = pointer_name.substr(0, pointer_name.length() - 1);
	auto item = known_types.get(name);
	if (item == std::end(known_types))
		return type{};
	else
		return *item;
}

}}

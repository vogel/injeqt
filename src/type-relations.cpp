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

#include "type-relations.h"

namespace injeqt { namespace v1 {

type_relations::type_relations(types unique, types ambiguous) :
	_unique{std::move(unique)},
	_ambiguous{std::move(ambiguous)}
{
}

types type_relations::unique() const
{
	return _unique;
}

types type_relations::ambiguous() const
{
	return _ambiguous;
}

bool operator == (const type_relations &first, const type_relations &second)
{
	if (std::addressof(first) == std::addressof(second))
		return true;

	if (first.unique() != second.unique())
		return false;

	if (first.ambiguous() != second.ambiguous())
		return false;

	return true;
}

bool operator != (const type_relations &first, const type_relations &second)
{
	return !(first == second);
}

}}

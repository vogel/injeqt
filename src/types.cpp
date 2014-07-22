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

#include "types.h"

namespace injeqt { namespace v1 {

type types::extract_key(const type &i)
{
	return i;
}

types::types(std::vector<type> i) :
	_content{std::move(i)}
{
}

types::const_iterator types::begin() const
{
	return _content.begin();
}

types::const_iterator types::end() const
{
	return _content.end();
}

const types::storage_type & types::content() const
{
	return _content;
}

bool types::empty() const
{
	return _content.empty();
}

bool types::contains(const type &i) const
{
	return _content.contains(i);
}

typename types::storage_type::size_type types::size() const
{
	return _content.size();
}

bool operator == (const types &x, const types &y)
{
	return x.content() == y.content();
}

bool operator != (const types &first, const types &second)
{
	return !(first == second);
}

}}

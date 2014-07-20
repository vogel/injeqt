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

#include "implementations.h"

namespace injeqt { namespace v1 {

type implementations::extract_key(const implementation &i)
{
	return i.implemented_type();
}

implementations::implementations(std::vector<implementation> i) :
	_content{std::move(i)}
{
}

implementations::const_iterator implementations::begin() const
{
	return _content.begin();
}

implementations::const_iterator implementations::end() const
{
	return _content.end();
}

const implementations::storage_type & implementations::content() const
{
	return _content;
}

bool implementations::empty() const
{
	return _content.empty();
}

bool implementations::contains(const implementation &i) const
{
	return _content.contains(i);
}

typename implementations::storage_type::size_type implementations::size() const
{
	return _content.size();
}

}}

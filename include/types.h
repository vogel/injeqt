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

#include "type.h"
#include "injeqt-global.h"
#include "sorted-unique-vector.h"

namespace injeqt { namespace v1 {

class INJEQT_API types final
{
	static type extract_key(const type &i);

public:
	using storage_type = sorted_unique_vector<type, type, extract_key>;
	using const_iterator = typename storage_type::const_iterator;

	explicit types(std::vector<type> t);

	const_iterator begin() const;
	const_iterator end() const;

	const storage_type & content() const;
	bool empty() const;
	bool contains(const type &t) const;
	typename storage_type::size_type size() const;

private:
	storage_type _content;

};

inline typename types::const_iterator begin(const types &i)
{
	return i.begin();
}

inline typename types::const_iterator end(const types &i)
{
	return i.end();
}

INJEQT_API bool operator == (const types &x, const types &y);
INJEQT_API bool operator != (const types &x, const types &y);

}}

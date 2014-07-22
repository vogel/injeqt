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

#include "injeqt-global.h"
#include "sorted-unique-vector.h"
#include <QtWidgets/qgraphicsitem.h>

namespace injeqt { namespace v1 {

class dependency;
class type;

class INJEQT_API dependencies final
{
	static type extract_key(const dependency &dep);

public:
	using storage_type = sorted_unique_vector<type, dependency, extract_key>;
	using const_iterator = typename storage_type::const_iterator;

	explicit dependencies(std::vector<dependency> dependencies);
	explicit dependencies(storage_type dependencies);

	const_iterator begin() const;
	const_iterator end() const;

	const storage_type & content() const;
	bool empty() const;
	bool contains(const dependency &d) const;
	typename storage_type::size_type size() const;

private:
	storage_type _content;

};

inline typename dependencies::const_iterator begin(const dependencies &d)
{
	return d.begin();
}

inline typename dependencies::const_iterator end(const dependencies &d)
{
	return d.end();
}

INJEQT_API bool operator == (const dependency &x, const dependency &y);
INJEQT_API bool operator != (const dependency &x, const dependency &y);

}}

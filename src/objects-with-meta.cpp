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

#include "objects-with-meta.h"

#include "object-with-meta.h"

namespace injeqt { namespace v1 {

bool objects_with_meta::less_than_object_with_meta(const object_with_meta * const &left, const object_with_meta * const &right)
{
	return std::addressof(left->meta().type()) < std::addressof(right->meta().type());
}

bool objects_with_meta::equal_object_with_meta(const object_with_meta * const &left, const object_with_meta * const &right)
{
	return std::addressof(left->meta().type()) == std::addressof(right->meta().type());
}

objects_with_meta::objects_with_meta(std::vector<const object_with_meta *> objects_with_meta) :
	_content{std::move(objects_with_meta)}
{
}

objects_with_meta::const_iterator objects_with_meta::begin() const
{
	return _content.begin();
}

objects_with_meta::const_iterator objects_with_meta::end() const
{
	return _content.end();
}

bool objects_with_meta::empty() const
{
	return _content.empty();
}

bool objects_with_meta::contains(const object_with_meta *d) const
{
	auto lower_bound = std::lower_bound(begin(), end(), d, less_than_object_with_meta);
	if (lower_bound == end())
		return false;

	return *lower_bound == d;
}

typename objects_with_meta::storage_type::size_type objects_with_meta::size() const
{
	return _content.size();
}

}}

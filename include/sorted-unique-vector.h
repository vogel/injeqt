/*
 * %injeqt copyright begin%
 * Copyright 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * %injeqt copyright end%
 *
 * type library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * type library is distributed in the hope that it will be useful,
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

#include <algorithm>
#include <functional>
#include <vector>

namespace injeqt { namespace v1 {

/**
 * @addtogroup Misc
 * @{
 */

/**
 * @class sorted_unique_vector
 * @short Vector that stored only unique values thata are always sorted.
 * @param T type of data
 * @param LessThanComparator comparator used for sorting
 * @param EqualityComparator comparator used for uniqueness testing
 */
template<typename T, bool (*LessThanComparator)(const T &, const T &), bool (*EqualityComparator)(const T &, const T &)>
class sorted_unique_vector
{

public:
	using type = sorted_unique_vector<T, LessThanComparator, EqualityComparator>;
	using storage_type = std::vector<T>;
	using size_type = typename storage_type::size_type;

	/**
	 * @short Create empty sorted_unique_vector.
	 */
	sorted_unique_vector() {}

	/**
	 * @short Create sorted_unique_vector from given vector.
	 * @param storage vector to get data from
	 *
	 * Copies content of storage, sorts it and removes duplicates.
	 */
	sorted_unique_vector(storage_type storage) :
			_content{std::move(storage)}
	{
		std::stable_sort(std::begin(_content), std::end(_content), LessThanComparator);
		ensure_unique(_content);
	}

	/**
	 * @short Add new item to sorted vector.
	 * @param item new item
	 *
	 * Item is added at proper place so vector remains sorted. Item will not be
	 * added if another one that compares equal (using EqualityComparator)
	 * already exists.
	 */
	void add(T item)
	{
		if (_content.empty())
		{
			_content.emplace_back(std::move(item));
			return;
		}

		auto upperBound = std::upper_bound(std::begin(_content), std::end(_content), item, LessThanComparator);
		if (upperBound == std::begin(_content) || !EqualityComparator(*(upperBound - 1), item))
			_content.emplace(upperBound, std::move(item));
	}

	/**
	 * @short Merge with another sorted vector.
	 * @param sorted_vector vector to merge with
	 *
	 * All items from sorted_vector are added at proper places and duplicates are removed.
	 */
	void merge(const type &sorted_vector)
	{
		auto result = storage_type{};

		std::merge(std::begin(_content), std::end(_content),
			std::begin(sorted_vector._content), std::end(sorted_vector._content),
			std::back_inserter(result), LessThanComparator);
		ensure_unique(result);

		_content = std::move(result);
	}

	/**
	 * @return Data stored in sorted vector.
	 */
	const storage_type & content() const
	{
		return _content;
	}

	/**
	 * @return true if no data is stored
	 */
	bool empty() const
	{
		return _content.empty();
	}

	/**
	 * @return number of stored items
	 */
	typename storage_type::size_type size() const
	{
		return _content.size();
	}

	/**
	 * @short Removes all items from sorted vector.
	 */
	void clear()
	{
		_content.clear();
	}

private:
	storage_type _content;

	void ensure_unique(storage_type &storage)
	{
		storage.erase(std::unique(std::begin(storage), std::end(storage), EqualityComparator), std::end(storage));
	}

};

/**
 * @return begin iterator to content of sorted_unique_vector.
 */
template<typename T, bool (*LessThanComparator)(const T &, const T &), bool (*EqualityComparator)(const T &, const T &)>
typename sorted_unique_vector<T, LessThanComparator, EqualityComparator>::storage_type::const_iterator begin(const sorted_unique_vector<T, LessThanComparator, EqualityComparator> &sorted_vector)
{
	return std::begin(sorted_vector.content());
}

/**
 * @return end iterator to content of sorted_unique_vector.
 */
template<typename T, bool (*LessThanComparator)(const T &, const T &), bool (*EqualityComparator)(const T &, const T &)>
typename sorted_unique_vector<T, LessThanComparator, EqualityComparator>::storage_type::const_iterator end(const sorted_unique_vector<T, LessThanComparator, EqualityComparator> &sorted_vector)
{
	return std::end(sorted_vector.content());
}

/**
 * @}
 */

}}

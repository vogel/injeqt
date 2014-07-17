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
template<typename K, typename V, K (*KeyExtractor)(const V &)>
class sorted_unique_vector
{

public:
	using type = sorted_unique_vector<K, V, KeyExtractor>;
	using value_type = V;
	using key_type = K;
	using storage_type = std::vector<value_type>;
	using const_iterator = typename storage_type::const_iterator;
	using size_type = typename storage_type::size_type;

private:
	static bool compare_keys(const value_type &v1, const value_type &v2)
	{
		auto k1 = KeyExtractor(v1);
		auto k2 = KeyExtractor(v2);
		return k1 < k2;
	}

	static bool keys_equal(const value_type &v1, const value_type &v2)
	{
		auto k1 = KeyExtractor(v1);
		auto k2 = KeyExtractor(v2);
		return k1 == k2;
	}

public:
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
		std::stable_sort(std::begin(_content), std::end(_content), compare_keys);
		ensure_unique(_content);
	}

	const_iterator begin() const
	{
		return std::begin(_content);
	}

	const_iterator end() const
	{
		return std::end(_content);
	}

	/**
	 * @short Add new item to sorted vector.
	 * @param item new item
	 *
	 * Item is added at proper place so vector remains sorted. Item will not be
	 * added if another one that compares equal (using EqualityComparator)
	 * already exists.
	 */
	void add(value_type item)
	{
		if (_content.empty())
		{
			_content.emplace_back(std::move(item));
			return;
		}

		auto upperBound = std::upper_bound(std::begin(_content), std::end(_content), item, compare_keys);
		if (upperBound == std::begin(_content) || !keys_equal(*(upperBound - 1), item))
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
			std::back_inserter(result), compare_keys);
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
	 * @return true if element with value v is found
	 */
	bool contains(const value_type &v) const
	{
		auto lower_bound = std::lower_bound(begin(), end(), v, compare_keys);
		if (lower_bound == end())
			return false;

		return *lower_bound == v;
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
		storage.erase(std::unique(std::begin(storage), std::end(storage), keys_equal), std::end(storage));
	}

};

/**
 * @return begin iterator to content of sorted_unique_vector.
 */
template<typename K, typename V, K (*KeyExtractor)(const V &)>
typename sorted_unique_vector<K, V, KeyExtractor>::const_iterator begin(const sorted_unique_vector<K, V, KeyExtractor> &sorted_vector)
{
	return std::begin(sorted_vector.content());
}

/**
 * @return end iterator to content of sorted_unique_vector.
 */
template<typename K, typename V, K (*KeyExtractor)(const V &)>
typename sorted_unique_vector<K, V, KeyExtractor>::const_iterator end(const sorted_unique_vector<K, V, KeyExtractor> &sorted_vector)
{
	return std::end(sorted_vector.content());
}

template<typename K, typename V1, typename V2, K (*KeyExtractor1)(const V1 &), K (*KeyExtractor2)(const V2 &)>
struct match_result
{
	std::vector<std::pair<V1, V2>> matched;
	sorted_unique_vector<K, V1, KeyExtractor1> unmatched_1;
	sorted_unique_vector<K, V2, KeyExtractor2> unmatched_2;
};

template<typename K, typename V1, typename V2, K (*KeyExtractor1)(const V1 &), K (*KeyExtractor2)(const V2 &)>
match_result<K, V1, V2, KeyExtractor1, KeyExtractor2>
match(const sorted_unique_vector<K, V1, KeyExtractor1> &suv_1, const sorted_unique_vector<K, V2, KeyExtractor2> &suv_2)
{
	auto unmatched_1 = std::vector<V1>{};
	auto unmatched_2 = std::vector<V2>{};
	auto matched = std::vector<std::pair<V1, V2>>{};

	auto suv_1_it = begin(suv_1);
	auto suv_1_end = end(suv_1);
	auto suv_2_it = begin(suv_2);
	auto suv_2_end = end(suv_2);

	while (suv_1_it != suv_1_end && suv_2_it != suv_2_end)
	{
		auto suv_1_key = KeyExtractor1(*suv_1_it);
		auto suv_2_key = KeyExtractor2(*suv_2_it);
		if (suv_1_key == suv_2_key)
		{
			matched.emplace_back(*suv_1_it, *suv_2_it);
			++suv_1_it;
			++suv_2_it;
		}
		else if (suv_1_key < suv_2_key)
		{
			unmatched_1.emplace_back(*suv_1_it);
			++suv_1_it;
		}
		else if (suv_2_key < suv_1_key)
		{
			unmatched_2.emplace_back(*suv_2_it);
			++suv_2_it;
		}
	}

	while (suv_1_it != suv_1_end)
	{
		unmatched_1.emplace_back(*suv_1_it);
		suv_1_it++;
	}

	while (suv_2_it != suv_2_end)
	{
		unmatched_2.emplace_back(*suv_2_it);
		suv_2_it++;
	}

	return {matched, unmatched_1, unmatched_2};
}

/**
 * @}
 */

}}

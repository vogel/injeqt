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

#include "internal.h"

#include <algorithm>
#include <vector>

/**
 * @file
 * @brief Helper functions for containers.
 */

namespace injeqt { namespace internal {

/**
 * @brief Functional interface to std::transform.
 * @tparam S type of source items
 * @tparam T type of result items
 * @param source source data
 * @param f transforming function
 */
template<typename S, typename T>
inline std::vector<T> transform(const std::vector<S> &source, std::function<T(S)> f)
{
	auto result = std::vector<T>{};
	std::transform(std::begin(source), std::end(source), std::back_inserter(result), f);
	return result;
}

/**
 * @brief Extract data from multiple vectors into one vector.
 * @tparam S type of source items
 * @tparam T type of result items
 * @param f function returning vector for each item in source
 */
template<typename S, typename T>
inline std::vector<T> extract(const std::vector<S> &sources, std::function<std::vector<T>(const S &)> f)
{
	auto result = std::vector<T>{};
	for (auto &&source : sources)
	{
		auto v = f(source);
		std::copy(std::begin(v), std::end(v), std::back_inserter(result));
	}
	return result;
}

}}

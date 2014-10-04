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

#include "interfaces-utils.h"

#include <map>

namespace injeqt { namespace internal {

type_relations::type_relations()
{
}

type_relations::type_relations(implemented_by_mapping unique, types ambiguous) :
	_unique{std::move(unique)},
	_ambiguous{std::move(ambiguous)}
{
}

const implemented_by_mapping & type_relations::unique() const
{
	return _unique;
}

const types & type_relations::ambiguous() const
{
	return _ambiguous;
}

bool operator == (const type_relations &x, const type_relations &y)
{
	if (x.unique() != x.unique())
		return false;

	if (x.ambiguous() != y.ambiguous())
		return false;

	return true;
}

bool operator != (const type_relations &x, const type_relations &y)
{
	return !(x == y);
}

type_relations make_type_relations(const std::vector<type> &main_types)
{
	auto type_count = std::map<type, std::size_t>{};
	auto implemented_by_type = std::map<type, type>{};

	for (auto &&main_type : main_types)
	{
		auto interface_types = extract_interfaces(main_type);
		for (auto &&interface_type : interface_types)
		{
			type_count[interface_type]++;
			implemented_by_type.insert({interface_type, main_type});
		}
	}

	auto unique = std::vector<implemented_by>{};
	auto ambiguous = std::vector<type>{};
	for (auto &&counted_type : type_count)
		if (counted_type.second == 1)
			unique.push_back(implemented_by{counted_type.first, implemented_by_type.at(counted_type.first)});
		else if (counted_type.second > 1)
			ambiguous.push_back(counted_type.first);

	return type_relations{implemented_by_mapping{unique}, types{ambiguous}};
}

}}

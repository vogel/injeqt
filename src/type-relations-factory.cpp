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

#include "type-relations-factory.h"

#include "implements-extractor.h"
#include "type-relations.h"

#include <map>

namespace injeqt { namespace v1 {

type_relations type_relations_factory::create_type_relations(const std::vector<type> &main_types)
{
	auto type_count = std::map<type, std::size_t>();
	for (auto &&main_type : main_types)
	{
		auto implemented_types = implements_extractor{}.extract_implements(main_type);
		for (auto &&implemented_type : implemented_types)
			type_count[implemented_type]++;
	}

	auto unique = std::vector<type>{};
	auto ambiguous = std::vector<type>{};
	for (auto &&counted_type : type_count)
		if (counted_type.second == 1)
			unique.push_back(counted_type.first);
		else if (counted_type.second > 1)
			ambiguous.push_back(counted_type.first);

	return type_relations{types{unique}, types{ambiguous}};
}

}}

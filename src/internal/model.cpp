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

#include "model.h"

#include "type-relations.h"

namespace injeqt { namespace internal {

model::model()
{
}

model::model(const std::vector<type> &all_types)
{
	auto relations = make_type_relations(all_types);

	for (auto &&t : all_types)
		if (relations.ambiguous().contains(t))
			throw ambiguous_type_exception{t.name()};

	auto all_dependencies = std::vector<type_dependencies>{};
	std::transform(std::begin(relations.unique()), std::end(relations.unique()), std::back_inserter(all_dependencies),
		[](const implemented_by &ib){ return type_dependencies{ib.implementation_type()}; });

	_available_types = relations.unique();
	_mapped_dependencies = types_dependencies{all_dependencies};
}

const implemented_by_mapping & model::available_types() const
{
	return _available_types;
}

const types_dependencies & model::mapped_dependencies() const
{
	return _mapped_dependencies;
}

}}

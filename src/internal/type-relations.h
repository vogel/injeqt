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

#include <injeqt/injeqt.h>

#include "implemented-by-mapping.h"
#include "types.h"

/**
 * @file
 * @brief Contains classes and functions for defining relations between Injeqt types.
 */

namespace injeqt { namespace internal {

/**
 * @brief Defines relations between Injeqt types.
 * @see make_type_relations(const std::vector<type> &) to learn about alghoritm
 *
 * This class is used in model to compute relations between all injeqt types and in
 * make_validated_dependencies(const type &) to check if there is ambiguity between dependencies
 * of a type. If a type configured in module lands in ambiguous() set then whole injector
 * conifguration is considered invalid.
 *
 * To create object of this type use make_type_relations(const std::vector<type> &) to ensure that
 * it is valid.
 */
class type_relations final
{

public:
	/**
	 * @brief Create empty type_relations object.
	 */
	type_relations();

	/**
	 * @brief Create new type_relations object.
	 * @param unique unique mappings
	 * @param ambiguous set of ambiguous types
	 *
	 * This constructor does not check for validity of data. Use make_type_relations(const std::vector<type> &)
	 * factory function to ensure that data is valid.
	 */
	explicit type_relations(implemented_by_mapping unique, types ambiguous);

	/**
	 * @return mappings of unique types.
	 */
	const implemented_by_mapping & unique() const;

	/**
	 * @return set of ambiguous types.
	 */
	const types & ambiguous() const;

private:
	implemented_by_mapping _unique;
	types _ambiguous;

};

bool operator == (const type_relations &x, const type_relations &y);
bool operator != (const type_relations &x, const type_relations &y);

/**
 * @brief Create valid type_relations from list of types.
 * @param main_types list of types to create relations from
 *
 * Each type T from main_types has set of base types (that can be empty) - it consists of all
 * base types between T (including) and QObject (excluding). All types that are unique in set
 * are added to unique() set of result object as implemented_by object with interface_type set
 * to this base type and implementation_type set to T. All non unique types are added to
 * ambiguous() set of result object.
 */
type_relations make_type_relations(const std::vector<type> &main_types);

}}

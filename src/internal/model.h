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

#include "implemented-by-mapping.h"
#include "injeqt.h"
#include "types-dependencies.h"
#include "type.h"

using namespace injeqt::v1;

namespace injeqt { namespace internal {

DEFINE_EXCEPTION(model_exception, injeqt_exception);
DEFINE_EXCEPTION(ambiguous_type_exception, model_exception);

class model
{

public:
	model();
	explicit model(const std::vector<type> &all_types);

	const implemented_by_mapping & available_types() const;
	const types_dependencies & mapped_dependencies() const;

private:
	implemented_by_mapping _available_types;
	types_dependencies _mapped_dependencies;

};

}}

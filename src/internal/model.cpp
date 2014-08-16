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

namespace injeqt { namespace internal {

model::model()
{
}

model::model(implemented_by_mapping available_types, types_dependencies mapped_dependencies) :
	_available_types{std::move(available_types)},
	_mapped_dependencies{std::move(mapped_dependencies)}
{
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

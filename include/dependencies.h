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

#include "dependency.h"
#include "injeqt-global.h"
#include "sorted-unique-vector.h"
#include "type.h"

namespace injeqt { namespace v1 {

inline type type_from_dependency(const dependency &d)
{
	return d.required_type();
}

using dependencies = sorted_unique_vector<type, dependency, type_from_dependency>;

DEFINE_EXCEPTION(dependency_exception, injeqt_exception);
DEFINE_EXCEPTION(dependency_too_many_parameters_exception, dependency_exception);
DEFINE_EXCEPTION(dependency_not_qobject_exception, dependency_exception);
DEFINE_EXCEPTION(dependency_duplicated_exception, dependency_exception);
DEFINE_EXCEPTION(dependency_on_self_exception, dependency_exception);
DEFINE_EXCEPTION(dependency_on_subtype_exception, dependency_exception);
DEFINE_EXCEPTION(dependency_on_supertype_exception, dependency_exception);

dependencies make_dependencies(const type &for_type);

}}

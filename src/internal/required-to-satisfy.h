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

#include "implementations.h"
#include "internal.h"
#include "types-model.h"
#include "types.h"

/**
 * @file
 * @brief Contains functions for computing list of types required to properly satisfy provided dependnecies.
 */

namespace injeqt { namespace internal {

/**
 * @brief Return list of types required to properly satisfy provided dependnecies.
 * @param dependencies_to_satisfy list of dependencies to satisfy
 * @param model model of all types in system, must be valid
 * @param objects list of available interfaces, must be valid
 * @pre model.get_unresolvable_dependencies().empty()
 *
 * This function computes list of all types that must be instantiated in order to properly resolve all
 * provided dependencies. It means it recursively traverses dependency tree and returns all nodes that
 * are not found in @p objects set.
 */
INJEQT_INTERNAL_API types required_to_satisfy(const dependencies &dependencies_to_satisfy, const types_model &model, const implementations &objects);

}}

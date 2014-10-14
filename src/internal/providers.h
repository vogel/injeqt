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
#include <injeqt/type.h>

#include "provider.h"
#include "sorted-unique-vector.h"

#include <memory>

/**
 * @file
 * @brief Contains classes and functions for representing set of Injeqt providers sorted by provider::provided_type().
 */

namespace injeqt { namespace internal {

/**
 * @brief Extract provided_type from pointer to provider for storting purposes.
 */
inline type type_from_provider(const std::unique_ptr<provider> &c)
{
	return c->provided_type();
}

/**
 * @brief Abstraction of Injeqt set of providers.
 *
 * As provider is an abstract class it stores set of pointers to it.
 *
 * This set is used to represent all providers available to module and injector. It is
 * not possible to store two providers with the same provider::provided_type() values in it.
 * Set is sorted by provider::provided_type().
 */
using providers = sorted_unique_vector<type, std::unique_ptr<provider>, type_from_provider>;

}}

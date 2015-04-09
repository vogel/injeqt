/*
 * %injeqt copyright begin%
 * Copyright 2015 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "provider-configuration.h"

#include "injector-impl.h"

/**
 * @file
 * @brief Contains classes and functions for representing configuration of provider that uses different injector.
 */

namespace injeqt { namespace internal {

/**
 * @brief Configuration of provider that uses different injector.
 *
 * This provider configuration object will return provider implementation that will
 * use different (parent) injector to provide objects.
 */
class provider_by_parent_injector_configuration : public provider_configuration
{

public:
	/**
	 * @brief Create provider configuration instance.
	 * @param parent_injector injector used to get object
	 * @param object_type type to provide
	 * @pre parent_injector != nullptr
	 * @pre !object_type.is_empty()
	 */
	explicit provider_by_parent_injector_configuration(injector_impl *parent_injector, type object_type);
	virtual ~provider_by_parent_injector_configuration();

	/**
	 * @return list consisting of object_type param passed to constructor
	 */
	virtual std::vector<type> types() const override;

	/**
	 * @param known_types list of all types known to injector, used to check return types of methods
	 * @return pointer to new @see provider_by_parent_injector_ object
	 */
	virtual std::unique_ptr<provider> create_provider(const types_by_name &known_types) const override;

private:
	injector_impl *_parent_injector;
	type _object_type;

};

}}

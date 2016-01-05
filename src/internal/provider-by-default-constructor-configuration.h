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

#include "internal.h"
#include "provider-configuration.h"

/**
 * @file
 * @brief Contains classes and functions for representing configuration of provider working on default constructor.
 */

namespace injeqt { namespace internal {

/**
 * @brief Configuration of provider that returns default-constructed object.
 *
 * This provider configuration object will return provider implementation that will
 * use default constructor to create objects.
 */
class INJEQT_INTERNAL_API provider_by_default_constructor_configuration : public provider_configuration
{

public:
	/**
	 * @brief Create provider configuration instance.
	 * @param object_type type of object that this provider will return
	 * @pre !object_type.is_empty()
	 * 
	 * This constructor does not throw even when @p object_type is invalid or does not have deafult
	 * contructor. Factory method create_provider(const types_by_name &) will throw in that case.
	 */
	explicit provider_by_default_constructor_configuration(type object_type);
	virtual ~provider_by_default_constructor_configuration();

	/**
	 * @return list consisting of object_type param passed to constructor
	 */
	virtual std::vector<type> types() const override;

	/**
	 * @param known_types list of all types known to injector, not used
	 * @return pointer to new @see provider_by_default_constructor object
	 * @throw exception::qobject_type if object_type passed to constructor was QObject
	 * @throw exception::default_constructor_not_found if object_type passed does not have default constructor
	 */
	virtual std::unique_ptr<provider> create_provider(const types_by_name &known_types) const override;

private:
	type _object_type;

};

}}

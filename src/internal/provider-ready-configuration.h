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

#include "provider-configuration.h"

/**
 * @file
 * @brief Contains classes and functions for representing configuration of provider working on ready object.
 */

namespace injeqt { namespace internal {

/**
 * @brief Configuration of provider that always returns the same object.
 *
 * This provider configuration object will return provider implementation that will always return the same object.
 */
class provider_ready_configuration : public provider_configuration
{

public:
	/**
	 * @brief Create provider configuration instance.
	 * @param object_type type of object that this provider will always return
	 * @param object object that this provider will always return
	 * @pre !object_type.is_empty()
	 * 
	 * This constructor does not throw even when @p object_type and @p object combination is invalid.
	 * Factory method create_provider(const types_by_name &) will throw in that case.
	 */
	explicit provider_ready_configuration(type object_type, QObject *object);
	virtual ~provider_ready_configuration();

	/**
	 * @return list consisting of object_type param passed to constructor
	 */
	virtual std::vector<type> types() const override;

	/**
	 * @param known_types list of all types known to injector, not used
	 * @return pointer to new @see provider_ready object
	 * @throw exception::empty_type if object_type passed to constructor was empty
	 * @throw exception::qobject_type if object_type passed to constructor was QObject
	 * @throw exception::invalid_qobject if object passed to constructor was nullptr
	 * @throw exception::invalid_qobject if object passed to constructor was nullptr does not have a valid metaObject()
	 * @throw exception::interface_not_implemented if object passed to constructor does not implement object_type interface
	 */
	virtual std::unique_ptr<provider> create_provider(const types_by_name &known_types) const override;

private:
	type _object_type;
	QObject *_object;

};

}}

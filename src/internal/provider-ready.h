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

#include "provider.h"
#include "implementation.h"

/**
 * @file
 * @brief Contains classes and functions for representing provider working on ready object.
 */

namespace injeqt { namespace internal {

/**
 * @brief Provider that always returns the same object.
 *
 * This provider implementation will always return the same object. Its provided_type()
 * returns type of object passed to it in constructor. Its required_types() returns
 * empty set of types.
 */
class provider_ready final : public provider
{

public:
	/**
	 * @brief Create provider instance with object to return.
	 * @param ready_implementation object that this provider will always return
	 */
	explicit provider_ready(implementation ready_implementation);
	virtual ~provider_ready() {}

	/**
	 * @return implementation::interface_type() of object passed to construtor
	 */
	virtual const type & provided_type() const override;

	/**
	 * @return object passed in constructor
	 * @post result != nullptr
	 * @post implements(type{result->metaObject()}, provided_type())
	 */
	virtual QObject * provide(injector_impl &i) override;

	/**
	 * @return empty set of object - this provider already has instance of a type
	 */
	virtual types required_types() const override { return types{}; }

	/**
	 * @return implementation object passed in constructor
	 */
	const implementation & ready_implementation() const;

private:
	implementation _ready_implementation;

};

}}

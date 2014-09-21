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

#include "provider.h"
#include "default-constructor-method.h"
#include "injeqt.h"

/**
 * @file
 * @brief Contains classes and functions for representing provider working on default constructor.
 */

using namespace injeqt::v1;

namespace injeqt { namespace internal {

/**
 * @brief Provider that returns default-constructed object.
 *
 * This provider implementation will return object using default constructor of some type.
 * Its provided_type() returns type of object that contains passed constructor. Its required_types()
 * returns empty set of types no other objects are required for construction.
 *
 * Once created, object will be stored inside and return on subsequents calls to provide(injector_impl &).
 * This provider has ownershipd over created object and will destroy it at own destruction.
 */
class provider_by_default_constructor final : public provider
{

public:
	/**
	 * @brief Create provider instance with default constructor to call.
	 * @param constructor constructor method used to create object
	 */
	explicit provider_by_default_constructor(default_constructor_method constructor);
	virtual ~provider_by_default_constructor() {}

	/**
	 * @return default_constructor_method::object_type() of object passed to construtor
	 */
	virtual const type & provided_type() const override;

	/**
	 * @return object created by default constructor
	 *
	 * If object was not yet created the constructor() method is called and object is stored
	 * in internal cache. Then object from cache is returned.
	 */
	virtual QObject * provide(injector_impl &i) override;

	/**
	 * @return empty set of object - this provider does not require another object to instantiate
	 */
	virtual types required_types() const { return types{}; }

	/**
	 * @return constructor object passed in constructor
	 */
	const default_constructor_method & constructor() const;

private:
	default_constructor_method _constructor;
	std::unique_ptr<QObject> _object;

};

/**
 * @brief Throws an exception if provider_by_default_constructor pbdc is not valid.
 * @param pbdc provider_by_default_constructor to validate
 * @throws invalid_default_constructor_exception if backing QMetaMethod is not a constructor
 * @throws invalid_default_constructor_exception if backing QMetaMethod contains paramers
 * @throws invalid_type_exception if backing QMetaMethod comes from invalid type
 *
 * Call to validate provider_by_default_constructor pbdc. If backing QMetaMethod of backing
 * provider_by_default_constructor is invalid, is not a constructor or requires paramters -
 * an exception is thrown.
 */
void validate(const provider_by_default_constructor &pbdc);

}}

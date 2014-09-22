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

#include "factory-method.h"
#include "provider.h"
#include "injeqt.h"

/**
 * @file
 * @brief Contains classes and functions for representing provider working on factory method.
 */

namespace injeqt { namespace internal {

/**
 * @brief Provider that returns object created by factory.
 *
 * This provider implementation will return object using factory method. Its provided_type() returns
 * type of factory method return type and its required_types() returns type of object of factory method.
 * On first call of provide(injector_impl &) it will ask injector_impl for object of type
 * factory_method::object_method() to be able to invoke this method on that object.
 *
 * Once created, object will be stored inside and return on subsequents calls to provide(injector_impl &).
 * This provider has ownershipd over created object and will destroy it at own destruction.
 */
class provider_by_factory final : public provider
{

public:
	/**
	 * @brief Create provider instance with factory method to call.
	 * @param factory factory method used to create object
	 */
	explicit provider_by_factory(factory_method factory);
	virtual ~provider_by_factory();

	/**
	 * @return factory_method::retrun_type() of object passed to construtor
	 */
	virtual const type & provided_type() const override;

	/**
	 * @return object created by factory method
	 *
	 * If object was not yet created the object of type factory_method::object_type() is requested
	 * from @p i and the factory method is called on it to get object and stoe it in internal cache,
	 * Then object from cache is returned.
	 */
	virtual QObject * provide(injector_impl &i) override;

	/**
	 * @return factory_method::object_type() from object passed to constructor
	 *
	 * Factory object must be present before factory method can be invoked.
	 */
	virtual types required_types() const;

	/**
	 * @return factory method object passed in constructor
	 */
	const factory_method & factory() const;

private:
	factory_method _factory;
	std::unique_ptr<QObject> _object;

};

/**
 * @brief Throws an exception if provider_by_factory pbf is not valid.
 * @param pbf provider_by_factory to validate
 * @throws invalid_factory_method_exception if backing QMetaMethod is not a method or slot
 * @throws invalid_factory_method_exception if backing QMetaMethod contains paramers
 * @throws invalid_factory_method_exception if backing QMetaMethod comes from invalid type
 * @throws invalid_factory_method_exception if backing QMetaMethod does not return valid type
 *
 * Call to validate provider_by_factory pbf. If backing QMetaMethod of backin factory_method
 * is not valid factory method an exception will be thrown.
 */
void validate(const provider_by_factory &pbf);

}}

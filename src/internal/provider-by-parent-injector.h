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

#include "injector-impl.h"
#include "provider.h"

/**
 * @file
 * @brief Contains classes and functions for representing provider that uses different injector.
 */

namespace injeqt { namespace internal {

/**
 * @brief Provider that returns object from other (parent) injector.
 *
 * This provider implementation will return object from different (parent) injector.
 * It is basically a proxy. It can be used to implement injectors inside plugins that
 * use main application injector for getting global services. It can be also used to manager
 * scopes of object - by creating separate subinjectors for each context.
 */
class provider_by_parent_injector final : public provider
{

public:
	/**
	 * @param parent_injector injector used to get object
	 * @param provided_type type to provide
	 * @pre parent_injector != nullptr
	 */
	explicit provider_by_parent_injector(injector_impl *parent_injector, type provided_type);
	virtual ~provider_by_parent_injector() {}

	/**
	 * @return provided_type value passed to constructor
	 */
	virtual const type & provided_type() const override;

	/**
	 * @return object returned by parent_injector
	 * @post result != nullptr
	 * @post implements(type{result->metaObject()}, provided_type())
	 * @throw instantiation_failed if instantiation of provided type failed
	 *
	 * If object was not yet created the constructor() method is called and object is stored
	 * in internal cache. Then object from cache is returned.
	 */
	virtual QObject * provide(injector_core &i) override;

	/**
	 * @return empty set of object - this provider does not require another object to instantiate
	 */
	virtual types required_types() const override { return types{}; }

	/**
	 * @return false
	 *
	 * Parent injector takes care of it.
	 */
	virtual bool require_resolving() const override;

private:
	injector_impl *_parent_injector;
	type _provided_type;

};

}}

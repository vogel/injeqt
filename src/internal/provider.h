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

#include "types.h"

/**
 * @file
 * @brief Contains classes and functions for representing providers of object.
 */

class QObject;

namespace injeqt { namespace internal {

class injector_core;

/**
 * @brief Abstract provider of objects.
 *
 * Providers are used in injector to create objects of configured types. Objects
 * can be created in different ways so this is an abstract class with a few
 * mplementations.
 *
 * Provider can require objects of other types to be already available in injector
 * using required_types() and then provide an object with its provide() object.
 * Provider may or may no take ownership of a provided object - look for concrete
 * types for more information.
 * 
 * Provider return objects that may or may not require dependency resolving.
 * It can be checked with require_resolving() method.
 */
class provider
{

public:
	explicit provider() {}
	virtual ~provider() {}

	/**
	 * @return type of object that this provider provides.
	 */
	virtual const type & provided_type() const = 0;

	/**
	 * @return provided object
	 * @param i injector_core that requests object
	 * @post result != nullptr
	 * @post implements(type{result->metaObject()}, provided_type())
	 * @throw instantiation_failed if instantiation of provided type failed
	 *
	 * Provider can require some types with required_types() to be already available
	 * in injector_core. Provider may or may no take ownership of a provided object -
	 * look for concrete types for more information.
	 */
	virtual QObject * provide(injector_core &i) = 0;

	/**
	 * @return list of types that must be instantiated in injector_core before calling provide(injector_core) method
	 * 
	 * Return value of this method must be the same for whole lifetime of this object.
	 */
	virtual types required_types() const = 0;

	/**
	 * @return true, if provided object require resolving of dependencies
	 */
	virtual bool require_resolving() const = 0;

};

}}

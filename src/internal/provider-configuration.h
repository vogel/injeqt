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

#include "types-by-name.h"

#include <memory>

/**
 * @file
 * @brief Contains classes and functions for representing configuration of object's provider.
 */

namespace injeqt { namespace internal {

class provider;

/**
 * @brief Abstract configuration of object's provider
 * @see provider
 *
 * This object is used to create @see provider objects. Configurations are stored in @see module
 * and @see module_impl, providers are stored in @see injector_core objects.
 * 
 * Provider configuration contains only two methods: types() and create_provider(const types_by_name &).
 * First one is represents list of types that are known to provider (like factory type and created
 * object type), second one creates provider based on list of all known types.
 * 
 * This class is reqruied due to Qt limitations of interactions between plugins and QMetaType system.
 * If QMetaType have worked properly inside plugins it could be used to extract parameter and return
 * types of invokable methods (using QMetaMethod::returnType() and QMetaMethod::paremeterType() with
 * QMetaType::metaObjectForType(int). As it does not work, Injeqt stores all known QMetaObject in
 * common @see types_by_name structure (that can only by computed in @see injector when all modules are known)
 * and uses QMetaMethod::typeName() and QMetaMethod::returnType()::paramterTypes() to get names of
 * types, that are mapped using this @see types_by_name.
 */
class provider_configuration
{

public:
	explicit provider_configuration() {}
	virtual ~provider_configuration() {}

	/**
	 * @return list of all types that this configuration knows (like factory type and created object type)
	 */
	virtual std::vector<type> types() const = 0;

	/**
	 * @param known_types list of all types known to injector
	 * @return new provider object
	 */
	virtual std::unique_ptr<provider> create_provider(const types_by_name &known_types) const = 0;

};

}}

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

#include <memory>

/**
 * @file
 * @brief Contains classes and functions for implementation of modules.
 */

namespace injeqt { namespace internal {

/**
 * @brief Implementation of module class.
 * @see module class
 *
 * Main job of this class is to create providers and keep list of pointers to them. All providers
 * are created using add_ready_object(type, QObject *), add_type(type) and add_factory(type, type) methods.
 *
 * List can be retreived using providers() method. Injector that takes the module gets all providers from
 * that list and move it to itself.
 */
class INJEQT_API module_impl final
{

public:
	const std::vector<std::unique_ptr<provider_configuration>> & provider_configurations();

	/**
	 * @brief Add provider to list
	 * @param p provider to add
	 * @pre p must be valid
	 */
	void add_provider_configuration(std::unique_ptr<provider_configuration> p);

private:
	std::vector<std::unique_ptr<provider_configuration>> _provider_configurations;

};

}}

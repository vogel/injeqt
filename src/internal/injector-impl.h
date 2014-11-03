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

#include "implementations.h"
#include "injector-core.h"
#include "providers.h"
#include "types-by-name.h"

#include <vector>
#include <QtCore/QObject>

/**
 * @file
 * @brief Contains classes and functions for implementation of injector.
 */

namespace injeqt { namespace v1 {
	class module;
}}

namespace injeqt { namespace internal {

/**
 * @brief Implementation of injector class.
 * @see injector
 * @see injector_core
 *
 * Its main purpose is to own all modules passed to injector constructor and to pass everthing else
 * to injector_core class.
 */
class INJEQT_API injector_impl final
{

public:
	/**
	 * @brief Create empty injector_impl with no configuration.
	 *
	 * Injector with empty configuration will not be able to return any objects. Each call
	 * to get(const type &) will result in type_not_configured
	 */
	injector_impl();

	/**
	 * @brief Create injector configured with set of modules.
	 * @param modules set of modules containing configuration of injector
	 * @see injector::injector(std::vector<std::unique_ptr<module>>)
	 * @throw ambiguous_types if one or more types in @p modules is ambiguous
	 * @throw unresolvable_dependencies if a type with unresolvable dependency is found in @p modules
	 * @throw dependency_duplicated when one type occurs twice as a dependency
	 * @throw dependency_on_self when type depends on self
	 * @throw dependency_on_subtype when type depends on own supertype
	 * @throw dependency_on_subtype when type depends on own subtype
	 * @throw invalid_setter if any tagged setter has parameter that is not a QObject-derived pointer
	 * @throw invalid_setter if any tagged setter has parameter that is a QObject pointer
	 * @throw invalid_setter if any tagged setter has other number of parameters than one
	 *
	 * This constructor extract all providers from all modules and creates injector_core object
	 * with these providers.
	 */
	explicit injector_impl(std::vector<std::unique_ptr<::injeqt::v1::module>> modules);

	/**
	 * @brief Returns pointer to object of given type interface_type
	 * @param interface_type type of object to return.
	 * @throw unknown_type if @p interface_type was not configured in injector
	 * @throw instantiation_failed if instantiation of one of required types failed
	 * @pre !interface_type.is_empty()
	 * @pre !interface_type.is_qobject()
	 * @see injector::get<T>()
	 */
	QObject * get(const type &interface_type);

private:
	std::vector<std::unique_ptr<module>> _modules;
	injector_core _core;

	types_by_name extract_types() const;

};

}}

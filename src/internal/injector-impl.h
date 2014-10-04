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

#include "implementations.h"
#include "injeqt.h"
#include "providers.h"
#include "types-model.h"
#include "type.h"

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
 * @see injector class
 *
 * This class is core of Injeqt library.
 *
 * Its main purpose is to gather configuration from set of modules and construct objects with
 * resolved dependencies.
 *
 * Injector keeps list of all configured providers and of all already created objects.
 * Two methods objects_with(implementations, const type &) and objects_with(implementations, const types &)
 * are used to update list of already created objects with new ones.
 */
class INJEQT_API injector_impl final
{

public:
	/**
	 * @brief Create empty injector_impl with no configuration.
	 *
	 * Injector with empty configuration will not be able to return any objects. Each call
	 * to get(const type &) will result in type_not_configured_exception
	 */
	injector_impl();

	/**
	 * @brief Create injector confiugred with set of modules.
	 * @param modules set of modules containing configuration of injector
	 * @see injector::injector(std::vector<std::unique_ptr<module>>)
	 * @throw ambiguous_types_exception if one or more types in @p modules is ambiguous
	 * @throw unresolvable_dependencies_exception if a type with unresolvable dependency is found in @p modules
	 * @throw dependency_duplicated_exception when one type occurs twice as a dependency of another type
	 * @throw dependency_on_self_exception when type depends on self
	 * @throw dependency_on_subtype_exception when type depends on own supertype
	 * @throw dependency_on_subtype_exception when type depends on own subtype
	 * @throw invalid_setter_exception if any tagged setter has parameter that is not a QObject-based pointer
	 * @throw invalid_setter_exception if any tagged setter has other number of parameters than one
	 *
	 * This constructor extract all providers from all modules and creates types_model object
	 * to get all required information from them. After calling that all providers will be moved-out from
	 * modules.
	 */
	explicit injector_impl(std::vector<std::unique_ptr<::injeqt::v1::module>> modules);

	/**
	 * @brief Returns pointer to object of given type interface_type
	 * @param interface_type type of object to return.
	 * @throw unknown_type_exception if @p interface_type was not configured in injector
	 * @see injector::get<T>()
	 */
	QObject * get(const type &interface_type);

private:
	std::vector<std::unique_ptr<module>> _modules;

	providers _available_providers;
	implementations _objects;
	types_model _types_model;

	/**
	 * @brief Extract set of providers from all modules
	 * @todo Should check if types do not have duplicated
	 */
	providers extract_providers(const std::vector<std::unique_ptr<module>> &modules) const;

	/**
	 * @brief Extract all provided types and makes a types_model from them.
	 * @throw ambiguous_types_exception if one or more types in @p all_providers is ambiguous
	 * @throw unresolvable_dependencies_exception if a type with unresolvable dependency is found in @p all_providers
	 * @throw dependency_duplicated_exception when one type occurs twice as a dependency of another type
	 * @throw dependency_on_self_exception when type depends on self
	 * @throw dependency_on_subtype_exception when type depends on own supertype
	 * @throw dependency_on_subtype_exception when type depends on own subtype
	 * @throw invalid_setter_exception if any tagged setter has parameter that is not a QObject-based pointer
	 * @throw invalid_setter_exception if any tagged setter has other number of parameters than one
	 */
	types_model create_types_model(const providers &all_providers) const;

	/**
	 * @brief Create new list of implementation objects with object of type implementation_type
	 * @param objects list of already created objects
	 * @param implementation_type type of object to create
	 *
	 * This method get list of all not-already created objects required to properly instantiate implementation_type
	 * using required_to_instantiate(const type &, const types_model &, const implementations &) and then uses
	 * objects_with(implementations, const types &) to create all these objects.
	 */
	implementations objects_with(implementations objects, const type &implementation_type);

	/**
	 * @brief Create new list of implementation objects with objects of types implementation_types
	 * @param objects list of already created objects
	 * @param implementation_types set of types of object to create
	 * @throw type_not_configured_exception if type from implementation_types or any required type is not configured in injector
	 * @throw type_not_instantiated_exception if any provider fails to instantiate object
	 *
	 * For each type in the list this method check if that type requires other one using provider::required_types()
	 * and if so, calls objects_with(implementations, const type &) to update list with that requires object.
	 * After all these requiremens are met all types from implementation_types list that still were not instantiated
	 * are instantiated. Last step is to resolve and apply dependencies on newly created objects.
	 */
	implementations objects_with(implementations objects, const types &implementation_types);

};

}}

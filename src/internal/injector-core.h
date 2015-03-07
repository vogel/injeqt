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
#include "providers.h"
#include "types-by-name.h"
#include "types-model.h"

#include <vector>
#include <QtCore/QObject>

/**
 * @file
 * @brief Contains classes and functions for implementation of injector core.
 */

namespace injeqt { namespace internal {

/**
 * @brief Implementation of injector class.
 * @see injector
 * @see injector_impl
 *
 * This class is core of Injeqt library.
 *
 * Its main purpose is to gather configuration from set of providers and construct objects with
 * resolved dependencies.
 *
 * Injector keeps list of all configured providers and of all already created objects.
 * Two methods objects_with(implementations, const type &) and objects_with(implementations, const types &)
 * are used to update list of already created objects with new ones.
 */
class INJEQT_API injector_core final
{

public:
	/**
	 * @brief Create empty injector_core with no configuration.
	 *
	 * Injector with empty configuration will not be able to return any objects. Each call
	 * to get(const type &) will result in type_not_configured
	 */
	injector_core();

	/**
	 * @brief Create injector configured with set of providers.
	 * @param all_providers set of all providers available to injector
	 * @see injector::injector(std::vector<std::unique_ptr<module>>)
	 * @throw ambiguous_types if one or more types in @p providers is ambiguous
	 * @throw unresolvable_dependencies if a type with unresolvable dependency is found in @p providers
	 * @throw dependency_duplicated when one type occurs twice as a dependency
	 * @throw dependency_on_self when type depends on self
	 * @throw dependency_on_subtype when type depends on own supertype
	 * @throw dependency_on_subtype when type depends on own subtype
	 * @throw invalid_setter if any tagged setter has parameter that is not a QObject-derived pointer
	 * @throw invalid_setter if any tagged setter has parameter that is a QObject pointer
	 * @throw invalid_setter if any tagged setter has parameter that is a QObject-derived pointer of not configured type
	 * @throw invalid_setter if any tagged setter has other number of parameters than one
	 *
	 * This constructor creates types_model object to get all required information from providers. This object
	 * takes ownership of passed providers.
	 */
	explicit injector_core(types_by_name known_types, std::vector<std::unique_ptr<provider>> &&all_providers);

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

	/**
	 * @brief Inject dependencies into @p object.
	 * @param object object to inject dependencies into.
	 * @throw invalid_setter if any tagged setter has parameter that is not a QObject-derived pointer
	 * @throw invalid_setter if any tagged setter has parameter that is a QObject pointer
	 * @throw invalid_setter if any tagged setter has parameter that is a QObject-derived pointer of not configured type
	 * @throw invalid_setter if any tagged setter has other number of parameters than one
	 * @pre object != nullptr
	 *
	 * This method looks for invokable setters tagged with INJEQT_SETTER in @p object. If any of setter is not valid
	 * dependency injector setter or its parameter is of type not configured in injector an exception is thrown.
	 * If all setters are valid, they are called with proper objects (which may be already available in injector
	 * or created on demand).
	 */
	void inject_into(QObject *object);

private:
	types_by_name _known_types;
	providers _available_providers;
	implementations _objects;
	types_model _types_model;

	/**
	 * @brief Extract all provided types and makes a types_model from them.
	 * @throw ambiguous_types if one or more types in @p all_providers is ambiguous
	 * @throw unresolvable_dependencies if a type with unresolvable dependency is found in @p all_providers
	 * @throw dependency_duplicated when one type occurs twice as a dependency
	 * @throw dependency_on_self when type depends on self
	 * @throw dependency_on_subtype when type depends on own supertype
	 * @throw dependency_on_subtype when type depends on own subtype
	 * @throw invalid_setter if any tagged setter has parameter that is not a QObject-derived pointer
	 * @throw invalid_setter if any tagged setter has parameter that is a QObject pointer
	 * @throw invalid_setter if any tagged setter has other number of parameters than one
	 * @todo can be moved outside of class
	 */
	types_model create_types_model() const;

	/**
	 * @brief Create new list of implementation objects with object of type interface_type
	 * @param objects list of already created objects
	 * @param interface_type type of interface of object to create
	 * @throw instantiation_failed if instantiation of one of required types failed
	 * @throw unknown_type if interface_type does not have corresponding implementation
	 *
	 * This method first finds implementation_type valid for given interface_type. Then
	 * objects_with_implementation_type(implementations, const type &) is called.
	 */
	implementations objects_with_interface_type(implementations objects, const type &interface_type);

	/**
	 * @brief Create new list of implementation objects with object of type implementation_type
	 * @param objects list of already created objects
	 * @param implementation_type type of object to create
	 * @throw instantiation_failed if instantiation of one of required types failed
	 *
	 * This method get list of all not-already created objects required to properly instantiate implementation_type
	 * using required_to_satisfy(const type &, const types_model &, const implementations &) and then uses
	 * objects_with(implementations, const types &) to create all these objects.
	 */
	implementations objects_with_implementation_type(implementations objects, const type &implementation_type);

	/**
	 * @brief Create new list of implementation objects with objects of types implementation_types
	 * @param objects list of already created objects
	 * @param implementation_types set of types of object to create
	 * @throw instantiation_failed if instantiation of one of required types failed
	 *
	 * For each type in the list this method check if that type requires other one using provider::required_types()
	 * and if so, calls objects_with(implementations, const type &) to update list with that requires object.
	 * After all these requiremens are met all types from implementation_types list that still were not instantiated
	 * are instantiated. Last step is to resolve and apply dependencies on newly created objects.
	 */
	implementations objects_with(implementations objects, const types &implementation_types);

};

}}

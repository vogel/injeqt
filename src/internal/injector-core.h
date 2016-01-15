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

class provided_object;

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

	injector_core(const injector_core &) = delete;
	injector_core(injector_core &&) = default;

	injector_core & operator = (const injector_core &) = delete;
	injector_core & operator = (injector_core &&) = default;

	/**
	 * @brief Destroy injector_core.
	 *
	 * TODO: add note about INJEQT_DONE
	 */
	~injector_core();

	/**
	 * @brief Returns list of all configured types.
	 *
	 * Result contians only types explicitely configured in modules, not supertypes of these. It also includes
	 * results of provided_types() calls from all added parent injectors.
	 */
	std::vector<type> provided_types() const;

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
	 * This method looks for invokable setters tagged with INJEQT_SET in @p object. If any of setter is not valid
	 * dependency injector setter or its parameter is of type not configured in injector an exception is thrown.
	 * If all setters are valid, they are called with proper objects (which may be already available in injector
	 * or created on demand).
	 */
	void inject_into(QObject *object);

private:
	types_by_name _known_types;
	providers _available_providers;
	implementations _objects;
	implementations _resolved_objects;
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
	 * @brief Return type that implements @p interface_type.
	 * @throw unknown_type if @p interface_type does not have corresponding implementation
	 */
	type implementation_for(const type &interface_type) const;

	/**
	 * @brief Instantiate class of interface type @p interface_type and makes it available for use.
	 * @param interface_type type of interface of object to create
	 * @throw instantiation_failed if instantiation of one of required types failed
	 * @throw unknown_type if @p interface_type does not have corresponding implementation
	 *
	 * Instantiate class of interface type @p interface_type with all of its dependencies, then resolves them and
	 * calls INJEQT_INIT slots.
	 */
	void instantiate_interface(const type &interface_type);

	/**
	 * @brief Instantiate class of type @p implementation_type and makes it available for use.
	 * @param interface_type type of interface of object to create
	 * @throw instantiation_failed if instantiation of one of required types failed
	 *
	 * Instantiate class of exact type @p implementation_type with all of its dependencies, then resolves them and
	 * calls INJEQT_INIT slots.
	 */
	void instantiate_implementation(const type &implementation_type);

	/**
	 * @brief Return all dependencies for @p implementation_type.
	 */
	dependencies implementation_type_dependencies(const type &implementation_type) const;

	/**
	 * @brief Instantiate classes of interface types from @p interface_types and makes them available for use.
	 * @param interface_types types of interfaces of objects to create
	 * @throw instantiation_failed if instantiation of one of required types failed
	 * @pre No class from @p interface_types contains dependency that is not already resolved or not in @p interface_types
	 *
	 * Instantiate all classes of interface type @p interface_types without looking for dependencies.
	 */
	void instantiate_all(const types &interface_types);

	/**
	 * @brief Instantiate classes that are required before instantiating any of @p types_to_instantiate.
	 * @param types_to_instantiate types that are checked for list of required types
	 * @throw instantiation_failed if instantiation of one of required types failed
	 *
	 * Instantiate all classes that are returned from @see provider::required_type() methods of any provider
	 * for these types.
	 */
	void instantiate_required_types_for(const types &types_to_instantiate);

	/**
	 * @brief Return list of providers required to instantiate types from @p for_types.
	 */
	template<typename T>
	std::vector<provider *> providers_for(const T &for_types) const
	{
		auto result = std::vector<provider *>{};
		result.reserve(for_types.size());
		for (auto &&for_type : for_types)
		{
			auto provider_it = _available_providers.get(for_type);
			assert(provider_it != end(_available_providers));

			result.push_back(provider_it->get());
		}

		return result;
	}

	/**
	 * @brief Filter list of types from @p to_filter to include only non already instantiated types.
	 */
	std::vector<type> non_instantiated(const types &to_filter) const;

	/**
	 * @brief Instantiate types with @p providers.
	 */
	std::vector<provided_object> provide_objects(const std::vector<provider *> &providers);

	/**
	 * @brief Return objects that needs resolving from @p provided_objects.
	 * @see provider::require_resolving()
	 */
	std::vector<implementation> objects_to_resolve(const std::vector<provided_object> &provided_objects) const;

	/**
	 * @brief Extract implementations from vector of @p provided_object.
	 */
	std::vector<implementation> extract_implementations(const std::vector<provided_object> &provided_objects) const;

	/**
	 * @brief Return objects to store in list of instantiated objects.
	 *
	 * Each implementation object is returned with set of @see implementation instances that contains all unique inferfaces
	 * for that object, so it is later avaialble under all types it implements.
	 */
	std::vector<implementation> objects_to_store(const std::vector<implementation> &objects) const;

	/**
	 * @brief Resolve all @p objects dependencies, call all INJEQT_INIT slots and add types to list of resolved objects.
	 *
	 * This method assumes that all object dependencies are already instantiated.
	 */
	void resolve_objects(const std::vector<implementation> &objects);

	/**
	 * @brief Resolve all @p object dependencies.
	 *
	 * This method assumes that all object dependencies are already instantiated.
	 */
	void resolve_object(const implementation &object) const;

	/**
	 * @brief Resolve all @p object dependencies with @p object_dependencies.
	 */
	void resolve_object(const dependencies &object_dependencies, const implementation &object) const;

	/**
	 * @brief Instantiate all objects with INJEQT_INSTANCE_IMMEDIATE tag.
	 */
	void instantiate_all_immediate();

	/**
	 * @brief Call all INJEQT_INIT methods on given object in proper order.
	 */
	void call_init_methods(QObject *object) const;

	/**
	 * @brief Call all INJEQT_DONE methods on given object in proper order.
	 */
	void call_done_methods(QObject *object) const;

};

}}

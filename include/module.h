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

#include "injeqt.h"
#include "type.h"

#include <memory>

/**
 * @file
 * @brief Contains classes and functions for creating modules.
 */

namespace injeqt { namespace internal {
	class injector_impl;
	class module_impl;
}}

namespace injeqt { namespace v1 {

/**
 * @brief Module represents set of types and objects that can be injected and that can use injections.
 *
 * Injector is normally constructed from set of modules, each one contains
 * group of related types.
 *
 * Modules are move-only types and cannot by copied.
 *
 * To add a module to system one must inherit from it and configure it in constructor and
 * then pass instance of it to created injector class.
 *
 * Set of types in given module does not have to fully satify each one dependencies - it
 * is only required for a group of modules passed into injector.
 *
 * Module configuration is done by calling any of add_* method. Currently implemnted are:
 * add_ready_object, add_type, add_factory.
 */
class INJEQT_API module
{

public:
	module();
	virtual ~module();

protected:
	/**
	 * @brief Add already constructed object to module.
	 * @tparam T type of added object (must be inherited from QObject).
	 * @param object added object
	 * @throw empty_type_exception when passed type @p T is an empty type
	 * @throw qobject_type_exception when passed type @p T represents QObject
	 * @throw invalid_qobject_exception when passed @p object is nullptr or does not have valid QMetaType
	 * @throw interface_not_implemented_exception when passed @p object does not implements type @p T
	 *
	 * It has to be fully constructed and must not expect any injection to take place on it
	 * (even if it has INJEQT_SETTER methods). However this object could be injected into other
	 * ones as it if is of type T.
	 *
	 * Real type of object must be same as T or descend from it. In other cases an exception
	 * will be thrown.
	 *
	 * Module class does not take ownership of this object. Either subclass of module must do
	 * it in any way or calee must ensure that object will be destroyed after module is destroyed.
	 *
	 * Example usage:
	 *
	 *     class injectable : public QObject
	 *     {
	 *         Q_OBJECT
	 *     };
	 *
	 *     class ready_module : public module
	 *     {
	 *         ready_module()
	 *         {
	 *              _ready_object = std::make_unique<injectable>();
	 *              add_ready_object<injectable>(_ready_object.get());
	 *         }
	 *     };
	 *
	 *     class ready_module2 : public module
	 *     {
	 *         ready_module2(injectable *ready_object)
	 *         {
	 *              add_ready_object<injectable>(ready_object);
	 *         }
	 *     };
	 *
	 * In second example responsibility for ready_object's lifetime lies on ready_module2 object
	 * creator.
	 *
	 * When injector object is created with any of these modules this code will lead to return
	 * the same object thas was passed to add_ready_object method:
	 *
	 *     auto o = injector.get<injectable>();
	 */
	template<typename T>
	void add_ready_object(QObject *object)
	{
		add_ready_object(make_type<T>(), object);
	}

	/**
	 * @brief Add type that can be default-constructed to module.
	 * @tparam T type added to module (must be inherited from QObject).
	 * @throw empty_type_exception when passed type @p T is an empty type
	 * @throw qobject_type_exception when passed type @p T represents QObject
	 * @throw default_constructor_not_found_exception is @p T does not have default constructor tagged with Q_INVOKABLE
	 *
	 * When an object of that type (or one of inherited types) is requested from injector or
	 * as a dependency to other objects then default constructor of T is invoked to create it.
	 * This constructor must be marked with Q_INVOKABLE macro and must not have any parameters
	 * (even with default values). In other cases this method will throw an exception.
	 *
	 * After object of type T is created it will be injected will all required dependencies
	 * and it will remain in cache until owning injector object exists. Onle one instance of
	 * T will exist at one time inside injector (this may change when in future scope abstraction
	 * will be added to library, but it won't affect default behavior). Injector takes ownership
	 * of created T object.
	 *
	 * Example usage:
	 *
	 *     class injectable : public QObject
	 *     {
	 *         Q_OBJECT
	 *     public:
	 *         Q_INVOKABLE injectable() {}
	 *     };
	 *
	 *     class type_module : public module
	 *     {
	 *         type_module()
	 *         {
	 *              add_type<injectable>();
	 *         }
	 *     };
	 *
	 * When injector object is created with that modules this code will lead to creation of new object
	 * of type injectable (for o1) and return it and (for o2) to return it again.
	 *
	 *     auto o1 = injector.get<injectable>();
	 *     auto o2 = injector.get<injectable>();
	 */
	template<typename T>
	void add_type()
	{
		add_type(make_validated_type<T>());
	}

	/**
	 * @brief Add type that can be created by factory to module.
	 * @tparam T type added to module (must be inherited from QObject).
	 * @tparam F factory type (must be inherited from QObject).
	 * @throw empty_type_exception when passed type @p T is an empty type
	 * @throw qobject_type_exception when passed type @p T represents QObject
	 * @throw empty_type_exception when passed type @p F is an empty type
	 * @throw qobject_type_exception when passed type @p F represents QObject
	 * @throw unique_factory_method_not_found_exception when type F does not have unique factory method for T
	 *
	 * When an object of that type (or one of inherited types) is requested from injector or
	 * as a dependency to other objects then object of type F is requted first and its method
	 * returning pointer to T* and not accepting any arguments if called to create T object.
	 * Type F must be registered in any of injector modules.
	 *
	 * Type F must contain exactly one method marked with Q_INVOKABLE macro that returns object
	 * of type T* and does not have any arguments. In other case an exception will be thrown.
	 * Qt meta-object system unfortunately does not allow us to require returned type to be
	 * std::unique_ptr<T> which would make object ownership clear in code.
	 *
	 * After object of type T is created it will be injected will all required dependencies
	 * and it will remain in cache until owning injector object exists. Onle one instance of
	 * T will exist at one time inside injector (this may change when in future scope abstraction
	 * will be added to library, but it won't affect default behavior). Injector takes ownership
	 * of created T object.
	 *
	 * Example usage:
	 *
	 *     class injectable : public QObject
	 *     {
	 *         Q_OBJECT
	 *     public:
	 *         injectable(int arg) { (void)arg; }
	 *     };
	 *
	 *     class injectable_factory : public QObject
	 *     {
	 *         Q_OBJECT
	 *     public:
	 *         Q_INVOKABLE injectable_factory() {}
	 *         Q_INVOKABLE injectable * create()
	 *             { return new injectable{2}; }
	 *     };
	 *
	 *     class type_module : public module
	 *     {
	 *         type_module()
	 *         {
	 *              add_type<injectable_factory>();
	 *              add_factory<injectable, injectable_factory>();
	 *         }
	 *     };
	 *
	 * Note that injectable constructor does not have to be maked with Q_INVOKABLE. Also note that
	 * injectable_factory had to be added itself to module (it could be added in another module
	 * that would have to be included into final injector).
	 *
	 * When injector object is created with that modules this code will lead to creation of new object
	 * of type injectable_factory (for o1) and then in call of injectable_factory::create(). Second line
	 * will lead to return of cached object (the same as o1) without any additional calls.
	 *
	 *     auto o1 = injector.get<injectable>();
	 *     auto o2 = injector.get<injectable>();
	 */
	template<typename T, typename F>
	void add_factory()
	{
		add_factory(make_validated_type<T>(), make_validated_type<F>());
	}

private:
	friend class ::injeqt::internal::injector_impl;
	std::unique_ptr<injeqt::internal::module_impl> _pimpl;

	/**
	 * @see add_ready_object<T>(QObject *);
	 */
	void add_ready_object(type t, QObject *object);

	/**
	 * @see add_type<T>();
	 */
	void add_type(type t);

	/**
	 * @see add_factory<T, F>();
	 */
	void add_factory(type t, type f);

};

}}

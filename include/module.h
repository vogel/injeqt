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

#include "injeqt-global.h"
#include "type.h"

#include <memory>

namespace injeqt { namespace internal {
	class injector_impl;
	class module_impl;
}}

namespace injeqt { namespace v1 {

class INJEQT_API module
{

public:
	module();
	virtual ~module();

	template<typename T>
	void add_ready_object(QObject *object)
	{
		add_ready_object(make_type<T>(), object);
	}


	template<typename T>
	void add_type()
	{
		add_type(make_type<T>());
	}


	template<typename T, typename F>
	void add_factory()
	{
		add_factory(make_type<T>(), make_type<F>());
	}


private:
	friend class ::injeqt::internal::injector_impl;
	std::unique_ptr<injeqt::internal::module_impl> _pimpl;

	void add_ready_object(type t, QObject *object);
	void add_type(type t);
	void add_factory(type t, type f);

};

}};

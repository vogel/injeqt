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

#include "provider.h"

#include <functional>
#include <memory>

using namespace injeqt::internal;
using namespace injeqt::v1;

class mocked_provider : public provider
{

public:
	explicit mocked_provider(type provided_type, types required_types, std::function<QObject *()> provide = [](){ return nullptr; }) :
		_provided_type{std::move(provided_type)},
		_required_types{std::move(required_types)},
		_provide{std::move(provide)},
		_object{nullptr}
	{}
	virtual ~mocked_provider() {}

	virtual const type & provided_type() const override { return _provided_type; };

	virtual QObject * provide(injector_core &i) override
	{
		if (!_object)
			_object = _provide();
		return _object;
	};

	virtual types required_types() const override { return _required_types; };
	
	QObject * object() const { return _object; }

private:
	type _provided_type;
	types _required_types;
	std::function<QObject *()> _provide;
	QObject *_object;

};

template<typename T, typename... R>
std::unique_ptr<mocked_provider> make_mocked_provider()
{
	return std::unique_ptr<mocked_provider>(new mocked_provider{make_type<T>(), types{make_type<R>()...}, [](){ return new T(); }});
}

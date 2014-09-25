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

#include "provider-by-factory.h"

#include "injector-impl.h"

namespace injeqt { namespace internal {

provider_by_factory::provider_by_factory(factory_method factory) :
	_factory{std::move(factory)}
{
}

provider_by_factory::provider_by_factory(provider_by_factory &&x)
{
	std::swap(*this, x);
}

provider_by_factory::~provider_by_factory()
{
}

provider_by_factory & provider_by_factory::operator = (provider_by_factory &&x)
{
	std::swap(*this, x);
	return *this;
}

const type & provider_by_factory::provided_type() const
{
	return _factory.result_type();
}

const factory_method & provider_by_factory::factory() const
{
	return _factory;
}

QObject * provider_by_factory::provide(injector_impl &i)
{
	if (!_object)
	{
		auto factory_object = i.get(_factory.object_type());
		_object = _factory.invoke(factory_object);
	}

	return _object.get();
}

types provider_by_factory::required_types() const
{
	return types{_factory.object_type()};
}

}}

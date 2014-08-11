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

#include "factory-method.h"
#include "provider.h"
#include "injeqt.h"

using namespace injeqt::v1;

namespace injeqt { namespace internal {

class provider_by_factory final : public provider
{

public:
	explicit provider_by_factory(factory_method factory);
	virtual ~provider_by_factory();

	virtual const type & created_type() const override;
	virtual QObject * create(injector_impl &i) override;
	virtual types required_types() const;

private:
	factory_method _factory;
	std::unique_ptr<QObject> _object;

};

}}

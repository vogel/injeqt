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
#include "default-constructor-method.h"
#include "injeqt-global.h"

namespace injeqt { namespace v1 {

class provider_by_default_constructor final : public provider
{

public:
	explicit provider_by_default_constructor(default_constructor_method constructor);
	virtual ~provider_by_default_constructor() {}

	virtual const type & created_type() const override;
	virtual QObject * create(scope &s) override;
	virtual types required_types() const { return types{}; }

private:
	default_constructor_method _constructor;
	std::unique_ptr<QObject> _object;

};

}}

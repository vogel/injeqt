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
#include "injeqt.h"
#include "type.h"

#include <memory>

namespace injeqt { namespace internal {

class INJEQT_API module_impl final
{

public:
	void add_ready_object(type t, QObject *object);
	void add_type(type t);
	void add_factory(type t, type f);

	std::vector<std::unique_ptr<provider>> & providers();

private:
	std::vector<std::unique_ptr<provider>> _providers;

	void add_provider(std::unique_ptr<provider> c);

};

}}

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
#include "scope.h"
#include "type.h"

#include <vector>
#include <QtCore/QObject>

namespace injeqt { namespace v1 {
	class module;
}}

namespace injeqt { namespace internal {

class INJEQT_API injector_impl final
{

public:
	explicit injector_impl(std::vector<std::unique_ptr<::injeqt::v1::module>> modules);

	QObject * get(const type &interface_type);

private:
	std::vector<std::unique_ptr<module>> _modules;
	scope _singleton_scope;

	scope scope_from_modules(const std::vector<std::unique_ptr<module>> &modules) const;

};

}}

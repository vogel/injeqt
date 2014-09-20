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

#include "implementations.h"
#include "injeqt.h"
#include "providers.h"
#include "types-model.h"
#include "type.h"

#include <vector>
#include <QtCore/QObject>

namespace injeqt { namespace v1 {
	class module;
}}

namespace injeqt { namespace internal {

INJEQT_EXCEPTION(injector_exception, injeqt_exception);
INJEQT_EXCEPTION(type_not_configured_exception, injector_exception);
INJEQT_EXCEPTION(unresolved_dependencies_exception, injector_exception);
INJEQT_EXCEPTION(type_not_instantiated_exception, injector_exception);

class INJEQT_API injector_impl final
{

public:
	injector_impl();
	explicit injector_impl(std::vector<std::unique_ptr<::injeqt::v1::module>> modules);

	QObject * get(const type &interface_type);

private:
	std::vector<std::unique_ptr<module>> _modules;

	providers _available_providers;
	implementations _objects;
	types_model _types_model;

	providers extract_providers(const std::vector<std::unique_ptr<module>> &modules) const;
	types_model create_types_model(const providers &all_providers) const;

	implementations objects_with(implementations objects, const type &implementation_type);
	implementations objects_with(implementations objects, const types &implementation_types);

};

}}

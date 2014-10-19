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

#include "injector-impl.h"

#include <injeqt/exception/unknown-type.h>
#include <injeqt/module.h>

#include "provider-by-default-constructor.h"
#include "provider-ready.h"
#include "provider.h"
#include "module-impl.h"
#include "required-to-instantiate.h"
#include "resolve-dependencies.h"
#include "resolved-dependency.h"

#include <cassert>

namespace injeqt { namespace internal {
// TODO: tests!!
injector_impl::injector_impl()
{
}

injector_impl::injector_impl(std::vector<std::unique_ptr<module>> modules) :
	// modules are only stored because these can own objects used by injector
	_modules{std::move(modules)},
	_core{extract_providers(_modules)}
{
}

providers injector_impl::extract_providers(const std::vector<std::unique_ptr<module>> &modules) const
{
	// TODO: should check if there are 2 providers of the same type
	auto result = std::vector<std::unique_ptr<provider>>{};
	for (auto &&module : _modules)
		std::move(std::begin(module->_pimpl->providers()), std::end(module->_pimpl->providers()), std::back_inserter(result));
	return providers{std::move(result)};
}

QObject * injector_impl::get(const type &interface_type)
{
	return _core.get(interface_type);
}

}}

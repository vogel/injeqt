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

#include "provider-by-default-constructor.h"
#include "provider-ready.h"
#include "provider.h"
#include "default-constructor-method.h"
#include "module-impl.h"
#include "module.h"

namespace injeqt { namespace internal {

injector_impl::injector_impl(std::vector<std::unique_ptr<module>> modules) :
	_modules{std::move(modules)},
	_singleton_scope{scope_from_modules(_modules)}
{
}

QObject * injector_impl::get(const type &interface_type)
{
	return _singleton_scope.get(interface_type);
}

scope injector_impl::scope_from_modules(const std::vector<std::unique_ptr<module>> &modules) const
{
	auto all_providers = std::vector<std::unique_ptr<provider>>{};
	for (auto &&module : modules)
		std::move(std::begin(module->_pimpl->providers()), std::end(module->_pimpl->providers()), std::back_inserter(all_providers));

	auto all_types = std::vector<type>{};
	std::transform(std::begin(all_providers), std::end(all_providers), std::back_inserter(all_types),
		[](const std::unique_ptr<provider> &c){ return c->created_type(); });

	// TODO: check for duplicates and stuff
	return make_scope(providers{std::move(all_providers)}, all_types);
}

}}

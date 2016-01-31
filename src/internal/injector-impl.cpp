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

#include "containers.h"
#include "interfaces-utils.h"
#include "provider-by-default-constructor.h"
#include "provider-by-parent-injector-configuration.h"
#include "provider-ready.h"
#include "provider.h"
#include "module-impl.h"
#include "required-to-satisfy.h"
#include "resolve-dependencies.h"
#include "resolved-dependency.h"

#include <cassert>

namespace injeqt { namespace internal {

injector_impl::injector_impl()
{
}

injector_impl::injector_impl(std::vector<std::unique_ptr<module>> modules) :
	// modules are only stored because these can own objects used by injector
	_modules{std::move(modules)}
{
	init(std::vector<injector_impl *>{});
}

injector_impl::injector_impl(std::vector<injector_impl *> super_injectors, std::vector<std::unique_ptr<module>> modules) :
	// modules are only stored because these can own objects used by injector
	_modules{std::move(modules)}
{
	init(super_injectors);
}

void injector_impl::init(std::vector<injector_impl *> super_injectors)
{
	auto extract_provider_configurations_lambda = [](const std::unique_ptr<module> &m){ return m->_pimpl->provider_configurations(); };
	auto extract_provider_configurations = std::function<std::vector<std::shared_ptr<provider_configuration>>(const std::unique_ptr<module> &)>{extract_provider_configurations_lambda};
	auto provider_configurations = extract(_modules, extract_provider_configurations);

	for (auto &&super_injector : super_injectors)
		for (auto &&provided_type : super_injector->provided_types())
			provider_configurations.push_back(std::make_shared<provider_by_parent_injector_configuration>(super_injector, provided_type));

	auto extract_types_lamdba = [](const std::shared_ptr<provider_configuration> &pc){
		auto result = std::vector<type>{};
		for (auto &&t : pc->types())
		{
			auto interfaces = extract_interfaces(t);
			std::copy(std::begin(interfaces), std::end(interfaces), std::back_inserter(result));
		}
		return result;
	};
	auto extract_types = std::function<std::vector<type>(const std::shared_ptr<provider_configuration> &)>{extract_types_lamdba};
	auto known_types = types_by_name{extract(provider_configurations, extract_types)};

	auto create_provider_lambda = [&known_types](const std::shared_ptr<provider_configuration> &pc){ return pc->create_provider(known_types); };
	auto create_provider = std::function<std::unique_ptr<provider>(std::shared_ptr<provider_configuration>)>{create_provider_lambda};
	auto providers = transform(provider_configurations, create_provider);

	_core = injector_core{known_types, std::move(providers)};
}

std::vector<type> injector_impl::provided_types() const
{
	return _core.provided_types();
}

void injector_impl::instantiate(const type &interface_type)
{
	assert(!interface_type.is_empty());
	assert(!interface_type.is_qobject());

	_core.instantiate(interface_type);
}

QObject * injector_impl::get(const type &interface_type)
{
	assert(!interface_type.is_empty());
	assert(!interface_type.is_qobject());

	return _core.get(interface_type);
}

void injector_impl::inject_into(QObject *object)
{
	assert(object);

	_core.inject_into(object);
}

}}

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

#include "injector.h"

#include "module.h"

namespace injeqt { namespace v1 {

namespace {

scope scope_from_modules(const std::vector<std::unique_ptr<module>> &modules)
{
	auto all_types = std::vector<type>{};
	auto all_implementations = std::vector<implementation>();

	for (auto &&module : modules)
	{
		std::copy(std::begin(module->types()), std::end(module->types()), std::back_inserter(all_types));
		std::copy(std::begin(module->implementations()), std::end(module->implementations()), std::back_inserter(all_implementations));
	}

	// TODO: check for duplicates and stuff
	return make_scope(all_types, implementations{all_implementations});
}

}

injector::injector(std::vector<std::unique_ptr<module>> modules) :
	_modules{std::move(modules)},
	_singleton_scope{scope_from_modules(_modules)}
{
}

QObject * injector::get(const type &interface_type)
{
	return _singleton_scope.get(interface_type);
}

}}

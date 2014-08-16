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

#include "module-impl.h"

#include "provider-by-default-constructor.h"
#include "provider-by-factory.h"
#include "provider-ready.h"

#include <QtCore/QMetaObject>

namespace injeqt { namespace internal {
// TODO: tests!!!
void module_impl::add_ready_object(type t, QObject *object)
{
	auto p = std::unique_ptr<provider_ready>{new provider_ready{implementation{std::move(t), object}}};
	validate(*p.get());

	add_provider(std::move(p));
}

void module_impl::add_type(type t)
{
	auto p = std::unique_ptr<provider_by_default_constructor>{new provider_by_default_constructor{make_default_constructor_method(std::move(t))}};
	validate(*p.get());

	add_provider(std::move(p));
}

void module_impl::add_factory(type t, const type f)
{
	auto p = std::unique_ptr<provider_by_factory>{new provider_by_factory{make_factory_method(std::move(t), std::move(f))}};
	validate(*p.get());

	add_provider(std::move(p));
}

void module_impl::add_provider(std::unique_ptr<provider> c)
{
	_providers.push_back(std::move(c));
}

std::vector<std::unique_ptr<provider>> & module_impl::providers()
{
	return _providers;
}

}}

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

#include <injeqt/module.h>

#include "module-impl.h"
#include "provider-by-default-constructor-configuration.h"
#include "provider-by-factory-configuration.h"
#include "provider-ready-configuration.h"

#include <QtCore/QMetaObject>
#include <cassert>

namespace injeqt { namespace v1 {

module::module() :
	_pimpl{new injeqt::internal::module_impl{}}
{
}

module::~module()
{
}

void module::add_ready_object(type t, QObject *object)
{
	assert(!t.is_empty());

	auto p = std::unique_ptr<internal::provider_ready_configuration>{
		new internal::provider_ready_configuration{std::move(t), object}};
	_pimpl->add_provider_configuration(std::move(p));
}

void module::add_type(type t)
{
	assert(!t.is_empty());

	auto p = std::unique_ptr<internal::provider_by_default_constructor_configuration>{
		new internal::provider_by_default_constructor_configuration{std::move(t)}};
	_pimpl->add_provider_configuration(std::move(p));
}

void module::add_factory(type t, type f)
{
	assert(!t.is_empty());
	assert(!f.is_empty());

	auto p = std::unique_ptr<internal::provider_by_factory_configuration>{
		new internal::provider_by_factory_configuration{std::move(t), std::move(f)}};
	_pimpl->add_provider_configuration(std::move(p));
}

}}

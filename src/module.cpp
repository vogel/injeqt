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

#include "module.h"

#include "exception/empty-type-exception.h"
#include "exception/interface-not-implemented-exception.h"
#include "exception/invalid-qobject-exception.h"
#include "exception/qobject-type-exception.h"
#include "extract-interfaces.h"
#include "module-impl.h"
#include "provider-by-default-constructor.h"
#include "provider-by-factory.h"
#include "provider-ready.h"

#include <QtCore/QMetaObject>

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
	if (t.is_empty())
		throw exception::empty_type_exception{};
	if (t.is_qobject())
		throw exception::qobject_type_exception();

	if (!object || !object->metaObject())
		throw exception::invalid_qobject_exception{};

	auto object_type = type{object->metaObject()};
	auto implements = internal::extract_interfaces(object_type);
	if (!implements.contains(t))
		throw exception::interface_not_implemented_exception{};

	_pimpl->add_ready_object(t, object);
}

void module::add_type(type t)
{
	_pimpl->add_type(t);
}

void module::add_factory(type t, type f)
{
	_pimpl->add_factory(t, f);
}

}}

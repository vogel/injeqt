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

#include "setter-method.h"

#include <injeqt/type.h>

#include "interfaces-utils.h"

#include <cassert>

namespace injeqt { namespace internal {

setter_method::setter_method()
{
}

setter_method::setter_method(QMetaMethod meta_method) :
	_object_type{meta_method.enclosingMetaObject()},
	_parameter_type{QMetaType::metaObjectForType(meta_method.parameterType(0))},
	_meta_method{std::move(meta_method)}
{
	assert(meta_method.methodType() == QMetaMethod::Slot);
	assert(meta_method.parameterCount() == 1);
	assert(meta_method.enclosingMetaObject() != nullptr);
	assert(!_parameter_type.is_empty());
}

bool setter_method::is_empty() const
{
	return !_meta_method.isValid();
}

const type & setter_method::object_type() const
{
	assert(!is_empty());

	return _object_type;
}

const type & setter_method::parameter_type() const
{
	assert(!is_empty());

	return _parameter_type;
}

const QMetaMethod & setter_method::meta_method() const
{
	return _meta_method;
}

std::string setter_method::signature() const
{
	return _meta_method.methodSignature().data();
}

bool setter_method::invoke(QObject *on, QObject *parameter) const
{
	assert(!is_empty());
	assert(on != nullptr);
	assert(type{on->metaObject()} == _object_type);
	assert(parameter != nullptr);
	assert(!type{parameter->metaObject()}.is_empty());
	assert(implements(type{parameter->metaObject()}, _parameter_type));

	return _meta_method.invoke(on, Q_ARG(QObject *, parameter));
}

bool operator == (const setter_method &x, const setter_method &y)
{
	if (x.object_type() != y.object_type())
		return false;

	if (x.parameter_type() != y.parameter_type())
		return false;

	if (x.meta_method() != y.meta_method())
		return false;

	return true;
}

bool operator != (const setter_method &x, const setter_method &y)
{
	return !(x == y);
}

}}

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

#include <injeqt/exception/invalid-setter.h>
#include <injeqt/type.h>

#include "interfaces-utils.h"

#include <cassert>

namespace injeqt { namespace internal {

bool setter_method::is_setter_tag(const std::string &tag)
{
	return tag == "INJEQT_SET" || tag == "INJEQT_SETTER";
}

bool setter_method::validate_setter_method(type parameter_type, const QMetaMethod &meta_method)
{
	auto meta_object = meta_method.enclosingMetaObject();
	if (!meta_object)
		throw exception::invalid_setter{std::string{"setter does not have enclosing meta object: "} + "?::" + meta_method.methodSignature().data()};
	if (meta_method.methodType() == QMetaMethod::Signal)
		throw exception::invalid_setter{std::string{"setter is signal: "} + meta_object->className() + "::" + meta_method.methodSignature().data()};
	if (meta_method.methodType() == QMetaMethod::Constructor)
		throw exception::invalid_setter{std::string{"setter is constructor: "} + meta_object->className() + "::" + meta_method.methodSignature().data()};
	if (!is_setter_tag(meta_method.tag()))
		throw exception::invalid_setter{std::string{"setter does not have valid tag: "} + meta_object->className() + "::" + meta_method.methodSignature().data()};
	if (meta_method.parameterCount() != 1)
		throw exception::invalid_setter{std::string{"invalid parameter count: "} + meta_object->className() + "::" + meta_method.methodSignature().data()};
	if (parameter_type.is_empty() || parameter_type.is_qobject())
		throw exception::invalid_setter{std::string{"invalid parameter (empty): "} + meta_object->className() + "::" + meta_method.methodSignature().data()};
	if (parameter_type.is_empty())
		throw exception::invalid_setter{std::string{"invalid parameter (qobject): "} + meta_object->className() + "::" + meta_method.methodSignature().data()};
	if (parameter_type.name() + "*" != std::string{meta_method.parameterTypes()[0].data()})
		throw exception::invalid_setter{std::string{"invalid parameter (type): "} + meta_object->className() + "::" + meta_method.methodSignature().data()};
	return true;
}

setter_method::setter_method()
{
}

setter_method::setter_method(type parameter_type, QMetaMethod meta_method) :
	_object_type{meta_method.enclosingMetaObject()},
	_parameter_type{std::move(parameter_type)},
	_meta_method{std::move(meta_method)}
{
	assert(validate_setter_method(parameter_type, meta_method));
}

bool setter_method::is_empty() const
{
	return !_meta_method.isValid();
}

const type & setter_method::object_type() const
{
	return _object_type;
}

const type & setter_method::parameter_type() const
{
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
	assert(implements(type{on->metaObject()}, _object_type));
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

bool operator < (const setter_method &x, const setter_method &y)
{
	if (x.object_type() < y.object_type())
		return true;

	if (x.object_type() > y.object_type())
		return false;

	if (x.parameter_type() < y.parameter_type())
		return true;

	if (x.parameter_type() > y.parameter_type())
		return false;

	return x.meta_method().methodSignature() < y.meta_method().methodSignature();
}

bool operator > (const setter_method &x, const setter_method &y)
{
	return y < x;
}

bool operator <= (const setter_method &x, const setter_method &y)
{
	return !(y < x);
}

bool operator >= (const setter_method &x, const setter_method &y)
{
	return !(x < y);
}

setter_method make_setter_method(const types_by_name &known_types, const QMetaMethod &meta_method)
{
	auto parameter_type = meta_method.parameterCount() == 1
		? type_by_pointer(known_types, meta_method.parameterTypes()[0].data())
		: type{nullptr};
	setter_method::validate_setter_method(parameter_type, meta_method);

	return setter_method{parameter_type, meta_method};
}

}}

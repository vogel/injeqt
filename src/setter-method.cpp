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

#include "object-with-meta.h"

namespace injeqt { namespace v1 {

namespace {

std::string exception_message(const QMetaMethod &method)
{
	return method.enclosingMetaObject()
		? std::string{method.enclosingMetaObject()->className()} + "::" + method.methodSignature().data()
		: std::string{"<no-object>::"} + method.methodSignature().data();
}

}

setter_method::setter_method(QMetaMethod meta_method)
try :
	_object_type{meta_method.enclosingMetaObject()},
	_parameter_type{QMetaType::metaObjectForType(meta_method.parameterType(0))},
	_meta_method{std::move(meta_method)}
{
	if (meta_method.parameterCount() != 1)
		throw setter_too_many_parameters_exception(exception_message(meta_method));
}
catch (invalid_type_exception &e)
{
	throw invalid_setter_exception(exception_message(meta_method));
}

type setter_method::object_type() const
{
	return _object_type;
}

type setter_method::parameter_type() const
{
	return _parameter_type;
}

std::string setter_method::signature() const
{
	return _meta_method.methodSignature().data();
}

bool setter_method::invoke(const object_with_meta &on, const object_with_meta &parameter) const
{
	return _meta_method.invoke(on.object(), Q_ARG(QObject *, parameter.object()));
}

bool operator == (const setter_method &x, const setter_method &y)
{
	if (std::addressof(x) == std::addressof(y))
		return true;

	if (x.object_type() != y.object_type())
		return false;

	if (x.parameter_type() != y.parameter_type())
		return false;

	if (x.signature() != y.signature())
		return false;

	return true;
}

bool operator != (const setter_method &x, const setter_method &y)
{
	return !(x == y);
}

}}

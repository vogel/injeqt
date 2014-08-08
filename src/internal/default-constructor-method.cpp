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

#include "default-constructor-method.h"

namespace injeqt { namespace v1 {

default_constructor_method::default_constructor_method(QMetaMethod meta_method)
try :
	_object_type{meta_method.enclosingMetaObject()},
	_meta_method{std::move(meta_method)}
{
	if (meta_method.methodType() != QMetaMethod::Constructor)
		throw invalid_default_constructor_exception{};
	if (meta_method.parameterCount() != 0)
		throw invalid_default_constructor_exception{};
}
catch (invalid_type_exception &e)
{
	throw invalid_default_constructor_exception{};
}

const type & default_constructor_method::object_type() const
{
	return _object_type;
}

std::unique_ptr<QObject> default_constructor_method::invoke() const
{
	return std::unique_ptr<QObject>(_meta_method.enclosingMetaObject()->newInstance());
}

bool operator == (const default_constructor_method &x, const default_constructor_method &y)
{
	if (x.object_type() != y.object_type())
		return false;

	return true;
}

bool operator != (const default_constructor_method &x, const default_constructor_method &y)
{
	return !(x == y);
}

default_constructor_method make_default_constructor_method(const type &t)
{
	auto meta_object = t.meta_object();
	auto constructor_count = meta_object->constructorCount();
	for (decltype(constructor_count) i = 0; i < constructor_count; i++)
	{
		auto constructor = meta_object->constructor(i);
		if (constructor.methodType() != QMetaMethod::Constructor)
			continue;
		if (constructor.parameterCount() != 0)
			continue;
		return default_constructor_method{constructor};
	}

	throw no_default_constructor_exception{t.name()};
}

}}

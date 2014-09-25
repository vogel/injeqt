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

#include "factory-method.h"

#include "extract-interfaces.h"

#include <cassert>

namespace injeqt { namespace internal {

factory_method::factory_method()
{
}

factory_method::factory_method(QMetaMethod meta_method) :
	_object_type{meta_method.enclosingMetaObject()},
	_result_type{QMetaType::metaObjectForType(meta_method.returnType())},
	_meta_method{std::move(meta_method)}
{
	assert(meta_method.methodType() == QMetaMethod::Method || meta_method.methodType() == QMetaMethod::Slot);
	assert(meta_method.parameterCount() == 0);
	assert(meta_method.enclosingMetaObject() != nullptr);
}

bool factory_method::is_empty() const
{
	return !_meta_method.isValid();
}

const type & factory_method::object_type() const
{
	assert(!is_empty());

	return _object_type;
}

const type & factory_method::result_type() const
{
	assert(!is_empty());

	return _result_type;
}

const QMetaMethod & factory_method::meta_method() const
{
	return _meta_method;
}

std::unique_ptr<QObject> factory_method::invoke(QObject *on) const
{
	assert(!is_empty());
	assert(on != nullptr);
	assert(meta_method().enclosingMetaObject() == on->metaObject());

	QObject *result = nullptr;
	_meta_method.invoke(on, QReturnArgument<QObject *>((_result_type.name() + "*").c_str(), result)); // TODO: check for false result
	return std::unique_ptr<QObject>{result};
}

bool operator == (const factory_method &x, const factory_method &y)
{
	if (x.object_type() != y.object_type())
		return false;

	if (x.result_type() != y.result_type())
		return false;

	return true;
}

bool operator != (const factory_method &x, const factory_method &y)
{
	return !(x == y);
}

factory_method make_factory_method(const type &t, const type &f)
{
	assert(!t.is_empty());
	assert(!t.is_qobject());
	assert(!f.is_empty());
	assert(!f.is_qobject());

	auto meta_object = f.meta_object();
	auto method_count = meta_object->methodCount();
	auto factory_methods = std::vector<factory_method>{};

	for (decltype(method_count) i = 0; i < method_count; i++)
	{
		auto method = meta_object->method(i);
		if (method.parameterCount() != 0)
			continue;
		auto return_type_meta_object = QMetaType::metaObjectForType(method.returnType());
		if (!return_type_meta_object)
			continue;
		auto return_type = type{return_type_meta_object};
		auto interfaces = extract_interfaces(return_type);
		if (interfaces.contains(t))
			factory_methods.emplace_back(method);
	}

	if (factory_methods.size() == 1)
		return factory_methods.front();
	else
		return factory_method{};
}

}}

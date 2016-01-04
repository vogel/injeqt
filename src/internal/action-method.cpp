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

#include "action-method.h"

#include <injeqt/exception/invalid-action.h>
#include <injeqt/type.h>

#include <cassert>

namespace injeqt { namespace internal {

action_method::action_method()
{
}

action_method::action_method(QMetaMethod meta_method) :
	_object_type{meta_method.enclosingMetaObject()},
	_meta_method{std::move(meta_method)}
{
	assert(meta_method.methodType() == QMetaMethod::Slot);
	assert(meta_method.parameterCount() == 0);
	assert(meta_method.enclosingMetaObject() != nullptr);
}

bool action_method::is_empty() const
{
	return !_meta_method.isValid();
}

const type & action_method::object_type() const
{
	return _object_type;
}

const QMetaMethod & action_method::meta_method() const
{
	return _meta_method;
}

std::string action_method::signature() const
{
	return _meta_method.methodSignature().data();
}

bool action_method::invoke(QObject *on) const
{
	assert(!is_empty());
	assert(on != nullptr);
	assert(type{on->metaObject()} == _object_type);

	return _meta_method.invoke(on);
}

bool operator == (const action_method &x, const action_method &y)
{
	if (x.object_type() != y.object_type())
		return false;

	if (x.meta_method() != y.meta_method())
		return false;

	return true;
}

bool operator != (const action_method &x, const action_method &y)
{
	return !(x == y);
}

std::vector<action_method> extract_actions(const std::string &action_tag, const type &for_type)
{
	assert(!for_type.is_empty());

	auto result = std::vector<action_method>{};

	auto meta_object = for_type.meta_object();
	auto method_count = meta_object->methodCount();
	for (decltype(method_count) i = 0; i < method_count; i++)
	{
		auto probably_action = meta_object->method(i);
		auto method_tag = std::string{probably_action.tag()};
		if (action_tag != method_tag)
			continue;

		if (probably_action.methodType() != QMetaMethod::Slot)
			throw exception::invalid_action{std::string{"action is not a slot: "} + meta_object->className() + "::" + probably_action.methodSignature().data()};
		if (probably_action.parameterCount() != 0)
			throw exception::invalid_action{std::string{"invalid parameter count: "} + meta_object->className() + "::" + probably_action.methodSignature().data()};

		result.emplace_back(action_method{probably_action});
	}

	return result;
}

}}

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

#include "internal/interfaces-utils.h"

#include <cassert>

namespace injeqt { namespace internal {
	
bool action_method::is_action_init_tag(const std::string& tag)
{
	return tag == "INJEQT_INIT";
}

bool action_method::is_action_done_tag(const std::string& tag)
{
	return tag == "INJEQT_DONE";
}

bool action_method::validate_action_method(const QMetaMethod &meta_method)
{
	auto meta_object = meta_method.enclosingMetaObject();
	if (!meta_object)
		throw exception::invalid_action{std::string{"action does not have enclosing meta object: "} + "?::" + meta_method.methodSignature().data()};
	if (meta_method.methodType() == QMetaMethod::Signal)
		throw exception::invalid_action{std::string{"action is signal: "} + meta_object->className() + "::" + meta_method.methodSignature().data()};
	if (meta_method.methodType() == QMetaMethod::Constructor)
		throw exception::invalid_action{std::string{"action is constructor: "} + meta_object->className() + "::" + meta_method.methodSignature().data()};
	if (!is_action_init_tag(meta_method.tag()) && !is_action_done_tag(meta_method.tag()))
		throw exception::invalid_action{std::string{"action does not have valid tag: "} + meta_object->className() + "::" + meta_method.methodSignature().data()};
	if (meta_method.parameterCount() != 0)
		throw exception::invalid_action{std::string{"invalid parameter count: "} + meta_object->className() + "::" + meta_method.methodSignature().data()};

	return true;
}

action_method::action_method()
{
}

action_method::action_method(QMetaMethod meta_method) :
	_object_type{meta_method.enclosingMetaObject()},
	_meta_method{std::move(meta_method)}
{
	assert(validate_action_method(meta_method));
}

bool action_method::is_empty() const
{
	return !_meta_method.isValid();
}

const type & action_method::object_type() const
{
	return _object_type;
}

bool action_method::invoke(QObject *on) const
{
	assert(!is_empty());
	assert(on != nullptr);
	assert(implements(type{on->metaObject()}, _object_type));

	return _meta_method.invoke(on);
}

action_method make_action_method(const QMetaMethod &meta_method)
{
	action_method::validate_action_method(meta_method);

	return action_method{meta_method};
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
		if (action_tag == method_tag)
			result.emplace_back(make_action_method(probably_action));
	}

	return result;
}

}}

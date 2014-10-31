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

#include "dependencies.h"

#include <injeqt/exception/dependency-duplicated.h>
#include <injeqt/exception/dependency-on-self.h>
#include <injeqt/exception/dependency-on-subtype.h>
#include <injeqt/exception/dependency-on-supertype.h>
#include <injeqt/exception/exception.h>
#include <injeqt/exception/invalid-dependency.h>
#include <injeqt/exception/invalid-setter.h>
#include <injeqt/type.h>

#include "dependency.h"
#include "interfaces-utils.h"
#include "setter-method.h"
#include "type-relations.h"

#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>
#include <algorithm>
#include <cassert>
#include <set>

namespace injeqt { namespace internal {

namespace {

std::string exception_message(const QMetaObject *meta_object, const QMetaObject *dependency_meta_object)
{
	return std::string{meta_object->className()} + "::" + dependency_meta_object->className();
}

std::vector<setter_method> extract_setters(const types_by_name &known_types, const type &for_type)
{
	assert(!for_type.is_empty());

	auto result = std::vector<setter_method>{};

	auto meta_object = for_type.meta_object();
	auto method_count = meta_object->methodCount();
	for (decltype(method_count) i = 0; i < method_count; i++)
	{
		auto probably_setter = meta_object->method(i);
		auto tag = std::string{probably_setter.tag()};
		if (tag != "INJEQT_SETTER")
			continue;

		if (probably_setter.methodType() != QMetaMethod::Slot)
			continue;
		if (probably_setter.parameterCount() != 1)
			throw exception::invalid_setter{std::string{"invalid parameter count: "} + meta_object->className() + "::" + probably_setter.methodSignature().data()};
		auto parameter_type = type_by_pointer(known_types, probably_setter.parameterTypes()[0].data());
		if (parameter_type.is_empty() || parameter_type.is_qobject())
			throw exception::invalid_setter{std::string{"invalid parameter: "} + meta_object->className() + "::" + probably_setter.methodSignature().data()};

		result.emplace_back(setter_method{probably_setter});
	}

	return result;
}

std::vector<type> extract_parameter_types(const std::vector<setter_method> &setters)
{
	auto result = std::vector<type>{};

	std::transform(std::begin(setters), std::end(setters), std::back_inserter(result),
		[](const setter_method &setter){ return setter.parameter_type(); }
	);

	return result;
}

}

dependencies extract_dependencies(const types_by_name &known_types, const type &for_type)
{
	assert(!for_type.is_empty());

	auto interfaces = extract_interfaces(for_type);
	auto setters = extract_setters(known_types, for_type);
	for (auto &&setter : setters)
	{
		auto parameter_type = setter.parameter_type();
		if (parameter_type == for_type)
			throw exception::dependency_on_self{};
		if (std::find(std::begin(interfaces), std::end(interfaces), parameter_type) != std::end(interfaces))
			throw exception::dependency_on_supertype{};
		auto parameter_interfaces = extract_interfaces(parameter_type);
		if (std::find(std::begin(parameter_interfaces), std::end(parameter_interfaces), for_type) != std::end(parameter_interfaces))
			throw exception::dependency_on_subtype{};
	}

	auto parameter_types = extract_parameter_types(setters);
	auto relations = make_type_relations(parameter_types);

	auto matches = match(types{parameter_types}, relations.ambiguous());
	if (!matches.matched.empty())
		throw exception::dependency_duplicated{exception_message(for_type.meta_object(), matches.matched.begin()->first.meta_object())};

	auto result = std::vector<dependency>{};
	std::transform(std::begin(setters), std::end(setters), std::back_inserter(result),
		[](const setter_method &setter){ return dependency{setter}; }
	);

	return dependencies{result};
}

}}

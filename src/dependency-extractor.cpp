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

#include "dependency-extractor.h"

#include "dependency.h"
#include "implements-extractor.h"
#include "setter-method.h"
#include "type.h"
#include "type-relations.h"
#include "type-relations-factory.h"

#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>
#include <algorithm>
#include <set>

namespace injeqt { namespace v1 {

namespace {

std::string exception_message(const QMetaObject *meta_object, const QMetaObject *dependency_meta_object)
{
	return std::string{meta_object->className()} + "::" + dependency_meta_object->className();
}

std::vector<setter_method> extract_setters(const type &for_type)
{
	auto result = std::vector<setter_method>{};

	auto meta_object = for_type.meta_object();
	auto method_count = meta_object->methodCount();
	for (decltype(method_count) i = 0; i < method_count; i++)
	{
		auto probably_setter = meta_object->method(i);
		auto tag = std::string{probably_setter.tag()};
		if (tag != "injeqt_setter")
			continue;

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

dependencies dependency_extractor::extract_dependencies(const type &for_type) const
try
{
	auto setters = extract_setters(for_type);
	auto parameter_types = extract_parameter_types(setters);
	auto relations = type_relations_factory{}.create_type_relations(parameter_types);

	auto matches = match(types{parameter_types}, relations.ambiguous());
	if (!matches.matched.empty())
		throw dependency_duplicated_exception(exception_message(for_type.meta_object(), matches.matched.begin()->first.meta_object()));

	auto result = std::vector<dependency>{};
	std::transform(std::begin(setters), std::end(setters), std::back_inserter(result),
		[](const setter_method &setter){ return dependency{setter}; }
	);

	return dependencies{result};
}
catch (setter_too_many_parameters_exception &e)
{
	throw dependency_too_many_parameters_exception();
}
catch (invalid_setter_exception &e)
{
	throw dependency_not_qobject_exception();
}

}}

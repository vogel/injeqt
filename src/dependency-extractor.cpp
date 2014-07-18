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
#include "dependency-apply-method.h"
#include "implements-extractor.h"
#include "method.h"
#include "type.h"

#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>
#include <algorithm>
#include <set>

namespace injeqt { namespace v1 {

namespace {

std::string exception_message(const QMetaObject *meta_object, const QMetaMethod &method)
{
	return std::string{meta_object->className()} + "::" + method.methodSignature().data();
}

}

dependencies dependency_extractor::extract_dependencies(const type &for_type) const
{
	auto result = std::vector<dependency>{};
	auto used_dependencies = std::set<type>{};
	auto meta_object = for_type.meta_object();
	auto method_count = meta_object->methodCount();
	for (decltype(method_count) i = 0; i < method_count; i++)
	{
		auto probably_setter = meta_object->method(i);
		auto tag = std::string{probably_setter.tag()};
		if (tag != "injeqt_setter")
			continue;
		if (probably_setter.parameterCount() != 1)
			throw dependency_too_many_parameters_exception{exception_message(meta_object, probably_setter)};
		auto parameter_type = probably_setter.parameterType(0);
		auto parameter_meta_object = QMetaType::metaObjectForType(parameter_type);
		if (!parameter_meta_object)
			throw dependency_not_qobject_exception{exception_message(meta_object, probably_setter)};

		auto implements = implements_extractor{}.extract_implements(type{parameter_meta_object});
		auto used_dependencies_size = used_dependencies.size();
		auto implements_size = implements.size();
		used_dependencies.insert(std::begin(implements), std::end(implements));

		if (used_dependencies_size + implements_size != used_dependencies.size())
			throw dependency_duplicated_exception(exception_message(meta_object, probably_setter));

		result.emplace_back(type{parameter_meta_object}, dependency_apply_method::setter, method{probably_setter});
	}

	return dependencies{result};
}

}}

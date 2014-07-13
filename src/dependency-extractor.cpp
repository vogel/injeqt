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

#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>
#include <algorithm>
#include <set>

namespace injeqt { namespace v1 {

namespace {

std::string exception_message(const QMetaObject &meta_object, const QMetaMethod &method)
{
	return std::string{meta_object.className()} + "::" + method.methodSignature().data();
}

}

dependencies dependency_extractor::extract_dependencies(const QMetaObject &meta_object) const
{
	auto result = std::vector<dependency>{};
	auto used_dependencies = std::set<const QMetaObject *>{};
	auto method_count = meta_object.methodCount();
	for (decltype(method_count) i = 0; i < method_count; i++)
	{
		auto method = meta_object.method(i);
		auto tag = std::string{method.tag()};
		if (tag != "injeqt_setter")
			continue;
		if (method.parameterCount() != 1)
			throw dependency_too_many_parameters_exception{exception_message(meta_object, method)};
		auto parameter_type = method.parameterType(0);
		auto parameter_meta_object = QMetaType::metaObjectForType(parameter_type);
		if (!parameter_meta_object)
			throw dependency_not_qobject_exception{exception_message(meta_object, method)};

		auto implements = implements_extractor{}.extract_implements(*parameter_meta_object);
		auto used_dependencies_size = used_dependencies.size();
		auto implements_size = implements.size();
		used_dependencies.insert(std::begin(implements), std::end(implements));

		if (used_dependencies_size + implements_size != used_dependencies.size())
			throw dependency_duplicated_exception(exception_message(meta_object, method));

		result.emplace_back(parameter_meta_object, dependency_apply_method::setter, method);
	}

	return result;
}

}}

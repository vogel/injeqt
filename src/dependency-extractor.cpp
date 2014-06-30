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
#include "dependency-type.h"

#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>

namespace injeqt { namespace internal {

std::map<const QMetaObject *, dependency> dependency_extractor::extract_dependencies(const QMetaObject &meta_object) const
{
	auto result = std::map<const QMetaObject *, dependency>{};
	auto method_count = meta_object.methodCount();
	for (decltype(method_count) i = 0; i < method_count; i++)
	{
		auto method = meta_object.method(i);
		auto tag = std::string{method.tag()};
		if (tag != "injeqt_setter")
			continue;
		if (method.parameterCount() != 1)
			throw invalid_dependency{};
		auto parameter_type = method.parameterType(0);
		auto parameter_meta_object = QMetaType::metaObjectForType(parameter_type);
		if (!parameter_meta_object)
			throw invalid_dependency{};

		result.emplace(parameter_meta_object, dependency{dependency_type::setter, *parameter_meta_object, method});
	}

	return result;
}

}}

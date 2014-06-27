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

#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>

namespace injeqt { namespace details {

std::set<const QMetaObject *> dependency_extractor::extract_dependencies(const QMetaObject &metaObject) const
{
	auto result = std::set<const QMetaObject *>{};
	auto methodCount = metaObject.methodCount();
	for (decltype(methodCount) i = 0; i < methodCount; i++)
	{
		auto method = metaObject.method(i);
		auto tag = std::string{method.tag()};
		if (tag != "injeqt_setter")
			continue;
		if (method.parameterCount() != 1)
			continue; // todo: exception
		auto parameterType = method.parameterType(0);
		auto metaObject = QMetaType::metaObjectForType(parameterType);
		if (!metaObject)
			continue; // todo: exception
		result.insert(metaObject);
	}

	return result;
}

}}

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

#include "dependency-resolver.h"

#include "dependency.h"
#include "dependency-type.h"
#include "implements-extractor.h"

namespace injeqt { namespace v1 {

const std::map<const QMetaObject *, dependency> dependency_resolver::resolve_dependencies(
	QObject &object,
	const std::map<const QMetaObject *, dependency> &to_resolve,
	const std::vector<QObject *> &dependencies) const
{
	auto result = std::map<const QMetaObject *, dependency>{};
	for (auto &&to_resolve_item : to_resolve)
		if (!resolve_dependency(object, to_resolve_item.first, to_resolve_item.second, dependencies))
			result.insert(to_resolve_item);

	return result;
}

bool dependency_resolver::resolve_dependency(
	QObject &object,
	const QMetaObject *type,
	const dependency &to_resolve,
	const std::vector<QObject *> &dependencies) const
{
	if (to_resolve.type() != dependency_type::setter)
		return false;

	for (auto &&dependency : dependencies)
	{
		auto implements = implements_extractor{}.extract_implements(*dependency->metaObject());
		if (implements.find(type) != std::end(implements))
		{
			to_resolve.setter_method().invoke(&object, Q_ARG(QObject *, dependency));
			return true;
		}
	}

	return false;
}

}}

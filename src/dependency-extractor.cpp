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
try
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

		auto setter = setter_method{probably_setter};
		auto implements = implements_extractor{}.extract_implements(setter.parameter_type());
		for (auto &&i : implements)
			if (used_dependencies.find(i) != std::end(used_dependencies))
				throw dependency_duplicated_exception(exception_message(meta_object, probably_setter));

		used_dependencies.insert(setter.parameter_type());

		result.emplace_back(std::move(setter));
	}

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

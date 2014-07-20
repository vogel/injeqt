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

#include "resolved-dependency-applicator.h"

#include "object-with-meta.h"

namespace injeqt { namespace v1 {

namespace {

std::string exception_message_2(const QMetaMethod &method)
{
	return std::string{method.methodSignature().data()};
}

std::string exception_message(const QMetaObject *meta_object, const setter_method &method)
{
	return std::string{meta_object->className()} + "::" + method.signature();
}

std::string exception_message(const QMetaObject *meta_object1, const QMetaObject *meta_object2)
{
	return std::string{meta_object1->className()} + " - " + meta_object2->className();
}

}

resolved_dependency_applicator::resolved_dependency_applicator(std::vector<resolved_dependency> resolved_dependencies) :
	_resolved_dependencies{std::move(resolved_dependencies)}
{
	for (auto &&resolved : _resolved_dependencies)
	{
		if (resolved.object().meta().main_type() != resolved.resolved().required_type())
			throw applicator_invalid_dependency_exception{};
		if (!resolved.object().object())
			throw applicator_invalid_dependency_exception{};

		auto method = resolved.resolved().setter().meta_method();
		if (method.parameterCount() != 1)
			throw applicator_invalid_setter_exception{exception_message_2(method)};
		if (!method.enclosingMetaObject())
			throw applicator_invalid_setter_exception{exception_message_2(method)};
		auto parameter_type = method.parameterType(0);
		auto parameter_meta_object = QMetaType::metaObjectForType(parameter_type);
		if (!parameter_meta_object)
			throw applicator_invalid_setter_exception{exception_message_2(method)};

		if (resolved.object().meta().main_type() != type{parameter_meta_object})
			throw applicator_non_matching_setter_exception{exception_message(resolved.object().object()->metaObject(), parameter_meta_object)};
	}
}

void resolved_dependency_applicator::apply_on(object_with_meta &object)
{
	for (auto &&resolved : _resolved_dependencies)
	{
		auto setter = resolved.resolved().setter();
		if (object.meta().main_type() != setter.object_type())
			throw applicator_non_matching_setter_exception{exception_message(object.meta().main_type().meta_object(), setter)};
		if (!setter.invoke(object, resolved.object()))
			throw applicator_failed_exception{exception_message(object.meta().main_type().meta_object(), setter)};
	}
}

}}

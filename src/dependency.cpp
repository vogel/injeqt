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

#include "dependency.h"

#include <QtCore/QMetaMethod>
#include <functional>
#include <string>
#include <type_traits>

namespace injeqt { namespace v1 {

dependency::dependency(dependency_apply_method apply_method, const QMetaObject &type, QMetaMethod setter_method) :
	_apply_method{apply_method},
	_type(type),
	_setter_method{std::move(setter_method)}
{
}

dependency_apply_method dependency::apply_method() const
{
	return _apply_method;
}

const QMetaObject & dependency::type() const
{
	return _type;
}

QMetaMethod dependency::setter_method() const
{
	return _setter_method;
}

bool operator == (const dependency &first, const dependency &second)
{
	if (std::addressof(first) == std::addressof(second))
		return true;

	if (first.apply_method() != second.apply_method())
		return false;

	if (std::addressof(first.type()) != std::addressof(second.type()))
		return false;

	if (first.setter_method() != second.setter_method())
		return false;

	return true;
}

bool operator != (const dependency &first, const dependency &second)
{
	return !(first == second);
}

}}

namespace std {

size_t hash<injeqt::v1::dependency>::operator() (const injeqt::v1::dependency &dependency) const
{
	using dependency_apply_method_type = underlying_type<injeqt::v1::dependency_apply_method>::type;
	auto dependency_hash = hash<dependency_apply_method_type>{};
	auto string_hash = hash<string>{};
	auto h = 0xcbf29ce484222325ULL;
	h = (h ^ dependency_hash(static_cast<dependency_apply_method_type>(dependency.apply_method()))) * 0x100000001b3ULL;
	h = (h ^ string_hash(string{dependency.type().className()})) * 0x100000001b3ULL;
	h = (h ^ string_hash(string{dependency.setter_method().methodSignature().constData()})) * 0x100000001b3ULL;
	return h;
}

}

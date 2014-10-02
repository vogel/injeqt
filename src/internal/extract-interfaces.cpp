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

#include "extract-interfaces.h"

#include "type.h"

#include <QtCore/QMetaObject>
#include <cassert>

namespace injeqt { namespace internal {

namespace
{

bool is_qobject(const QMetaObject * const meta_object)
{
	return !meta_object->superClass();
}

}

types extract_interfaces(const type &for_type)
{
	assert(!for_type.is_empty());

	auto result = std::vector<type>{};
	auto meta_object = for_type.meta_object();
	while (meta_object && !is_qobject(meta_object))
	{
		result.emplace_back(meta_object);
		meta_object = meta_object->superClass();
	}

	return types{result};
}

bool implements(const type &implementation, const type &interface)
{
	assert(!implementation.is_empty());
	assert(!interface.is_empty());

	auto interfaces = extract_interfaces(implementation);
	return std::find(std::begin(interfaces), std::end(interfaces), interface) != std::end(interfaces);
}

}}

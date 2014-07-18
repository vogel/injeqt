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

#include "implements-extractor.h"

#include "type.h"

#include <QtCore/QMetaObject>

namespace injeqt { namespace v1 {

namespace
{

bool is_qobject(const QMetaObject * const meta_object)
{
	return !meta_object->superClass();
}

}

std::set<type> implements_extractor::extract_implements(const type &for_type) const
{
	auto result = std::set<type>{};
	auto meta_object = for_type.meta_object();
	while (meta_object && !is_qobject(meta_object))
	{
		result.insert(type{meta_object});
		meta_object = meta_object->superClass();
	}

	return result;
}

}}

/*
 * %injeqt copyright begin%
 * Copyright 2016 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "instantiate-type.h"

#include <injeqt/exception/invalid-instantiate-type.h>

#include <QtCore/QMetaClassInfo>
#include <QtCore/QMetaObject>

namespace injeqt { namespace internal {

instantiate_type get_instantiate_type(type for_type)
{
	auto result = instantiate_type::on_demand;
	auto meta_object = for_type.meta_object();
	auto class_info_count = meta_object->classInfoCount();
	for (decltype(class_info_count) i = 0; i < class_info_count; i++)
	{
		auto class_info = meta_object->classInfo(i);
		if (std::string{INJEQT_INSTANTIATE_CLASSINFO_NAME} == class_info.name())
		{
			auto value = std::string{class_info.value()};
			if (value == INJEQT_INSTANTIATE_IMMEDIATE_CLASSINFO_VALUE)
				result = instantiate_type::immediate;
			else if (value == INJEQT_INSTANTIATE_ON_DEMAND_CLASSINFO_VALUE)
				result = instantiate_type::on_demand;
			else
				throw exception::invalid_instantiate_type{value};
		}
	}

	return result;
}

}}

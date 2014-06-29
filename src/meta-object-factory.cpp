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

#include "meta-object-factory.h"

#include "dependency-extractor.h"
#include "implements-extractor.h"
#include "meta-object.h"

namespace injeqt { namespace details {

meta_object meta_object_factory::create_meta_object(QMetaObject *metaObject) const
{
	return
	{
		metaObject,
		implements_extractor{}.extract_implements(*metaObject),
		dependency_extractor{}.extract_dependencies(*metaObject)
	};
}

}}

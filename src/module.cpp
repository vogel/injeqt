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

#include "module.h"

#include "meta-object.h"
#include "meta-object-factory.h"

#include <QtCore/QMetaObject>

using namespace injeqt::internal;

namespace injeqt { namespace v1 {

void module::add_class(const QMetaObject &meta_object)
{
	_meta_objects.push_back(meta_object_factory{}.create_meta_object(meta_object));
}

const std::vector<meta_object> & module::meta_objects() const
{
	return _meta_objects;
}

}}

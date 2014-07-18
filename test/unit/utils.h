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

#pragma once

#include "meta-object-factory.h"
#include "object-with-meta.h"

#include <QtCore/QObject>

namespace injeqt { namespace v1 {

template<typename T>
object_with_meta make_object_with_meta()
{
	auto qobject = std::unique_ptr<QObject>(new T{});
	auto object = object_with_meta
	{
		meta_object_factory{}.create_meta_object(type{std::addressof(T::staticMetaObject)}),
		std::move(qobject)
	};

	return object;
}

}}

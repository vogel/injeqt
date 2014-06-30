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

#include "dependency.h"
#include "injeqt-global.h"

#include <map>
#include <set>

class QMetaObject;

namespace injeqt { namespace internal {

class meta_object final
{

public:
	meta_object(const QMetaObject *type, std::set<const QMetaObject *> implements, std::map<const QMetaObject *, dependency> dependencies);

	const QMetaObject * type() const;
	std::set<const QMetaObject *> implements() const;
	std::map<const QMetaObject *, dependency> dependencies() const;

private:
	const QMetaObject * _type;
	std::set<const QMetaObject *> _implements;
	std::map<const QMetaObject *, dependency> _dependencies;

};

}}

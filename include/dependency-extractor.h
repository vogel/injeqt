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

#include "dependencies.h"
#include "injeqt-exception.h"
#include "injeqt-global.h"

#include <QtCore/QMetaObject>
#include <exception>
#include <map>

namespace injeqt { namespace v1 {

DEFINE_EXCEPTION(dependency_exception, injeqt_exception);
DEFINE_EXCEPTION(dependency_too_many_parameters_exception, dependency_exception);
DEFINE_EXCEPTION(dependency_not_qobject_exception, dependency_exception);
DEFINE_EXCEPTION(dependency_duplicated_exception, dependency_exception);

class dependency_extractor final
{

public:
	dependencies extract_dependencies(const QMetaObject &meta_object) const;

};

}}

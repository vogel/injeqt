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

#pragma once

#include <injeqt/injeqt.h>

#include "implementation.h"
#include "internal.h"

/**
 * @file
 * @brief Contains classes and functions for defining provider/implementation pair.
 */

class QObject;

namespace injeqt { namespace internal {

class provider;

/**
 * @brief Connects implementation objet with provider that created it.
 */
class INJEQT_INTERNAL_API provided_object final
{

public:
	/**
	 * @brief Create new instance.
	 * @param interface_type type that object implements
	 * @param object object that should implement interface_type
	 * @pre provider != nullptr
	 * @pre !provider->provided_type().is_empty()
	 * @pre !provider->provided_type().is_qobject()
	 * @pre provider->provided_type() == object->implementation_type()
	 */
	explicit provided_object(provider *provided_by, implementation object);

	provider * provided_by() const;
	implementation object() const;

private:
	provider *_provided_by;
	implementation _object;

};

INJEQT_INTERNAL_API bool operator == (const provided_object &x, const provided_object &y);
INJEQT_INTERNAL_API bool operator != (const provided_object &x, const provided_object &y);

}}

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

#include "exception/exception.h"
#include "injeqt.h"
#include "type.h"

/**
 * @file
 * @brief Contains classes and functions for representing "interface is implemented by type" relations.
 */

namespace injeqt { namespace internal {

/**
 * @brief Any exception that can occur when validating or creating implemented_by objects.
 */
INJEQT_EXCEPTION(invalid_implemented_by_exception, ::injeqt::v1::exception::exception);

/**
 * @brief Represents "interface is implemented by type" relations.
 *
 * In Injeqt dependencies between objects are described by types. When an object of some type
 * T is added to injector using module class then it can be passed as a dependency of type
 * T or any of T supertypes - excluding only supertypes common with other configured objects.
 * This class represens such relation. It is mostly used in implemnted_by_mapping set to
 * represents all types that injector knows and can handle.
 *
 * Call validate(const implemented_by &) to check if object is valid.
 */
class implemented_by final
{

public:
	/**
	 * @brief Create new instance of implemented_by.
	 * @param interface_type type of available interface
	 * @param implementation_type type of object implementing interface
	 *
	 * To check if @p implementation_type really implements @p interface_type call
	 * validate(const implemented_by &).
	 */
	explicit implemented_by(type interface_type, type implementation_type);

	/**
	 * @return type of available interface.
	 */
	type interface_type() const;

	/**
	 * @return type of object implementing interface.
	 */
	type implementation_type() const;

private:
	type _interface_type;
	type _implementation_type;

};

/**
 * @brief Check if implemented_by object is valid.
 * @param ib object to check
 * @throw invalid_type_exception when interface_type is invalid
 * @throw invalid_type_exception when implementation_type is invalid
 * @throw invalid_implemented_by_exception if implementation_type does not implement interface_type
 */
void validate(const implemented_by &ib);

bool operator == (const implemented_by &x, const implemented_by &y);
bool operator != (const implemented_by &x, const implemented_by &y);

}}

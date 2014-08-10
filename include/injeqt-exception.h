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

#include "injeqt.h"

#include <exception>
#include <string>

namespace injeqt { namespace v1 {

class INJEQT_API injeqt_exception : public std::exception
{

public:
	explicit injeqt_exception(std::string what = std::string{}) : _what{std::move(what)} {}
	virtual ~injeqt_exception() {}

	virtual const char * what() const noexcept override { return _what.c_str(); }

private:
	std::string _what;

};

#define DEFINE_EXCEPTION(name, parent) \
class INJEQT_API name : public parent \
{ \
 \
public: \
	explicit name(std::string what = std::string{}) : parent{std::move(what)} {} \
	virtual ~name() {} \
 \
};


}}

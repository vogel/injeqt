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

#include "injeqt-exception.h"
#include "injeqt-global.h"
#include "resolved-dependency.h"

namespace injeqt { namespace v1 {

class object_with_meta;

DEFINE_EXCEPTION(applicator_exception, injeqt_exception);
DEFINE_EXCEPTION(applicator_invalid_dependency_exception, applicator_exception);
DEFINE_EXCEPTION(applicator_invalid_setter_exception, applicator_exception);
DEFINE_EXCEPTION(applicator_non_matching_setter_exception, applicator_exception);
DEFINE_EXCEPTION(applicator_failed_exception, applicator_exception);

class resolved_dependency_applicator final
{

public:
	resolved_dependency_applicator(std::vector<resolved_dependency> resolved_dependencies);

	void apply_on(object_with_meta &object);

private:
	std::vector<resolved_dependency> _resolved_dependencies;

};

}}

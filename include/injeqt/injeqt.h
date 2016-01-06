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

#include <QtCore/QtGlobal>

#ifdef injeqt_EXPORTS
#define INJEQT_API Q_DECL_EXPORT
#else
#define INJEQT_API Q_DECL_IMPORT
#endif

#ifndef Q_MOC_RUN
#  define INJEQT_INIT
#  define INJEQT_DONE
#  define INJEQT_SET
// depreceated, use INJEQT_SET instead
#  define INJEQT_SETTER
#endif

#define INJEQT_INSTANTIATE_CLASSINFO_NAME "injeqt.instantiate"
#define INJEQT_INSTANTIATE_ON_DEMAND_CLASSINFO_VALUE "on-demand"
#define INJEQT_INSTANTIATE_IMMEDIATE_CLASSINFO_VALUE "immediate"
#define INJEQT_INSTANCE_ON_DEMAND Q_CLASSINFO(INJEQT_INSTANTIATE_CLASSINFO_NAME, INJEQT_INSTANTIATE_ON_DEMAND_CLASSINFO_VALUE)
#define INJEQT_INSTANCE_IMMEDIATE Q_CLASSINFO(INJEQT_INSTANTIATE_CLASSINFO_NAME, INJEQT_INSTANTIATE_IMMEDIATE_CLASSINFO_VALUE)

namespace injeqt {
	namespace v1 { }
	using namespace v1;
}

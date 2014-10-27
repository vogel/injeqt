#
# %injeqt copyright begin%
# Copyright 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
# %injeqt copyright end%
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#

set (INJEQT_SRCS
	injector.cpp
	module.cpp
	type.cpp

	exception/ambiguous-types.cpp
	exception/default-constructor-not-found.cpp
	exception/dependency-duplicated.cpp
	exception/dependency-on-self.cpp
	exception/dependency-on-subtype.cpp
	exception/dependency-on-supertype.cpp
	exception/empty-type.cpp
	exception/exception.cpp
	exception/instantiation-failed.cpp
	exception/interface-not-implemented.cpp
	exception/invalid-dependency.cpp
	exception/invalid-qobject.cpp
	exception/invalid-setter.cpp
	exception/qobject-type.cpp
	exception/unavailable-required-types.cpp
	exception/unknown-type.cpp
	exception/unique-factory-method-not-found.cpp
	exception/unresolvable-dependencies.cpp

	internal/default-constructor-method.cpp
	internal/dependencies.cpp
	internal/dependency.cpp
	internal/factory-method.cpp
	internal/implementation.cpp
	internal/implemented-by.cpp
	internal/injector-core.cpp
	internal/injector-impl.cpp
	internal/interfaces-utils.cpp
	internal/module-impl.cpp
	internal/provider-by-default-constructor.cpp
	internal/provider-by-factory.cpp
	internal/provider-ready.cpp
	internal/required-to-instantiate.cpp
	internal/resolved-dependency.cpp
	internal/resolve-dependencies.cpp
	internal/setter-method.cpp
	internal/type-dependencies.cpp
	internal/type-relations.cpp
	internal/types-model.cpp
)

add_library (
	injeqt
	SHARED
	${INJEQT_SRCS}
)

set_target_properties (injeqt PROPERTIES
	SOVERSION "${INJEQT_SOVERSION}"
	VERSION "${INJEQT_VERSION}"
)

message ("aa ${CMAKE_INSTALL_LIBDIR}")
install (TARGETS
	injeqt
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

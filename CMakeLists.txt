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

project (injeqt)
cmake_minimum_required (VERSION 2.8)

find_package (Qt5Core 5.2 REQUIRED)
set (CMAKE_AUTOMOC TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories (
	${Qt5Core_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/src/internal
)

set (WARNINGS "-W -Wall -Wextra -Wundef -Wunused -Wuninitialized -Wcast-align -Wpointer-arith -Woverloaded-virtual -Wnon-virtual-dtor -fno-common")
set (VISIBILITY "-fvisibility=hidden -fvisibility-inlines-hidden")
set (CMAKE_CXX_FLAGS "-std=c++11 ${WARNINGS} ${VISIBILITY}")

set (INJEQT_SRCS
	src/injector.cpp
	src/module.cpp
	src/type.cpp

	src/exception/ambiguous-types.cpp
	src/exception/default-constructor-not-found.cpp
	src/exception/dependency-duplicated.cpp
	src/exception/dependency-on-self.cpp
	src/exception/dependency-on-subtype.cpp
	src/exception/dependency-on-supertype.cpp
	src/exception/empty-type.cpp
	src/exception/exception.cpp
	src/exception/instantiation-failed.cpp
	src/exception/interface-not-implemented.cpp
	src/exception/invalid-dependency.cpp
	src/exception/invalid-qobject.cpp
	src/exception/invalid-setter.cpp
	src/exception/qobject-type.cpp
	src/exception/unavailable-required-types.cpp
	src/exception/unknown-type.cpp
	src/exception/unique-factory-method-not-found.cpp
	src/exception/unresolvable-dependencies.cpp

	src/internal/default-constructor-method.cpp
	src/internal/dependencies.cpp
	src/internal/dependency.cpp
	src/internal/factory-method.cpp
	src/internal/implementation.cpp
	src/internal/implemented-by.cpp
	src/internal/injector-core.cpp
	src/internal/injector-impl.cpp
	src/internal/interfaces-utils.cpp
	src/internal/module-impl.cpp
	src/internal/provider-by-default-constructor.cpp
	src/internal/provider-by-factory.cpp
	src/internal/provider-ready.cpp
	src/internal/required-to-instantiate.cpp
	src/internal/resolved-dependency.cpp
	src/internal/resolve-dependencies.cpp
	src/internal/setter-method.cpp
	src/internal/type-dependencies.cpp
	src/internal/type-relations.cpp
	src/internal/types-model.cpp
)

add_subdirectory (examples)

enable_testing ()
add_subdirectory (test)

add_library (
	injeqt
	SHARED
	${INJEQT_SRCS}
)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
	set (CMAKE_INSTALL_LIBDIR lib)
endif (NOT DEFINED CMAKE_INSTALL_LIBDIR)

install (TARGETS
	injeqt
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install (DIRECTORY
	include/injeqt DESTINATION include/injeqt
)

set (prefix ${CMAKE_INSTALL_PREFIX})
set (exec_prefix ${CMAKE_INSTALL_PREFIX})
set (libdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set (includedir ${CMAKE_INSTALL_PREFIX}/include)
set (LIBS "-lQt5Core")

configure_file (
	${CMAKE_CURRENT_SOURCE_DIR}/injeqt.pc.in
	${CMAKE_CURRENT_BINARY_DIR}/injeqt.pc
	@ONLY
)

install (FILES
	${CMAKE_CURRENT_BINARY_DIR}/injeqt.pc
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

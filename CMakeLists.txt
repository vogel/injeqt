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

project(injeqt)
cmake_minimum_required(VERSION 2.8)

find_package(Qt5Core 5.2 REQUIRED)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
	${Qt5Core_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(WARNINGS "-Wall -Wextra -Wundef -Wunused -Wuninitialized -Wcast-align -Wpointer-arith -Woverloaded-virtual -Wnon-virtual-dtor -fno-common")
set(VISIBILITY "-fvisibility=hidden -fvisibility-inlines-hidden")
set(CMAKE_CXX_FLAGS "-std=c++11 ${VISIBILITY}")

set(INJEQT_SRCS
	src/class-mapping.cpp
	src/dependencies.cpp
	src/dependency.cpp
	src/dependency-extractor.cpp
	src/dependency-resolver.cpp
	src/implements-extractor.cpp
	src/injeqt-object.cpp
	src/meta-object.cpp
	src/meta-object-factory.cpp
	src/module.cpp
	src/resolved-dependency.cpp
	src/resolved-dependency-applicator.cpp
	src/setter-injector.cpp
)

enable_testing()
add_subdirectory(test)

add_library(
	injeqt
	SHARED
	${INJEQT_SRCS}
)

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
	set(CMAKE_INSTALL_LIBDIR lib)
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)

install(TARGETS
	injeqt
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY
	include/ DESTINATION include/injeqt
)

set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(libdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(includedir ${CMAKE_INSTALL_PREFIX}/include)
set(LIBS "-lQt5Core")
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/injeqt.pc.in
	${CMAKE_CURRENT_BINARY_DIR}/injeqt.pc
	@ONLY
)
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/injeqt.pc
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

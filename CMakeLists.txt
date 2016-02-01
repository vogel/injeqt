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

set (INJEQT_SOVERSION "1")
set (INJEQT_VERSION "1.0.100")

option (DISABLE_TESTS "Do not build tests" OFF)
option (DISABLE_EXAMPLES "Do not build examples" OFF)

find_package (Qt5Core 5.2 REQUIRED)

# do not link with qtmain on windows
if (POLICY CMP0020)
	cmake_policy (SET CMP0020 OLD)
endif ()
if (POLICY CMP0043)
	cmake_policy (SET CMP0043 NEW)
endif ()

set (WARNINGS "-Werror -W -Wall -Wextra -Wundef -Wunused -Wuninitialized -Wcast-align -Wpointer-arith -Woverloaded-virtual -Wnon-virtual-dtor -fno-common")
set (VISIBILITY "-fvisibility=hidden -fvisibility-inlines-hidden")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${WARNINGS} ${VISIBILITY}")

set (CMAKE_AUTOMOC TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/src/internal
)

add_subdirectory (src)

if (NOT DISABLE_EXAMPLES)
	add_subdirectory (examples)
endif (NOT DISABLE_EXAMPLES)

if (NOT CMAKE_BUILD_TYPE MATCHES RELEASE AND NOT DISABLE_TESTS)
	enable_testing ()
	add_subdirectory (test)
endif (NOT CMAKE_BUILD_TYPE MATCHES RELEASE AND NOT DISABLE_TESTS)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
	set (CMAKE_INSTALL_LIBDIR lib)
endif (NOT DEFINED CMAKE_INSTALL_LIBDIR)

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

install (DIRECTORY
	include/injeqt DESTINATION include
)

install (FILES
	${CMAKE_CURRENT_BINARY_DIR}/injeqt.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

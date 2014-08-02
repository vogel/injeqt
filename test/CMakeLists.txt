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

include_directories(
	${CMAKE_SOURCE_DIR}/src
)

find_package(Qt5Test 5.2 REQUIRED)

function (injeqt_add_test name_)
	file (RELATIVE_PATH sourcePath "${CMAKE_SOURCE_DIR}/test" "${CMAKE_CURRENT_SOURCE_DIR}")

	get_filename_component (exeDir "${CMAKE_CURRENT_BINARY_DIR}/${name_}" PATH)
	file (MAKE_DIRECTORY "${exeDir}")

	add_executable (${name_} ${ARGN})
	set_property (TARGET ${name_} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-error")
	qt5_use_modules (${name_} Core Test)

	add_test ("${sourcePath}/${name_}" ${name_})
endfunction ()

injeqt_add_test(extract-dependencies-test unit/extract-dependencies-test.cpp)
injeqt_add_test(extract-interfaces-test unit/extract-interfaces-test.cpp)
injeqt_add_test(implementation-test unit/implementation-test.cpp)
injeqt_add_test(implemented-by-test unit/implemented-by-test.cpp)
injeqt_add_test(instantiation-state-test unit/instantiation-state-test.cpp)
injeqt_add_test(required-to-instantiate-test unit/required-to-instantiate-test.cpp)
injeqt_add_test(resolved-dependency-test unit/resolved-dependency-test.cpp)
injeqt_add_test(resolve-dependencies-test unit/resolve-dependencies-test.cpp)
injeqt_add_test(setter-method-test unit/setter-method-test.cpp)
injeqt_add_test(sorted-unique-vector-test unit/sorted-unique-vector-test.cpp)
injeqt_add_test(type-test unit/type-test.cpp)
injeqt_add_test(type-relations-test unit/type-relations-test.cpp)

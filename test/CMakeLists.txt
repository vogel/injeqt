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

find_package (Qt5Test 5.2 REQUIRED)

option (DISABLE_COVERAGE "Do not gather coverage data" OFF)

include_directories (
	${CMAKE_SOURCE_DIR}/src
)

if (NOT DISABLE_COVERAGE)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage -O0")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage -O0")
	set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -O0")

	add_custom_target (coverage
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/coverage.sh
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif (NOT DISABLE_COVERAGE)

function (injeqt_add_test name_)
	file (RELATIVE_PATH sourcePath "${CMAKE_SOURCE_DIR}/test" "${CMAKE_CURRENT_SOURCE_DIR}")

	get_filename_component (exeDir "${CMAKE_CURRENT_BINARY_DIR}/${name_}" PATH)
	file (MAKE_DIRECTORY "${exeDir}")

	add_executable (${name_} ${ARGN})
	set_property (TARGET ${name_} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-error")
	qt5_use_modules (${name_} Core Test)

	if (NOT DISABLE_COVERAGE)
		target_link_libraries (${name_} gcov)
	endif (NOT DISABLE_COVERAGE)

	add_test ("${sourcePath}/${name_}" ${name_})
endfunction ()

function (injeqt_add_unit_test name_)
	injeqt_add_test (${ARGV})
	target_link_libraries (${name_} injeqt)
endfunction ()

function (injeqt_add_integration_test name_)
	injeqt_add_test (${ARGV})
	target_link_libraries (${name_} injeqt)
endfunction ()

injeqt_add_unit_test (default-constructor-method-test unit/default-constructor-method-test.cpp)
injeqt_add_unit_test (dependencies-test unit/dependencies-test.cpp)
injeqt_add_unit_test (dependency-test unit/dependency-test.cpp)
injeqt_add_unit_test (factory-method-test unit/factory-method-test.cpp)
injeqt_add_unit_test (implementation-test unit/implementation-test.cpp)
injeqt_add_unit_test (implemented-by-test unit/implemented-by-test.cpp)
injeqt_add_unit_test (injector-core-test unit/injector-core-test.cpp)
injeqt_add_unit_test (injector-test unit/injector-test.cpp)
injeqt_add_unit_test (interfaces-utils-test unit/interfaces-utils-test.cpp)
injeqt_add_unit_test (module-impl-test unit/module-impl-test.cpp)
injeqt_add_unit_test (module-test unit/module-test.cpp)
injeqt_add_unit_test (provider-by-default-constructor-test unit/provider-by-default-constructor-test.cpp)
injeqt_add_unit_test (provider-by-default-constructor-configuration-test unit/provider-by-default-constructor-configuration-test.cpp)
injeqt_add_unit_test (provider-by-factory-test unit/provider-by-factory-test.cpp)
injeqt_add_unit_test (provider-by-factory-configuration-test unit/provider-by-factory-configuration-test.cpp)
injeqt_add_unit_test (provider-ready-test unit/provider-ready-test.cpp)
injeqt_add_unit_test (provider-ready-configuration-test unit/provider-ready-configuration-test.cpp)
injeqt_add_unit_test (required-to-satisfy-test unit/required-to-satisfy-test.cpp)
injeqt_add_unit_test (resolved-dependency-test unit/resolved-dependency-test.cpp)
injeqt_add_unit_test (resolve-dependencies-test unit/resolve-dependencies-test.cpp)
injeqt_add_unit_test (setter-method-test unit/setter-method-test.cpp)
injeqt_add_unit_test (sorted-unique-vector-test unit/sorted-unique-vector-test.cpp)
injeqt_add_unit_test (type-dependencies-test unit/type-dependencies-test.cpp)
injeqt_add_unit_test (type-test unit/type-test.cpp)
injeqt_add_unit_test (type-relations-test unit/type-relations-test.cpp)
injeqt_add_unit_test (types-by-name-test unit/types-by-name-test.cpp)
injeqt_add_unit_test (types-model-test unit/types-model-test.cpp)

injeqt_add_integration_test (default-constructor-behavior-test integration/default-constructor-behavior-test.cpp)
injeqt_add_integration_test (factory-behavior-test integration/factory-behavior-test.cpp)
injeqt_add_integration_test (inject-into-behavior-test integration/inject-into-behavior-test.cpp)
injeqt_add_integration_test (ready-object-behavior-test integration/ready-object-behavior-test.cpp)

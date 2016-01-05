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

function (injeqt_add_test name file)
	file (RELATIVE_PATH sourcePath "${CMAKE_SOURCE_DIR}/test" "${CMAKE_CURRENT_SOURCE_DIR}")

	get_filename_component (exeDir "${CMAKE_CURRENT_BINARY_DIR}/${name}" PATH)
	file (MAKE_DIRECTORY "${exeDir}")

	add_executable (${name} ${file})
	set_property (TARGET ${name} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-error")
	qt5_use_modules (${name} Core Test)

	if (NOT DISABLE_COVERAGE)
		target_link_libraries (${name} gcov)
	endif (NOT DISABLE_COVERAGE)

	add_test ("${sourcePath}/${name}" ${name})
endfunction ()

function (injeqt_add_unit_test name)
	injeqt_add_test (${name} unit/${name}.cpp)
	target_link_libraries (${name} injeqt)
endfunction ()

function (injeqt_add_integration_test name)
	injeqt_add_test (${name} integration/${name}.cpp)
	target_link_libraries (${name} injeqt)
endfunction ()

set (UNIT_TESTS
	action-method-test
	default-constructor-method-test
	dependencies-test
	dependency-test
	factory-method-test
	implementation-test
	implemented-by-test
	injector-core-test
	injector-test
	interfaces-utils-test
	module-impl-test
	module-test
	provider-by-default-constructor-test
	provider-by-default-constructor-configuration-test
	provider-by-factory-test
	provider-by-factory-configuration-test
	provider-ready-test
	provider-ready-configuration-test
	required-to-satisfy-test
	resolved-dependency-test
	resolve-dependencies-test
	setter-method-test
	sorted-unique-vector-test
	type-dependencies-test
	type-test
	type-relations-test
	types-by-name-test
	types-model-test
)

set (INTEGRATION_TESTS
	default-constructor-behavior-test
	factory-behavior-test
	init-done-test
	inject-into-behavior-test
	ready-object-behavior-test
)

foreach (UNIT_TEST ${UNIT_TESTS})
	injeqt_add_unit_test (${UNIT_TEST})
endforeach ()

foreach (INTEGRATION_TEST ${INTEGRATION_TESTS})
	injeqt_add_integration_test (${INTEGRATION_TEST})
endforeach ()

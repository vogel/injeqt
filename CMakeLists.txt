project(injeqt)
cmake_minimum_required(VERSION 2.8)

find_package(Qt5Core 5.2 REQUIRED)

set(INJEQT_SRCS
	src/setter-injector.cpp
)

add_library(
	injeqt
	SHARED
	${INJEQT_SRCS}
)

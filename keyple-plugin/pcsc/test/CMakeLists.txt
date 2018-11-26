#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

#
# GoogleTest
#
# We need thread support
find_package(Threads REQUIRED)

configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
    )

if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(
    COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
    )

if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL
                 )


#
# Test Files
#
file(GLOB SRCS *.cpp)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}

	${CMAKE_CURRENT_SOURCE_DIR}/../src
	${CMAKE_CURRENT_SOURCE_DIR}/../src/smartcardio

	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/util

    /usr/include/PCSC
)

IF(APPLE)
	find_library(PCSC PCSC)
ENDIF(APPLE)

IF(UNIX)
	find_library(PCSC pcsclite)
ENDIF(UNIX)

IF(WIN32)
	find_libray(PCSC pcsclite)
ENDIF(WIN32)

if(NOT PCSC)
	message(FATAL_ERROR "PC/SC framework/library not found")
endif()

add_executable(keyplepluginpcsctest ${SRCS})
target_link_libraries(keyplepluginpcsctest keyplepluginpcsc gtest gmock ${PCSC})

add_test(name PcscTest command testPcscPlugin)

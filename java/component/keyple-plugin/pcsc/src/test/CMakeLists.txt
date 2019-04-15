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
FIND_PACKAGE(Threads REQUIRED)

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)

CONFIGURE_FILE(CMakeLists.txt.in googletest-download/CMakeLists.txt)
EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
)

IF(result)
    MESSAGE(FATAL_ERROR "CMake step for googletest failed: ${result}")
ENDIF()

EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
)

IF(result)
    MESSAGE(FATAL_ERROR "Build step for googletest failed: ${result}")
ENDIF()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
ADD_SUBDIRECTORY(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL
)

#
# Test Files
#
FILE(GLOB SRCS *.cpp)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}

    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-smartcardio/src/main/java/org/eclipse/keyple/smartcardio

    # core
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/util
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/transaction

    # common
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-common/src/main/java/org/eclipse/keyple/common

    # pcsc plugin
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc

    # pcsc (Linux / macOS)
    /usr/include/PCSC
)

IF(APPLE)
	FIND_LIBRARY(PCSC PCSC)
ENDIF(APPLE)

IF(UNIX)
	FIND_LIBRARY(PCSC pcsclite)
ENDIF(UNIX)

IF(WIN32)
	SET(PCSC pcsclite)
ENDIF(WIN32)

IF(NOT PCSC)
	MESSAGE(FATAL_ERROR "PC/SC framework/library not found")
ENDIF()

ADD_EXECUTABLE(keyplepluginpcsctest

    ${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/pcsc/PcscPluginTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/pcsc/SmartCardIOReaderTest.cpp
)

TARGET_LINK_LIBRARIES(keyplepluginpcsctest keyplepluginpcsc gtest gmock ${PCSC})

#ADD_TEST(name PcscTest command testPcscPlugin)

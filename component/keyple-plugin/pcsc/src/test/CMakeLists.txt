#******************************************************************************
#  Copyright (c) 2020 Calypso Networks Association                            *
#  https://www.calypsonet-asso.org/                                           *
#                                                                             *
#  See the NOTICE file(s) distributed with this work for additional           *
#  information regarding copyright ownership.                                 *
#                                                                             *
#  This program and the accompanying materials are made available under the   *
#  terms of the Eclipse Public License 2.0 which is available at              *
#  http://www.eclipse.org/legal/epl-2.0                                       *
#                                                                             *
#  SPDX-License-Identifier: EPL-2.0                                           *
# *****************************************************************************/

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)
PROJECT(TU C CXX)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}

    # core
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/plugin
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/plugin/local
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/exception
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/event
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/message
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/protocol
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/util
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/transaction

    # common
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-common/src/main
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-common/src/main/exception

    # pcsc plugin
    ${CMAKE_CURRENT_SOURCE_DIR}/../main

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
	SET(CMAKE_FIND_LIBRARY_PREFIXES "")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dll")
	SET(CMAKE_BUILD_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../../../../..")
	SET(PCSC winscard.lib)
ENDIF(WIN32)

IF(NOT PCSC)
	MESSAGE(FATAL_ERROR "PC/SC framework/library not found")
ENDIF()

ADD_EXECUTABLE(keyplepluginpcsc_ut

    ${CMAKE_CURRENT_SOURCE_DIR}/MainTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PcscReaderImplTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PcscTerminalTest.cpp
)

TARGET_LINK_LIBRARIES(keyplepluginpcsc_ut keyplepluginpcsc gtest gmock ${PCSC})

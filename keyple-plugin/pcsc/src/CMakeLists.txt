#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

PROJECT(KeypleCore C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "keyplepluginpcsc")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

# Common definitions
ADD_DEFINITIONS(-DK_KPL_PLUGIN_PCSC_SUPPORT -DK_KPL_OS_MEMORY_DEBUG -D_CRT_SECURE_NO_WARNINGS)

# OS Specific definitions
IF(UNIX)
ADD_DEFINITIONS(-DK_KPL_OS_PLATFORM=K_KPL_OS_PLATFORM_LINUX -std=c++11)
ENDIF(UNIX)

IF(WIN32)
ADD_DEFINITIONS(-DK_KPL_OS_PLATFORM=K_KPL_OS_PLATFORM_WIN32)
ENDIF(WIN32)

message(STATUS "base dir: ${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/smartcardio

    ${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/seproxy
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/seproxy/event
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/seproxy/exception
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/seproxy/plugin
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/util

    /usr/include/PCSC
)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
	  message(STATUS "dir='${dir}'")
endforeach()

add_library(keyplepluginpcsc SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/PcscPlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PcscReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/smartcardio/Card.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/smartcardio/CardChannel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/smartcardio/CardTerminal.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/smartcardio/CommandAPDU.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/smartcardio/ResponseAPDU.cpp
)


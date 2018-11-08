#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#
PROJECT(SeProxyService C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "SeProxyService")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

# Common definitions
ADD_DEFINITIONS(-DK_KPL_PCSC_PLUGIN_SUPPORT -DK_OS_KPL_MEMORY_DEBUG -D_CRT_SECURE_NO_WARNINGS)

# OS Specific definitions
IF(UNIX)
ADD_DEFINITIONS(-DK_OS_KPL_PLATFORM=K_OS_KPL_PLATFORM_LINUX -std=c++11)
ENDIF(UNIX)

IF(WIN32)
ADD_DEFINITIONS(-DK_OS_KPL_PLATFORM=K_OS_KPL_PLATFORM_WIN32)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}../../keyple-core/src/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}../../keyple-core/src/seproxy/interfaces
	${CMAKE_CURRENT_SOURCE_DIR}../../keyple-core/src/seproxy/containers
	${CMAKE_CURRENT_SOURCE_DIR}../../keyple-core/src/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}../../keyple-core/src/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}../../keyple-core/src/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}../../keyple-core/src/util
	${CMAKE_CURRENT_SOURCE_DIR}../../keyple-core/src/command
	${CMAKE_CURRENT_SOURCE_DIR}../../keyple-core/src/transaction
) 

# SeProxyService library
add_library(SeProxyService SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/SeProxyService.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/seproxy/SeProxyService.def
)

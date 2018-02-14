#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

PROJECT(PcscPlugin C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "PcscPlugin")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

# Common definitions
ADD_DEFINITIONS(-DK_KPL_PCSC_PLUGIN_SUPPORT -DK_OS_KPL_MEMORY_DEBUG -D_CRT_SECURE_NO_WARNINGS)

# OS Specific definitions
IF(UNIX)
ADD_DEFINITIONS(-DK_OS_KPL_PLATFORM=K_OS_KPL_PLATFORM_LINUX -std=c++11 -pthread)
ENDIF(UNIX)
 
IF(WIN32) 
ADD_DEFINITIONS(-DK_OS_KPL_PLATFORM=K_OS_KPL_PLATFORM_WIN32)
ENDIF(WIN32)

# enable standard output log (debug)
ADD_DEFINITIONS(-DSTANDARD_OUTPUT_LOG)

INCLUDE_DIRECTORIES(/usr/include/PCSC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../keyple-core/interfaces ${CMAKE_CURRENT_SOURCE_DIR}/../../keyple-core/interfaces/containers ) 

# PcscPlugin library
add_library(PcscPlugin SHARED
# add OS utils
# add CPP plugin classes
	${CMAKE_CURRENT_SOURCE_DIR}/PcscReader.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/PcscPlugin.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/PcscPlugin.def
)

IF(WIN32)
target_link_libraries(PcscPlugin winscard)
ENDIF(WIN32)

IF(UNIX)
target_link_libraries(PcscPlugin pcsclite pthread)
ENDIF(UNIX)

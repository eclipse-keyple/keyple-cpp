#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

PROJECT(KeyplePluginPcsc C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "KeyplePluginPcsc")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)

SET(LIBRARY_NAME keyplepluginpcsc)

IF(WIN32)
	ADD_COMPILE_OPTIONS(/wd4275 /wd4251 /wd4290 /wd4250)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple

	# Core
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/core/util
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/core/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/core/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/core/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/core/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/core/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/core/seproxy/protocol
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/core/transaction

	# Helpers
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-common/src/main/java/org/eclipse/keyple/common
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-smartcardio/src/main/java/org/eclipse/keyple/smartcardio

	# pcsc (Linux / macOS) 
	/usr/include/PCSC	
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED

	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/pcsc/package-info.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/pcsc/PcscProtocolSetting.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/pcsc/PcscReader.cpp
)

IF(WIN32)
	SET(CMAKE_FIND_LIBRARY_PREFIXES "")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dll")
	SET(CMAKE_BUILD_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../../../../..")
	SET(WINSCARD winscard.lib)
ENDIF(WIN32)

IF(APPLE)
	SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
	SET(CMAKE_BUILD_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../../../../..")
	SET(WINSCARD "-framework PCSC")
ENDIF(APPLE)

#FIND_LIBRARY(KEYPLECORE
#	NAMES keyplecore
#	PATHS ${CMAKE_BUILD_DIRECTORY}
#	PATH_SUFFIXES bin)

SET(LINK_LIBS ${WINSCARD} keyplecore keyplesmartcardio)

TARGET_LINK_LIBRARIES(keyplepluginpcsc ${LINK_LIBS})

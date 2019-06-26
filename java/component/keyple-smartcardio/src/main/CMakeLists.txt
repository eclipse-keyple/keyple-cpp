#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

PROJECT(KeypleSmartcardio C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "KeypleSmartcardio")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)

SET(LIBRARY_NAME keyplesmartcardio)

IF(WIN32)
	ADD_COMPILE_OPTIONS(/wd4275 /wd4251)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}

	# Helpers
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-common/src/main/java/org/eclipse/keyple/common

	# Core
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/java/org/eclipse/keyple/core/util
	
	# PC/SC (Linux / macOS) 
	/usr/include/PCSC	
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED

	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/smartcardio/ATR.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/smartcardio/Card.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/smartcardio/CardChannel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/smartcardio/CardTerminal.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/smartcardio/CardTerminals.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/smartcardio/CommandAPDU.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/smartcardio/ResponseAPDU.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/smartcardio/TerminalFactory.cpp
)

IF(WIN32)
	SET(CMAKE_FIND_LIBRARY_PREFIXES "")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dll")
	SET(CMAKE_BUILD_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../../../../..")
	SET(WINSCARD winscard.lib)
	SET(PTHREAD "")
ENDIF(WIN32)

IF(APPLE)
	SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
	SET(CMAKE_BUILD_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../../../../..")
	SET(WINSCARD "-framework PCSC")
	SET(PTHREAD pthread)
ENDIF(APPLE)

If(UNIX)
	SET(PTHREAD pthread)
ENDIF(UNIX)

SET(LINK_LIBS ${WINSCARD} keyplecore ${PTHREAD})

TARGET_LINK_LIBRARIES(keyplesmartcardio ${LINK_LIBS})

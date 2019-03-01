#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

PROJECT(KeyplePluginStub C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "KeyplePluginStub")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)

SET(LIBRARY_NAME keyplepluginstub)

IF(WIN32)
	ADD_COMPILE_OPTIONS(/wd4275 /wd4251)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple

	# Core
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/util
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol

	# Helpers
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-common/src/main/java/org/eclipse/keyple/common
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-smartcardio/src/main/java/org/eclipse/keyple/smartcardio
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED

	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/stub/StubPlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/stub/StubProtocolSetting.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/stub/StubReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/stub/StubSecureElement.cpp
)

SET(LINK_LIBS ${WINSCARD} keyplecore)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${LINK_LIBS})

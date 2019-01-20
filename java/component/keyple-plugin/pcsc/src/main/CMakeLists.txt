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

SET(LIBRARY_NAME libkeyplepluginpcsc)

IF(WIN32)
	ADD_COMPILE_OPTIONS(/wd4275 /wd4251)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple

	/* Helpers */
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-common/src/main/java/org/eclipse/keyple/common
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED

	# seproxy - event
	#${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/pcsc/package-info.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/pcsc/PcscProtocolSetting.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/plugin/pcsc/PcscReader.cpp
)

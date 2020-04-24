#******************************************************************************
#* Copyright (c) 2018 Calypso Networks Association                            *
#* https://www.calypsonet-asso.org/                                           *
#*                                                                            *
#* See the NOTICE file(s) distributed with this work for additional           *
#* information regarding copyright ownership.                                 *
#*                                                                            *
#* This program and the accompanying materials are made available under the   *
#* terms of the Eclipse Public License 2.0 which is available at              *
#* http://www.eclipse.org/legal/epl-2.0                                       *
#*                                                                            *
#* SPDX-License-Identifier: EPL-2.0                                           *
#******************************************************************************/

PROJECT(KeypleCommon C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "KeypleCommon")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)

SET(LIBRARY_NAME keyplecommon)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DKEYPLECOMMON_EXPORT")

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED

	${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LoggerFactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Matcher.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Pattern.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Properties.cpp
)

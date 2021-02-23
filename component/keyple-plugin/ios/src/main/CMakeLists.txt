#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

PROJECT(KeyplePluginIOS OBJCXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "9")
SET(VERSION_PATCH "1")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "KeyplePluginIOS")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)

SET(LIBRARY_NAME keyplepluginios)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DKEYPLEPLUGINIOS_EXPORT")

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}

    # Core
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/util
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/event
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/exception
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/message
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/plugin
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/plugin/local
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/plugin/local/state
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/plugin/local/monitoring
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/protocol
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/transaction

    # Helpers
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-common/src/main
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-common/src/main/exception
)

ADD_LIBRARY(
    ${LIBRARY_NAME}

    ${LIBRARY_TYPE}

    ${CMAKE_CURRENT_SOURCE_DIR}/IOSPlugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IOSPluginFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IOSPluginImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IOSProtocolSetting.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IOSReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IOSReaderImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IOSTerminal.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/IOSTerminalImpl.mm
)

SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
SET(CMAKE_BUILD_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../../../../..")

FIND_LIBRARY(FOUNDATION Foundation)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} keyplecore ${FOUNDATION})

SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "YES")

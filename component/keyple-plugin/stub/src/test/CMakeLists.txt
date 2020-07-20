#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)
PROJECT(TU C CXX)

IF(WIN32)
	ADD_COMPILE_OPTIONS(/wd4275 /wd4251 /wd4290)
ENDIF(WIN32)


INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/eclipse/keyple
	${CMAKE_CURRENT_SOURCE_DIR}/eclipse/keyple/plugin/stub
	${CMAKE_CURRENT_SOURCE_DIR}/eclipse/keyple/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/eclipse/keyple/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/eclipse/keyple/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/eclipse/keyple/core/seproxy/plugin

	# Common
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-common/src/main
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-common/src/main/exception

	# Core
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/util
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/seproxy/protocol
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/transaction
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/selection
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/transaction
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-core/src/main/command

	# Plugin - stub
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../keyple-plugin/stub/src/main/

	# google test and mock
	${GOOGLETEST_DIRECTORY}/googletest-src
	${GOOGLETEST_DIRECTORY}/googletest-src/googlemock/include
	${GOOGLETEST_DIRECTORY}/googletest-src/googletest/include
)

ADD_EXECUTABLE(keyplepluginstub_ut
	# TU Stub
	${CMAKE_CURRENT_SOURCE_DIR}/MainTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StubPluginTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/StubReaderTest.cpp

	#${CMAKE_CURRENT_SOURCE_DIR}/StubPlugin.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/StubProtocolSetting.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/StubReader.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/StubSecureElement.cpp
)

SET(LINK_LIBS gtest gmock keyplepluginstub keyplecore ${WINSCARD} )

TARGET_LINK_LIBRARIES(keyplepluginstub_ut ${LINK_LIBS})

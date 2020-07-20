#******************************************************************************
#  Copyright (c) 2020 Calypso Networks Association                            *
#  https://www.calypsonet-asso.org/                                           *
#                                                                             *
#  See the NOTICE file(s) distributed with this work for additional           *
#  information regarding copyright ownership.                                 *
#                                                                             *
#  This program and the accompanying materials are made available under the   *
#  terms of the Eclipse Public License 2.0 which is available at              *
#  http://www.eclipse.org/legal/epl-2.0                                       *
#                                                                             *
#  SPDX-License-Identifier: EPL-2.0                                           *
# *****************************************************************************/

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)
PROJECT(TU C CXX)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/keyple
	${CMAKE_CURRENT_SOURCE_DIR}/keyple-plugin/stub
	${CMAKE_CURRENT_SOURCE_DIR}/keyple-core/src/main/seproxy/plugin

	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/util
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/monitoring
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/state
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/protocol
	${CMAKE_CURRENT_SOURCE_DIR}/command
	${CMAKE_CURRENT_SOURCE_DIR}/transaction

	# common
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-common/src/main
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-common/src/main/exception

	# TLV
	${CMAKE_CURRENT_SOURCE_DIR}/../main/util/bertlv

	# Core
	${CMAKE_CURRENT_SOURCE_DIR}/../main/util
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../main/selection
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/plugin/local
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/plugin/local/monitoring
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/plugin/local/state
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/protocol

	# google test and mock
	${GOOGLETEST_DIRECTORY}/googletest-src
	${GOOGLETEST_DIRECTORY}/googletest-src/googlemock/include
	${GOOGLETEST_DIRECTORY}/googletest-src/googletest/include
)

ADD_EXECUTABLE(keyplecore_ut

	# command
	${CMAKE_CURRENT_SOURCE_DIR}/command/AbstractApduCommandBuilderTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/AbstractApduResponseParserTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/AbstractIso7816CommandBuilderTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/SeCommandTest.cpp

	# selection
	${CMAKE_CURRENT_SOURCE_DIR}/selection/AbstractMatchingSeTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/selection/AbstractSeSelectionRequestTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/selection/SelectionsResultTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/selection/SeResourceTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/selection/SeSelectionTest.cpp

	# seproxy - event
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event/AbstractDefaultSelectionsRequestTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event/AbstractDefaultSelectionsResponseTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event/ObservablePluginTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event/ObservableReaderTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event/PluginEventTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event/ReaderEventTest.cpp

	# seproxy - message
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/AnswerToReset.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/ApduRequestTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/ApduResponseTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/DefaultSelectionsRequestTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/DefaultSelectionsResponseTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/ProxyReaderTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/SelectionStatusTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/SeRequestTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/SeResponseTest.cpp

	# util - bertlv
	${CMAKE_CURRENT_SOURCE_DIR}/util/bertlv/TLVTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/util/bertlv/TagTest.cpp

	# TU keyple util
	${CMAKE_CURRENT_SOURCE_DIR}/util/ByteArrayUtilTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/util/CountDownLatchTest.cpp

	# seproxy - plugin - local - monitoring

	# seproxy - plugin - local - state

	# seproxy - plugin - local
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/AbstractLocalReaderSelectionTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/AbstractLocalReaderTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/AbstractLocalReaderTransmitTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/AbstractObservableLocalReaderTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/AbstractObservableStateTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/AbstractSmartInsertionThreadedReaderTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/AbstractSmartPresenceThreadedReaderTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/ObservableReaderStateServiceTest.cpp

	# seproxy - plugin
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractObservablePluginTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractPluginTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractReaderTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractSeProxyComponentTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractThreadedObservablePluginTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/ObservablePluginNotifierTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/ObservableReaderNotifierTest.cpp

	# seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/ReaderPluginTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/SeProxyServiceTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/SeReaderTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/SeSelectorTest.cpp

	# seproxy - protocol
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/protocol/SeCommonProtocolsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/protocol/SeProtocolTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/protocol/TransmissionModeTest.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/MainTest.cpp
)

SET(LINK_LIBS gtest gmock keyplecore ${WINSCARD} )

TARGET_LINK_LIBRARIES(keyplecore_ut ${LINK_LIBS})

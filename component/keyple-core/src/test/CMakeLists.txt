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

	# calypso
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-calypso/src/main
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-calypso/src/main/command
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-calypso/src/main/command/po
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-calypso/src/main/command/po/parser

	# common
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-common/src/main

	# TLV
	${CMAKE_CURRENT_SOURCE_DIR}/../main/util/bertlv

	# Core
	${CMAKE_CURRENT_SOURCE_DIR}/../main/util
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command
	${CMAKE_CURRENT_SOURCE_DIR}/../main/selection
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/plugin/local
	${CMAKE_CURRENT_SOURCE_DIR}/../main/seproxy/protocol

	# google test and mock
	${GOOGLETEST_DIRECTORY}/googletest-src
	${GOOGLETEST_DIRECTORY}/googletest-src/googlemock/include
	${GOOGLETEST_DIRECTORY}/googletest-src/googletest/include
)

ADD_EXECUTABLE(keyple_core_ut

	# command
	${CMAKE_CURRENT_SOURCE_DIR}/command/AbstractApduCommandBuilderTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/AbstractApduResponseParserTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/AbstractIso7816CommandBuilderTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/CommandsTableTest.cpp

	# selection
	${CMAKE_CURRENT_SOURCE_DIR}/selection/AbstractMatchingSeTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/selection/AbstractSeSelectionRequestTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/selection/MatchingSelectionTest.cpp
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
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/ProxyReader.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/SelectionStatus.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/SeRequestTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/SeResponseTest.cpp

	# TU keyple util - bertlv
	${CMAKE_CURRENT_SOURCE_DIR}/MainTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/util/TLVTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/util/TagTest.cpp

	# TU keyple util
	${CMAKE_CURRENT_SOURCE_DIR}/util/ConfigurableTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/util/ByteArrayUtilTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/util/CountDownLatchTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/util/NameableTest.cpp

	# seproxy
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/ChannelControl.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/ReaderPlugin.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/SeProxyServiceTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/SeReader.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/SeSelector.cpp

	# seproxy - plugin
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractLocalReader.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractLoggedObservable.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractObservablePlugin.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractObservableReaderTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractSelectionLocalReader.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractStaticPlugin.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractStaticReader.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractThreadedLocalReader.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractThreadedObservablePlugin.cpp

	# seproxy - protocol
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/protocol/SeCommonProtocolsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/protocol/SeProtocolTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/protocol/TransmissionModeTest.cpp

	#${CMAKE_CURRENT_SOURCE_DIR}/CoreBaseTest.cpp
)

SET(LINK_LIBS gtest gmock keyplecore keyplecalypso ${WINSCARD} )

TARGET_LINK_LIBRARIES(keyple_core_ut ${LINK_LIBS})

#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

PROJECT(KeypleCore C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "KeypleCore")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)

SET(LIBRARY_NAME keyplecore)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
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

	/* Helpers */
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-common/src/main
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED

	# command
	${CMAKE_CURRENT_SOURCE_DIR}/command/AbstractApduCommandBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/AbstractApduResponseParser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/AbstractIso7816CommandBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/command/CommandsTable.cpp

	# selection
	${CMAKE_CURRENT_SOURCE_DIR}/selection/AbstractMatchingSe.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/selection/AbstractSeSelectionRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/selection/MatchingSelection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/selection/SelectionsResult.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/selection/SeResource.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/selection/SeSelection.cpp

	# seproxy - event
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event/AbstractDefaultSelectionsRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event/AbstractDefaultSelectionsResponse.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event/ObservablePlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event/ObservableReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event/PluginEvent.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/event/ReaderEvent.cpp

	# seproxy - exception
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/exception/KeypleReaderException.cpp

	# seproxy - message
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/AnswerToReset.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/ApduRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/ApduResponse.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/DefaultSelectionsRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/DefaultSelectionsResponse.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/ProxyReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/SelectionStatus.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/SeRequest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/message/SeResponse.cpp

    # seproxy - plugin - local - monitoring
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/monitoring/CardAbsentPingMonitoringJob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/monitoring/SmartInsertionMonitoringJob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/monitoring/SmartRemovalMonitoringJob.cpp

	# seproxy - plugin - local - state
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/state/WaitForSeInsertion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/state/WaitForSeProcessing.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/state/WaitForSeRemoval.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/state/WaitForStartDetect.cpp

    # seproxy - plugin - local
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/AbstractLocalReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/AbstractObservableLocalReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/AbstractObservableState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/MonitoringPool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/local/ObservableReaderStateService.cpp

	# seproxy - plugin
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractPlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/seproxy/plugin/AbstractThreadedObservablePlugin.cpp

	# seproxy - protocol
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/protocol/SeCommonProtocols.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/protocol/SeProtocol.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/protocol/TransmissionMode.cpp

	# seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/ChannelControl.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/ReaderPlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/ReaderPoolPlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/SeProxyService.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/SeReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/seproxy/SeSelector.cpp

	# util
	${CMAKE_CURRENT_SOURCE_DIR}/util/ByteArrayUtil.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/util/Configurable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/util/Nameable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/util/Observable.cpp

	# util - bertlv
	${CMAKE_CURRENT_SOURCE_DIR}/util/bertlv/Tag.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/util/bertlv/TLV.cpp
)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} keyplecommon)

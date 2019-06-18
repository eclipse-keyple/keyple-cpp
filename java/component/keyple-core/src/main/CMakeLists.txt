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

IF(WIN32)
	ADD_COMPILE_OPTIONS(/wd4275 /wd4251 /wd4290)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/util
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/protocol
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/command
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/transaction

	/* Helpers */
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-common/src/main/java/org/eclipse/keyple/common
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED

	# command
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/command/AbstractApduCommandBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/command/AbstractApduResponseParser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/command/AbstractIso7816CommandBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/command/CommandsTable.cpp

	# seproxy - event
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/event/DefaultSelectionRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/event/ObservablePlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/event/ObservableReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/event/PluginEvent.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/event/ReaderEvent.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/event/SelectionResponse.cpp

	# seproxy - exception
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/exception/KeypleApplicationSelectionException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/exception/KeypleBaseException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/exception/KeypleChannelStateException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/exception/KeypleIOReaderException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/exception/KeyplePluginException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/exception/KeyplePluginNotFoundException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/exception/KeypleReaderException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/exception/KeypleReaderNotFoundException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/exception/KeypleRuntimeException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/exception/NoStackTraceThrowable.cpp

	# seproxy - message
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/message/AnswerToReset.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/message/ApduRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/message/ApduResponse.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/message/ProxyReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/message/SelectionStatus.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/message/SeRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/message/SeRequestSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/message/SeResponse.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/message/SeResponseSet.cpp

	# seproxy - plugin
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/plugin/AbstractLocalReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/plugin/AbstractLoggedObservable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/plugin/AbstractObservablePlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/plugin/AbstractObservableReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/plugin/AbstractSelectionLocalReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/plugin/AbstractStaticPlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/plugin/AbstractStaticReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/plugin/AbstractThreadedLocalReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/plugin/AbstractThreadedObservablePlugin.cpp

	# seproxy - protocol
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/protocol/ContactlessProtocols.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/protocol/ContactsProtocols.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/protocol/Protocol.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/protocol/SeProtocol.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/protocol/SeProtocolSetting.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/protocol/SeProtocolSettingList.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/protocol/TransmissionMode.cpp

	# seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/ChannelState.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/ReaderPlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/SeProxyService.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/SeReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/seproxy/SeSelector.cpp

	# transaction
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/transaction/MatchingSe.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/transaction/SeSelection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/transaction/SeSelectionRequest.cpp

	# util
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/util/ByteArrayUtils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/util/Configurable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/util/Nameable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/core/util/Observable.cpp
)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} keyplecommon)

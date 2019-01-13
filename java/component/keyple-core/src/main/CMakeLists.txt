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

SET(LIBRARY_NAME libkeyplecore)

IF(WIN32)
	ADD_COMPILE_OPTIONS(/wd4275 /wd4251)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/util
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/protocol
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/command
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/transaction

	/* Helpers */
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../common
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED

	# seproxy - event
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/event/ObservablePlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/event/ObservableReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/event/PluginEvent.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/event/ReaderEvent.cpp

	# seproxy - exception
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/exception/KeypleApplicationSelectionException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/exception/KeypleChannelStateException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/exception/KeypleIOReaderException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/exception/KeyplePluginException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/exception/KeyplePluginNotFoundException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/exception/KeypleRuntimeException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.cpp

	# seproxy - message
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/message/AnswerToReset.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/message/ApduRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/message/ApduResponse.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/message/ProxyReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/message/SelectionStatus.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/message/SeRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/message/SeRequestSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/message/SeResponse.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/message/SeResponseSet.cpp

	# seproxy - plugin
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/plugin/AbstractLocalReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/plugin/AbstractLoggedObservable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/plugin/AbstractObservablePlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/plugin/AbstractObservableReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/plugin/AbstractSelectionLocalReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/plugin/AbstractStaticPlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/plugin/AbstractStaticReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/plugin/AbstractThreadedLocalReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/plugin/AbstractThreadedObservablePlugin.cpp

	# seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/ChannelState.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/ReaderPlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/SeProxyService.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/seproxy/SeReader.cpp

	# util
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/util/ByteArrayUtils.cpp

	# helpers
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../common/Arrays.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../common/Pattern.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../common/Logger.cpp
)

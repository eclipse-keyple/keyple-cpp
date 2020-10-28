#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

# Generic includes for all examples
INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}

	# Example
	${CMAKE_CURRENT_SOURCE_DIR}/calypso
	${CMAKE_CURRENT_SOURCE_DIR}/common
	${CMAKE_CURRENT_SOURCE_DIR}/common/calypso
	${CMAKE_CURRENT_SOURCE_DIR}/common/calypso/pc/transaction
	${CMAKE_CURRENT_SOURCE_DIR}/common/calypso/postructure
	${CMAKE_CURRENT_SOURCE_DIR}/common/calypso/stub
	${CMAKE_CURRENT_SOURCE_DIR}/common/generic
	${CMAKE_CURRENT_SOURCE_DIR}/common/generic/stub
	${CMAKE_CURRENT_SOURCE_DIR}/generic

	# Core
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-core/src/main/command
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-core/src/main/command/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-core/src/main/selection
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-core/src/main/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-core/src/main/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-core/src/main/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-core/src/main/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-core/src/main/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-core/src/main/seproxy/protocol
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-core/src/main/util
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-core/src/main/util/bertlv

	# Plugin
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-plugin/stub/src/main
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-plugin/pcsc/src/main

	# Common
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-common/src/main
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-common/src/main/exception

	# Calypso
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-calypso/src/main
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-calypso/src/main/command
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-calypso/src/main/command/po
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-calypso/src/main/command/po/builder
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-calypso/src/main/command/po/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-calypso/src/main/command/po/parser
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-calypso/src/main/command/sam
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-calypso/src/main/command/sam/builder
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-calypso/src/main/command/sam/parser
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-calypso/src/main/transaction
	${CMAKE_CURRENT_SOURCE_DIR}/../component/keyple-calypso/src/main/transaction/exception

	# pcsc (Linux / macOS)
	/usr/include/PCSC
)

# Add projects
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/generic/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/calypso/)

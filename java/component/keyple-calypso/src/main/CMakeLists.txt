#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

PROJECT(KeypleCalypso C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "KeypleCalypso")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)

SET(LIBRARY_NAME keyplecalypso)

IF(WIN32)
	ADD_COMPILE_OPTIONS(/wd4275 /wd4251 /wd4290)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/security
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/security
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/builder
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/builder/security
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/parser
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/parser/security
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/transaction
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/transaction/exception

	/* Core */
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/java/org/eclipse/keyple
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/java/org/eclipse/keyple/util
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/java/org/eclipse/keyple/command
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/java/org/eclipse/keyple/transaction
	

	/* Helpers */
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-common/src/main/java/org/eclipse/keyple/common
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED

	# calypso - command
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/PoClass.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/SendableInSession.cpp

	# calypso - command - po
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/CalypsoPoCommands.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/PoCommandBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/PoCustomModificationCommandBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/PoCustomReadCommandBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/PoModificationCommand.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/PoRevision.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/PoSendableInSession.cpp

	# calypso - command - po - builder
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/AppendRecordCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/DecreaseCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/GetDataFciCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/IncreaseCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/ReadRecordsCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/SelectFileCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/UpdateRecordCmdBuild.cpp

	# calypso - command - po - builder - security
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/security/AbstractOpenSessionCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/security/CloseSessionCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/security/OpenSession10CmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/security/OpenSession24CmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/security/OpenSession31CmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/security/OpenSession32CmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/builder/security/PoGetChallengeCmdBuild.cpp

	# calypso - command - po - parser
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/AppendRecordRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/DecreaseRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/GetDataFciRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/IncreaseRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/ReadDataStructure.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/ReadRecordsRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/SelectFileRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/UpdateRecordRespPars.cpp

	# calypso - command - po - parser - security
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/security/AbstractOpenSessionRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/security/CloseSessionRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/security/OpenSession10RespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/security/OpenSession24RespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/security/OpenSession31RespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/security/OpenSession32RespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/po/parser/security/PoGetChallengeRespPars.cpp

	# calypso - command - sam
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/CalypsoSamCommands.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/SamCommandBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/SamRevision.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/SamSendableInSession.cpp

	# calypso - command - sam - builder - security
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/builder/security/DigestAuthenticateCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/builder/security/DigestCloseCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/builder/security/DigestInitCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/builder/security/DigestUpdateCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/builder/security/DigestUpdateMultipleCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/builder/security/SamGetChallengeCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/builder/security/SelectDiversifierCmdBuild.cpp

	# calypso - command - sam - parser - security
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/parser/security/DigestAuthenticateRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/parser/security/DigestCloseRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/parser/security/DigestInitRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/parser/security/DigestUpdateMultipleRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/parser/security/DigestUpdateRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/parser/security/SamGetChallengeRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/command/sam/parser/security/SelectDiversifierRespPars.cpp

	# calypso - transaction
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/transaction/CalypsoPo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/transaction/PoSelectionRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/transaction/PoSelector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/transaction/PoTransaction.cpp

	# calypso - transaction - exception
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/transaction/exception/KeypleCalypsoSecureSessionException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/transaction/exception/KeypleCalypsoSecureSessionUnauthorizedKvcException.cpp

	# calypso - transaction - sam
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/transaction/sam/CalypsoSam.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/transaction/sam/SamSelectionRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/java/org/eclipse/keyple/calypso/transaction/sam/SamSelector.cpp
)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} keyplecore)

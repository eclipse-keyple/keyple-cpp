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
	${CMAKE_CURRENT_SOURCE_DIR}/command
	${CMAKE_CURRENT_SOURCE_DIR}/command/po
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security
	${CMAKE_CURRENT_SOURCE_DIR}/transaction
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/exception

	/* Core */
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/util
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/util/bertlv
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/seproxy
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/seproxy/event
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/seproxy/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/seproxy/message
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/seproxy/plugin
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/seproxy/protocol
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/command
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/selection


	/* Helpers */
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-common/src/main
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED

	# calypso - command
	${CMAKE_CURRENT_SOURCE_DIR}/command/PoClass.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/SendableInSession.cpp

	# calypso - command - po
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/AbstractPoCommandBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/AbstractPoResponseParser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/CalypsoPoCommands.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoBuilderParser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoCustomModificationCommandBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoCustomReadCommandBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoModificationCommand.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoRevision.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoSendableInSession.cpp

	# calypso - command - po - builder
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/AppendRecordCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/DecreaseCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/GetDataFciCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/IncreaseCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/ReadRecordsCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/SelectFileCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/UpdateRecordCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/WriteRecordCmdBuild.cpp

	# calypso - command - po - builder - security
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/AbstractOpenSessionCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/ChangeKeyCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/CloseSessionCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/OpenSession10CmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/OpenSession24CmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/OpenSession31CmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/OpenSession32CmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/PoGetChallengeCmdBuild.cpp

	# calypso - command - po - parser
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/AppendRecordRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/DecreaseRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/GetDataFciRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/IncreaseRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/ReadDataStructure.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/ReadRecordsRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/SelectFileRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/UpdateRecordRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/WriteRecordRespPars.cpp


	# calypso - command - po - parser - security
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/AbstractOpenSessionRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/ChangeKeyRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/CloseSessionRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/OpenSession10RespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/OpenSession24RespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/OpenSession31RespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/OpenSession32RespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/PoGetChallengeRespPars.cpp

	# calypso - command - sam
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/AbstractSamCommandBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/AbstractSamResponseParser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/CalypsoSamCommands.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/SamBuilderParser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/SamRevision.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/SamSendableInSession.cpp

	# calypso - command - sam - builder - security
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/CardGenerateKeyCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/DigestAuthenticateCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/DigestCloseCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/DigestInitCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/DigestUpdateCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/DigestUpdateMultipleCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/GiveRandomCmdBuild.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SamGetChallengeCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SamReadCeilingsCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SamReadEventCounterCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SamReadKeyParametersCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SamWriteKeyCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SelectDiversifierCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/UnlockCmdBuild.cpp

	# calypso - command - sam - parser - security
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/CardGenerateKeyRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/DigestAuthenticateRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/DigestCloseRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/DigestInitRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/DigestUpdateMultipleRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/DigestUpdateRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/GiveRandomRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SamGetChallengeRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SamReadCeilingsRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SamReadEventCounterRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SamReadKeyParametersRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SamWriteKeyRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SelectDiversifierRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/UnlockRespPars.cpp

	# calypso - transaction
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/CalypsoPo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/CalypsoSam.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoResource.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoSelectionRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoSelector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoTransaction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamIdentifier.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamResource.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamResourceManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamSelectionRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamSelector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SecuritySettings.cpp

	# calypso - transaction - exception
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/exception/KeypleCalypsoSecureSessionException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/exception/KeypleCalypsoSecureSessionUnauthorizedKvcException.cpp
)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} keyplecore)

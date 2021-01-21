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
SET(VERSION_MINOR "8")
SET(VERSION_PATCH "1")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "KeypleCalypso")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)

SET(LIBRARY_NAME keyplecalypso)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DKEYPLECALYPSO_EXPORT")

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/command
	${CMAKE_CURRENT_SOURCE_DIR}/command/po
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/storedvalue
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/exception
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/storedvalue
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/exception
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security
	${CMAKE_CURRENT_SOURCE_DIR}/exception
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
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/command/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-core/src/main/selection


	/* Helpers */
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-common/src/main
	${CMAKE_CURRENT_SOURCE_DIR}/../../../keyple-common/src/main/exception
)

ADD_LIBRARY(
	${LIBRARY_NAME}

	${LIBRARY_TYPE}

	# calypso - command
	${CMAKE_CURRENT_SOURCE_DIR}/command/PoClass.cpp

	# calypso - command - po
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/AbstractPoResponseParser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/CalypsoPoCommand.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoRevision.cpp

	# calypso - command - po - builder
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/AppendRecordCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/DecreaseCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/GetDataFciCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/GetDataTraceCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/IncreaseCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/ReadRecordsCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/SelectFileCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/UpdateRecordCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/WriteRecordCmdBuild.cpp

	# calypso - command - po - builder - security
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/ChangeKeyCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/CloseSessionCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/InvalidateCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/OpenSession10CmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/OpenSession24CmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/OpenSession31CmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/OpenSession32CmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/PoGetChallengeCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/RatificationCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/RehabilitateCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/VerifyPinCmdBuild.cpp


	# calypso - command - po - builder - storedvalue
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/storedvalue/SvDebitCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/storedvalue/SvGetCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/storedvalue/SvReloadCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/storedvalue/SvUndebitCmdBuild.cpp

	# calypso - command - po - parser
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/AppendRecordRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/DecreaseRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/GetDataFciRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/GetDataTraceRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/IncreaseRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/ReadRecordsRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/SelectFileRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/UpdateRecordRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/WriteRecordRespPars.cpp

	# calypso - command - po - parser - security
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/AbstractOpenSessionRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/ChangeKeyRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/CloseSessionRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/InvalidateRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/OpenSession10RespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/OpenSession24RespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/OpenSession31RespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/OpenSession32RespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/PoGetChallengeRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/RehabilitateRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/VerifyPinRespPars.cpp

	# calypso - command - po - parser - storedvalue
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/storedvalue/SvDebitRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/storedvalue/SvGetRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/storedvalue/SvReloadRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/storedvalue/SvUndebitRespPars.cpp

	# calypso - command - sam
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/AbstractSamResponseParser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/CalypsoSamCommand.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/SamRevision.cpp

	# calypso - command - sam - builder - security
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/CardCipherPinCmdBuild.cpp
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
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SvCheckCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SvPrepareDebitCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SvPrepareLoadCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SvPrepareUndebitCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/UnlockCmdBuild.cpp

	# calypso - command - sam - parser - security
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/CardCipherPinRespPars.cpp
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
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SvCheckRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SvPrepareOperationRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/UnlockRespPars.cpp

	# calypso - transaction
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/CalypsoPo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/CalypsoPoUtils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/CalypsoSam.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/DirectoryHeader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/ElementaryFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/FileData.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/FileHeader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoCommandManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoSecuritySettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoSelectionRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoSelector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoTransaction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamCommandProcessor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamIdentifier.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamResourceManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamResourceManagerDefault.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamResourceManagerFactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamResourceManagerPool.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamSelectionRequest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamSelector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SvDebitLogRecord.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transaction/SvLoadLogRecord.cpp

	# calypso
	${CMAKE_CURRENT_SOURCE_DIR}/KeyReference.cpp
)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} keyplecore)

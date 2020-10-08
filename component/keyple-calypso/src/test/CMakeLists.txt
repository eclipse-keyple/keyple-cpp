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
	${CMAKE_CURRENT_SOURCE_DIR}/../main
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/po
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/po/builder
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/po/builder/security
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/po/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/po/parser
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/po/parser/security
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/sam
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/sam/builder
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/sam/builder/security
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/sam/exception
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/sam/parser
	${CMAKE_CURRENT_SOURCE_DIR}/../main/command/sam/parser/security
	${CMAKE_CURRENT_SOURCE_DIR}/../main/transaction
	${CMAKE_CURRENT_SOURCE_DIR}/../main/transaction/exception

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

	# google test and mock
	${GOOGLETEST_DIRECTORY}/googletest-src
	${GOOGLETEST_DIRECTORY}/googletest-src/googlemock/include
	${GOOGLETEST_DIRECTORY}/googletest-src/googletest/include
)

ADD_EXECUTABLE(keyplecalypso_ut

	# TU keyple calypso
	#${CMAKE_CURRENT_SOURCE_DIR}/MainTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/../main/KeyReference.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/KeyReferenceTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/PoClassTest.cpp

	# calypso - command
	#${CMAKE_CURRENT_SOURCE_DIR}/command/SendableInSessionTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/CalypsoBuilderParserTest.cpp

	# calypso - command - po
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/AbstractPoCommandBuilderTest.cpp

	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/AbstractPoResponseParser.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/CalypsoPoCommands.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoBuilderParser.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoCustomModificationCommandBuilder.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoCustomReadCommandBuilder.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoModificationCommand.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoRevision.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/PoSendableInSession.cpp

	# calypso - command - po - builder
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/AppendRecordCmdBuild.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/DecreaseCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/GetDataFciCmdBuildTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/IncreaseCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/ReadRecordsCmdBuildTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/SelectFileCmdBuild.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/UpdateRecordCmdBuildTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/WriteRecordCmdBuildTest.cpp

	# calypso - command - po - builder - security
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/ChangeKeyCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/CloseSessionCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/OpenSessionCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/PoGetChallengeCmdBuildTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/builder/security/RatificationCmdBuildTest.cpp

	# calypso - command - po - parser
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/AppendRecordRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/DecreaseRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/GetDataFciRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/IncreaseRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/ReadRecordsRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/SelectFileRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/UpdateRecordRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/WriteRecordRespParsTest.cpp

	# calypso - command - po - parser - security
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/AbstractOpenSessionRespParsTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/ChangeKeyRespPars.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/CloseSessionRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/OpenSessionRespParsTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/OpenSession24RespPars.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/OpenSession31RespPars.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/OpenSession32RespPars.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/po/parser/security/PoGetChallengeRespParsTest.cpp

	# calypso - command - sam
	#${CMAKE_CURRENT_SOURCE_DIR}/command/sam/AbstractSamCommandBuilder.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/sam/AbstractSamResponseParser.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/sam/CalypsoSamCommands.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/sam/SamBuilderParser.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/sam/SamRevision.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/command/sam/SamSendableInSession.cpp

	# calypso - command - sam - builder
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/CardGenerateKeyCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/DigestAuthenticateCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/DigestCloseCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/DigestInitCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/DigestUpdateCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/DigestUpdateMultipleCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/GiveRandomCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SamGetChallengeCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SamReadCeilingsCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SamReadEventCounterCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SamWriteKeyCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/SelectDiversifierCmdBuildTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/builder/security/UnlockCmdBuildTest.cpp

	# calypso - command - sam - parser
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/CardGenerateKeyRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/DigestAuthenticateRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/DigestCloseRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/DigestInitRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/DigestUpdateMultipleRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/DigestUpdateRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/GiveRandomRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SamGetChallengeRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SamReadCeilingsRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SamReadEventCounterRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SamReadKeyParametersRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SamWriteKeyRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/SelectDiversifierRespParsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command/sam/parser/security/UnlockRespParsTest.cpp

	# calypso - transaction
	#${CMAKE_CURRENT_SOURCE_DIR}/transaction/CalypsoPoTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/transaction/CalypsoSamTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoResource.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoSelectionRequest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoSelector.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/transaction/PoTransaction.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamResource.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamSelectionRequest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/transaction/SamSelectorTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/transaction/SecuritySettings.cpp

	# calypso - transaction - exception
	#${CMAKE_CURRENT_SOURCE_DIR}/transaction/exception/KeypleCalypsoSecureSessionException.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/transaction/exception/KeypleCalypsoSecureSessionUnauthorizedKvcException.cpp

	# calypso - test - util
	#${CMAKE_CURRENT_SOURCE_DIR}/util/ResponseUtilsTest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/util/TestsUtilsResponseTabByteGenerator.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/util/TestsUtilsStatusCodeGenerator.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/MainTest.cpp
)

SET(LINK_LIBS gtest gmock keyplecalypso keyplecore ${WINSCARD} )

TARGET_LINK_LIBRARIES(keyplecalypso_ut ${LINK_LIBS})

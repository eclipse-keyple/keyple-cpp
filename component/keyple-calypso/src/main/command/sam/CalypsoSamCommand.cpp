/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include "CalypsoSamCommand.h"

#include "DigestCloseCmdBuild.h"
#
#include "DigestInitCmdBuild.h"
#include "DigestInitRespPars.h"
#include "DigestUpdateCmdBuild.h"
#include "DigestUpdateRespPars.h"
#include "DigestUpdateMultipleCmdBuild.h"
#include "DigestUpdateMultipleRespPars.h"
#include "SamGetChallengeCmdBuild.h"
#include "SamGetChallengeRespPars.h"
#include "SelectDiversifierCmdBuild.h"
#include "SelectDiversifierCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

const CalypsoSamCommand::SELECT_DIVERSIFIER(
    "Select Diversifier",
    0x14,
    typeid(SelectDiversifierCmdBuild),
    typeid(SelectDiversifierCmdBuild));

const CalypsoSamCommand::GET_CHALLENGE(
    "Get Challenge",
    0x84,
    typeid(SamGetChallengeCmdBuild),
    typeid(SamGetChallengeRespPars));

const CalypsoSamCommand::DIGEST_INIT(
    "Digest Init",
    0x8A,
    typeid(DigestInitCmdBuild),
    typeid(DigestInitRespPars));

const CalypsoSamCommand::DIGEST_UPDATE(
    "Digest Update",
    0x8C,
    typeid(DigestUpdateCmdBuild),
    typeid(DigestUpdateRespPars));

const CalypsoSamCommand::DIGEST_UPDATE_MULTIPLE(
    "Digest Update Multiple",
    0x8C,
    typeid(DigestUpdateMultipleCmdBuild),
    typeid(DigestUpdateMultipleRespPars));

const CalypsoSamCommand::DIGEST_CLOSE(
    "Digest Close",
    0x8E,
    typeid(DigestCloseCmdBuild),
    typeid(DigestCloseRespPars));

const CalypsoSamCommand::DIGEST_AUTHENTICATE(
    "Digest Authenticate",
    0x82,
    typeid(DigestAuthenticateCmdBuild),
    typeid(DigestAuthenticateRespPars));

const CalypsoSamCommand::GIVE_RANDOM(
    "Give Random",
    0x86,
    typeid(GiveRandomCmdBuild),
    typeid(GiveRandomRespPars));

const CalypsoSamCommand::CARD_GENERATE_KEY(
    "Card Generate Key",
    0x12,
    typeid(CardGenerateKeyCmdBuild),
    typeid(CardGenerateKeyRespPars));

const CalypsoSamCommand::UNLOCK(
    "Unlock",
    0x20,
    typeid(UnlockCmdBuild),
    typeid(UnlockRespPars)):

const CalypsoSamCommand::WRITE_KEY(
    "Write Key",
    0x1A,
    typeid(SamWriteKeyCmdBuild),
    typeid(SamWriteKeyRespPars));

const CalypsoSamCommand::READ_KEY_PARAMETERS(
    "Read Key Parameters",
    0xBC,
    typeid(amReadKeyParametersCmdBuild),
    typeid(SamReadKeyParametersRespPars));

const CalypsoSamCommand::READ_EVENT_COUNTER(
    "Read Event Counter",
    0xBE,
    typeid(SamReadEventCounterCmdBuild),
    typeid(SamReadEventCounterRespPars));

const CalypsoSamCommand::READ_CEILINGS(
    "Read Ceilings",
    0xBE,
    typeid(SamReadCeilingsCmdBuild),
    typeid(SamReadCeilingsRespPars));

CalypsoSamCommand::CalypsoSamCommand(
  const std::string& name, const uint8_t instructionByte,
  const std::type_info& commandBuilderClass,
  const std::type_info& responseParserClass);
: mName(name), mInstructionByte(instructionByte),
  mCommandBuilderClass(commandBuilderClass),
  mResponseParserClass(responseParserClass) {}

const std::string& CalypsoSamCommand::getName() const
{
    return mName;
}

const uint8_t CalypsoSamCommand::getInstructionByte() const
{
    return mInstructionByte;
}

const std::type_info& getCommandBuilderClass() const
{
    return mCommandBuilderClass;
}

const std::type_info& getResponseParserClass() const
{
    return mResponseParserClass;
}

}
}
}
}

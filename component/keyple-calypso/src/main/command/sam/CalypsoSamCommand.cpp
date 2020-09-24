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

#include "CardGenerateKeyCmdBuild.h"
#include "CardGenerateKeyRespPars.h"
#include "DigestAuthenticateCmdBuild.h"
#include "DigestAuthenticateRespPars.h"
#include "DigestCloseCmdBuild.h"
#include "DigestCloseRespPars.h"
#include "DigestInitCmdBuild.h"
#include "DigestInitRespPars.h"
#include "DigestUpdateCmdBuild.h"
#include "DigestUpdateRespPars.h"
#include "DigestUpdateMultipleCmdBuild.h"
#include "DigestUpdateMultipleRespPars.h"
#include "GiveRandomCmdBuild.h"
#include "GiveRandomRespPars.h"
#include "SamGetChallengeCmdBuild.h"
#include "SamGetChallengeRespPars.h"
#include "SamWriteKeyCmdBuild.h"
#include "SamWriteKeyRespPars.h"
#include "SamReadKeyParametersCmdBuild.h"
#include "SamReadKeyParametersRespPars.h"
#include "SamReadEventCounterCmdBuild.h"
#include "SamReadEventCounterRespPars.h"
#include "SamReadCeilingsCmdBuild.h"
#include "SamReadCeilingsRespPars.h"
#include "SelectDiversifierCmdBuild.h"
#include "SelectDiversifierCmdBuild.h"
#include "UnlockCmdBuild.h"
#include "UnlockRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::calypso::command::sam::builder::security;

const CalypsoSamCommand CalypsoSamCommand::SELECT_DIVERSIFIER(
    "Select Diversifier",
    0x14,
    typeid(SelectDiversifierCmdBuild),
    typeid(SelectDiversifierCmdBuild));

const CalypsoSamCommand CalypsoSamCommand::GET_CHALLENGE(
    "Get Challenge",
    0x84,
    typeid(SamGetChallengeCmdBuild),
    typeid(SamGetChallengeRespPars));

const CalypsoSamCommand CalypsoSamCommand::DIGEST_INIT(
    "Digest Init",
    0x8A,
    typeid(DigestInitCmdBuild),
    typeid(DigestInitRespPars));

const CalypsoSamCommand CalypsoSamCommand::DIGEST_UPDATE(
    "Digest Update",
    0x8C,
    typeid(DigestUpdateCmdBuild),
    typeid(DigestUpdateRespPars));

const CalypsoSamCommand CalypsoSamCommand::DIGEST_UPDATE_MULTIPLE(
    "Digest Update Multiple",
    0x8C,
    typeid(DigestUpdateMultipleCmdBuild),
    typeid(DigestUpdateMultipleRespPars));

const CalypsoSamCommand CalypsoSamCommand::DIGEST_CLOSE(
    "Digest Close",
    0x8E,
    typeid(DigestCloseCmdBuild),
    typeid(DigestCloseRespPars));

const CalypsoSamCommand CalypsoSamCommand::DIGEST_AUTHENTICATE(
    "Digest Authenticate",
    0x82,
    typeid(DigestAuthenticateCmdBuild),
    typeid(DigestAuthenticateRespPars));

const CalypsoSamCommand CalypsoSamCommand::GIVE_RANDOM(
    "Give Random",
    0x86,
    typeid(GiveRandomCmdBuild),
    typeid(GiveRandomRespPars));

const CalypsoSamCommand CalypsoSamCommand::CARD_GENERATE_KEY(
    "Card Generate Key",
    0x12,
    typeid(CardGenerateKeyCmdBuild),
    typeid(CardGenerateKeyRespPars));

const CalypsoSamCommand CalypsoSamCommand::UNLOCK(
    "Unlock",
    0x20,
    typeid(UnlockCmdBuild),
    typeid(UnlockRespPars));

const CalypsoSamCommand CalypsoSamCommand::WRITE_KEY(
    "Write Key",
    0x1A,
    typeid(SamWriteKeyCmdBuild),
    typeid(SamWriteKeyRespPars));

const CalypsoSamCommand CalypsoSamCommand::READ_KEY_PARAMETERS(
    "Read Key Parameters",
    0xBC,
    typeid(SamReadKeyParametersCmdBuild),
    typeid(SamReadKeyParametersRespPars));

const CalypsoSamCommand CalypsoSamCommand::READ_EVENT_COUNTER(
    "Read Event Counter",
    0xBE,
    typeid(SamReadEventCounterCmdBuild),
    typeid(SamReadEventCounterRespPars));

const CalypsoSamCommand CalypsoSamCommand::READ_CEILINGS(
    "Read Ceilings",
    0xBE,
    typeid(SamReadCeilingsCmdBuild),
    typeid(SamReadCeilingsRespPars));

CalypsoSamCommand::CalypsoSamCommand(
  const std::string& name,
  const uint8_t instructionByte,
  const std::type_info& commandBuilderClass,
  const std::type_info& responseParserClass)
: mName(name),
  mInstructionByte(instructionByte),
  mCommandBuilderClass(commandBuilderClass),
  mResponseParserClass(responseParserClass) {}

const std::string& CalypsoSamCommand::getName() const
{
    return mName;
}

uint8_t CalypsoSamCommand::getInstructionByte() const
{
    return mInstructionByte;
}

const std::type_info& CalypsoSamCommand::getCommandBuilderClass() const
{
    return mCommandBuilderClass;
}

const std::type_info& CalypsoSamCommand::getResponseParserClass() const
{
    return mResponseParserClass;
}

}
}
}
}

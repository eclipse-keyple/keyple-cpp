/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
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

#include "CalypsoSamCommands.h"
#include "CardGenerateKeyCmdBuild.h"
#include "CardGenerateKeyRespPars.h"
#include "SelectDiversifierCmdBuild.h"
#include "SelectDiversifierRespPars.h"
#include "DigestAuthenticateCmdBuild.h"
#include "DigestAuthenticateRespPars.h"
#include "DigestCloseRespPars.h"
#include "DigestCloseCmdBuild.h"
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
#include "SamReadCeilingsCmdBuild.h"
#include "SamReadCeilingsRespPars.h"
#include "SamReadEventCounterCmdBuild.h"
#include "SamReadEventCounterRespPars.h"
#include "SamReadKeyParametersCmdBuild.h"
#include "SamReadKeyParametersRespPars.h"
#include "SamWriteKeyCmdBuild.h"
#include "SamWriteKeyRespPars.h"
#include "UnlockCmdBuild.h"
#include "UnlockRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::core::command;

CalypsoSamCommands CalypsoSamCommands::SELECT_DIVERSIFIER(
    "SELECT_DIVERSIFIER",
    InnerEnum::SELECT_DIVERSIFIER,
    "Select Diversifier",
    static_cast<char>(0x14),
    typeid(SelectDiversifierCmdBuild),
    typeid(SelectDiversifierRespPars));

CalypsoSamCommands CalypsoSamCommands::GET_CHALLENGE(
    "GET_CHALLENGE",
    InnerEnum::GET_CHALLENGE,
    "Get Challenge",
    static_cast<char>(0x84),
    typeid(SamGetChallengeCmdBuild),
    typeid(SamGetChallengeRespPars));

CalypsoSamCommands CalypsoSamCommands::DIGEST_INIT(
    "DIGEST_INIT",
    InnerEnum::DIGEST_INIT,
    "Digest Init",
    static_cast<char>(0x8A),
    typeid(DigestInitCmdBuild),
    typeid(DigestInitRespPars));

CalypsoSamCommands CalypsoSamCommands::DIGEST_UPDATE(
    "DIGEST_UPDATE",
    InnerEnum::DIGEST_UPDATE,
    "Digest Update",
    static_cast<char>(0x8C),
    typeid(DigestUpdateCmdBuild),
    typeid(DigestUpdateRespPars));

CalypsoSamCommands CalypsoSamCommands::DIGEST_UPDATE_MULTIPLE(
    "DIGEST_UPDATE_MULTIPLE",
    InnerEnum::DIGEST_UPDATE_MULTIPLE,
    "Digest Update Multiple",
    static_cast<char>(0x8C),
    typeid(DigestUpdateMultipleCmdBuild),
    typeid(DigestUpdateMultipleRespPars));

CalypsoSamCommands CalypsoSamCommands::DIGEST_CLOSE(
    "DIGEST_CLOSE",
    InnerEnum::DIGEST_CLOSE,
    "Digest Close",
    static_cast<char>(0x8E),
    typeid(DigestCloseCmdBuild),
    typeid(DigestCloseRespPars));

CalypsoSamCommands CalypsoSamCommands::DIGEST_AUTHENTICATE(
    "DIGEST_AUTHENTICATE",
    InnerEnum::DIGEST_AUTHENTICATE,
    "Digest Authenticate",
    static_cast<char>(0x82),
    typeid(DigestAuthenticateCmdBuild),
    typeid(DigestAuthenticateRespPars));

CalypsoSamCommands CalypsoSamCommands::GIVE_RANDOM(
    "GIVE_RANDOM",
    InnerEnum::GIVE_RANDOM,
    "Give Random",
    static_cast<char>(0x86),
    typeid(GiveRandomCmdBuild),
    typeid(GiveRandomRespPars));

CalypsoSamCommands CalypsoSamCommands::CARD_GENERATE_KEY(
    "CARD_GENERATE_KEY",
    InnerEnum::CARD_GENERATE_KEY,
    "Card Generate Key",
    static_cast<char>(0x12),
    typeid(CardGenerateKeyCmdBuild),
    typeid(CardGenerateKeyRespPars));

CalypsoSamCommands CalypsoSamCommands::UNLOCK(
    "UNLOCK",
    InnerEnum::UNLOCK,
    "Unlock",
    static_cast<char>(0x20),
    typeid(UnlockCmdBuild),
    typeid(UnlockRespPars));

CalypsoSamCommands CalypsoSamCommands::WRITE_KEY(
    "WRITE_KEY",
    InnerEnum::WRITE_KEY,
    "Write Key",
    static_cast<char>(0x1A),
    typeid(SamWriteKeyCmdBuild),
    typeid(SamWriteKeyRespPars));

CalypsoSamCommands CalypsoSamCommands::READ_KEY_PARAMETERS(
    "READ_KEY_PARAMETERS",
    InnerEnum::READ_KEY_PARAMETERS,
    "Read Key Parameters",
    static_cast<char>(0xBC),
    typeid(SamReadKeyParametersCmdBuild),
    typeid(SamReadKeyParametersRespPars));

CalypsoSamCommands CalypsoSamCommands::READ_EVENT_COUNTER(
    "READ_EVENT_COUNTER",
    InnerEnum::READ_EVENT_COUNTER,
    "Read Event Counter",
    static_cast<char>(0xBE),
    typeid(SamReadEventCounterCmdBuild),
    typeid(SamReadEventCounterRespPars));

CalypsoSamCommands CalypsoSamCommands::READ_CEILINGS(
    "READ_CEILINGS",
    InnerEnum::READ_CEILINGS,
    "Read Ceilings",
    static_cast<char>(0xBE),
    typeid(SamReadCeilingsCmdBuild),
    typeid(SamReadCeilingsRespPars));

std::vector<CalypsoSamCommands> CalypsoSamCommands::valueList;

CalypsoSamCommands::StaticConstructor::StaticConstructor() {
    valueList.push_back(SELECT_DIVERSIFIER);
    valueList.push_back(GET_CHALLENGE);
    valueList.push_back(DIGEST_INIT);
    valueList.push_back(DIGEST_UPDATE);
    valueList.push_back(DIGEST_UPDATE_MULTIPLE);
    valueList.push_back(DIGEST_CLOSE);
    valueList.push_back(DIGEST_AUTHENTICATE);
}

CalypsoSamCommands::StaticConstructor CalypsoSamCommands::staticConstructor;
int CalypsoSamCommands::nextOrdinal = 0;

CalypsoSamCommands::CalypsoSamCommands(
  const std::string &nameValue, InnerEnum innerEnum, const std::string &name,
  char instructionByte, const std::type_info& commandBuilderClass,
  const std::type_info& responseParserClass)
: innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++),
  name(name), instructionbyte(instructionByte),
  commandBuilderClass(commandBuilderClass),
  responseParserClass(responseParserClass)
{
}

const std::string& CalypsoSamCommands::getName() const
{
    return name;
}

uint8_t CalypsoSamCommands::getInstructionByte() const
{
    return instructionbyte;
}

const std::type_info& CalypsoSamCommands::getCommandBuilderClass() const
{
    return commandBuilderClass;
}

const std::type_info& CalypsoSamCommands::getResponseParserClass() const
{
    return responseParserClass;
}

bool CalypsoSamCommands::operator==(const CalypsoSamCommands &other)
{
    return this->ordinalValue == other.ordinalValue;
}

bool CalypsoSamCommands::operator!=(const CalypsoSamCommands &other)
{
    return this->ordinalValue != other.ordinalValue;
}

std::vector<CalypsoSamCommands> CalypsoSamCommands::values()
{
    return valueList;
}

int CalypsoSamCommands::ordinal()
{
    return ordinalValue;
}

std::string CalypsoSamCommands::toString()
{
    return nameValue;
}

CalypsoSamCommands CalypsoSamCommands::valueOf(const std::string &name)
{
    for (auto enumInstance : CalypsoSamCommands::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Compiler fix */
    return CalypsoSamCommands("Dummy", InnerEnum::DIGEST_AUTHENTICATE, "Dummy",
                              0, typeid(CalypsoSamCommands),
                              typeid(CalypsoSamCommands));
}

}
}
}
}

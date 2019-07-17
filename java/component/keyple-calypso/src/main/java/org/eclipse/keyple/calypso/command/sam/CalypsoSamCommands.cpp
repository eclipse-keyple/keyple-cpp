#include "CalypsoSamCommands.h"
#include "CardGenerateKeyCmdBuild.h"
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
#include "SamGetChallengeCmdBuild.h"
#include "SamGetChallengeRespPars.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace org::eclipse::keyple::calypso::command::sam::builder::security;
using namespace org::eclipse::keyple::calypso::command::sam::parser::security;
using namespace org::eclipse::keyple::core::command;

CalypsoSamCommands CalypsoSamCommands::SELECT_DIVERSIFIER(    "SELECT_DIVERSIFIER",     InnerEnum::SELECT_DIVERSIFIER,     "Select Diversifier",     static_cast<char>(0x14), typeid(SelectDiversifierCmdBuild),    typeid(SelectDiversifierRespPars));
CalypsoSamCommands CalypsoSamCommands::GET_CHALLENGE(         "GET_CHALLENGE",          InnerEnum::GET_CHALLENGE,          "Get Challenge",          static_cast<char>(0x84), typeid(SamGetChallengeCmdBuild),      typeid(SamGetChallengeRespPars));
CalypsoSamCommands CalypsoSamCommands::DIGEST_INIT(           "DIGEST_INIT",            InnerEnum::DIGEST_INIT,            "Digest Init",            static_cast<char>(0x8A), typeid(DigestInitCmdBuild),           typeid(DigestInitRespPars));
CalypsoSamCommands CalypsoSamCommands::DIGEST_UPDATE(         "DIGEST_UPDATE",          InnerEnum::DIGEST_UPDATE,          "Digest Update",          static_cast<char>(0x8C), typeid(DigestUpdateCmdBuild),         typeid(DigestUpdateRespPars));
CalypsoSamCommands CalypsoSamCommands::DIGEST_UPDATE_MULTIPLE("DIGEST_UPDATE_MULTIPLE", InnerEnum::DIGEST_UPDATE_MULTIPLE, "Digest Update Multiple", static_cast<char>(0x8C), typeid(DigestUpdateMultipleCmdBuild), typeid(DigestUpdateMultipleRespPars));
CalypsoSamCommands CalypsoSamCommands::DIGEST_CLOSE(          "DIGEST_CLOSE",           InnerEnum::DIGEST_CLOSE,           "Digest Close",           static_cast<char>(0x8E), typeid(DigestCloseCmdBuild),          typeid(DigestCloseRespPars));
CalypsoSamCommands CalypsoSamCommands::DIGEST_AUTHENTICATE(   "DIGEST_AUTHENTICATE",    InnerEnum::DIGEST_AUTHENTICATE,    "Digest Authenticate",    static_cast<char>(0x82), typeid(DigestAuthenticateCmdBuild),   typeid(DigestAuthenticateRespPars));
CalypsoSamCommands CalypsoSamCommands::GIVE_RANDOM(           "GIVE_RANDOM",            InnerEnum::GIVE_RANDOM,            "Give Random",            static_cast<char>(0x86), typeid(GiveRandomCmdBuild),           typeid(GiveRandomCmdBuild));
CalypsoSamCommands CalypsoSamCommands::CARD_GENERATE_KEY(     "CARD_GENERATE_KEY",      InnerEnum::CARD_GENERATE_KEY,      "Card Generate Key",      static_cast<char>(0x12), typeid(CardGenerateKeyCmdBuild),      typeid(CardGenerateKeyCmdBuild));

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

CalypsoSamCommands::CalypsoSamCommands(const std::string &nameValue, InnerEnum innerEnum, const std::string &name, char instructionByte, const std::type_info& commandBuilderClass, const std::type_info& responseParserClass)
: innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++), name(name), instructionbyte(instructionByte), commandBuilderClass(commandBuilderClass), responseParserClass(responseParserClass)
{
}

std::string CalypsoSamCommands::getName() {
    return name;
}

char CalypsoSamCommands::getInstructionByte() {
    return instructionbyte;
}

const std::type_info& CalypsoSamCommands::getCommandBuilderClass() {
    return commandBuilderClass;
}

const std::type_info& CalypsoSamCommands::getResponseParserClass() {
    return responseParserClass;
}

bool CalypsoSamCommands::operator == (const CalypsoSamCommands &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool CalypsoSamCommands::operator != (const CalypsoSamCommands &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<CalypsoSamCommands> CalypsoSamCommands::values() {
    return valueList;
}

int CalypsoSamCommands::ordinal() {
    return ordinalValue;
}

std::string CalypsoSamCommands::toString() {
    return nameValue;
}

CalypsoSamCommands CalypsoSamCommands::valueOf(const std::string &name) {
    for (auto enumInstance : CalypsoSamCommands::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Compiler fix */
    return CalypsoSamCommands("Dummy", InnerEnum::DIGEST_AUTHENTICATE, "Dummy", 0, typeid(CalypsoSamCommands), typeid(CalypsoSamCommands));
}

}
}
}
}
}
}

#include <typeinfo>

#include "CalypsoPoCommands.h"
#include "PoRevision.h"
#include "AbstractApduCommandBuilder.h"
#include "AbstractApduResponseParser_Import.h"
#include "GetDataFciCmdBuild.h"
#include "OpenSession10CmdBuild.h"
#include "OpenSession24CmdBuild.h"
#include "OpenSession31CmdBuild.h"
#include "OpenSession32CmdBuild.h"
#include "CloseSessionCmdBuild.h"
#include "ReadRecordsCmdBuild.h"
#include "AppendRecordCmdBuild.h"
#include "UpdateRecordCmdBuild.h"
#include "PoGetChallengeCmdBuild.h"
#include "SelectFileCmdBuild.h"
#include "IncreaseCmdBuild.h"
#include "DecreaseCmdBuild.h"
#include "GetDataFciRespPars.h"
#include "OpenSession10RespPars.h"
#include "OpenSession24RespPars.h"
#include "OpenSession31RespPars.h"
#include "OpenSession32RespPars.h"
#include "CloseSessionRespPars.h"
#include "ReadRecordsRespPars.h"
#include "AppendRecordRespPars.h"
#include "UpdateRecordRespPars.h"
#include "PoGetChallengeRespPars.h"
#include "SelectFileRespPars.h"
#include "IncreaseRespPars.h"
#include "DecreaseRespPars.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {

using namespace org::eclipse::keyple::core::command;
using namespace org::eclipse::keyple::calypso::command::po::builder;
using namespace org::eclipse::keyple::calypso::command::po::builder::security;
using namespace org::eclipse::keyple::calypso::command::po::parser;
using namespace org::eclipse::keyple::calypso::command::po::parser::security;

CalypsoPoCommands CalypsoPoCommands::GET_DATA_FCI("GET_DATA_FCI", InnerEnum::GET_DATA_FCI, "Get Data'FCI'", static_cast<char>(0xCA));
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_10("OPEN_SESSION_10", InnerEnum::OPEN_SESSION_10, "Open Secure Session V1", static_cast<char>(0x8A));
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_24("OPEN_SESSION_24", InnerEnum::OPEN_SESSION_24, "Open Secure Session V2.4", static_cast<char>(0x8A));
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_31("OPEN_SESSION_31", InnerEnum::OPEN_SESSION_31, "Open Secure Session V3.1", static_cast<char>(0x8A));
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_32("OPEN_SESSION_32", InnerEnum::OPEN_SESSION_32, "Open Secure Session V3.2", static_cast<char>(0x8A));
CalypsoPoCommands CalypsoPoCommands::CLOSE_SESSION("CLOSE_SESSION", InnerEnum::CLOSE_SESSION, "Close Secure Session", static_cast<char>(0x8E));
CalypsoPoCommands CalypsoPoCommands::READ_RECORDS("READ_RECORDS", InnerEnum::READ_RECORDS, "Read Records", static_cast<char>(0xB2));
CalypsoPoCommands CalypsoPoCommands::UPDATE_RECORD("UPDATE_RECORD", InnerEnum::UPDATE_RECORD, "Update Record", static_cast<char>(0xDC));
CalypsoPoCommands CalypsoPoCommands::APPEND_RECORD("APPEND_RECORD", InnerEnum::APPEND_RECORD, "Append Record", static_cast<char>(0xE2));
CalypsoPoCommands CalypsoPoCommands::GET_CHALLENGE("GET_CHALLENGE", InnerEnum::GET_CHALLENGE, "Get Challenge", static_cast<char>(0x84));
CalypsoPoCommands CalypsoPoCommands::INCREASE("INCREASE", InnerEnum::INCREASE, "Increase", static_cast<char>(0x32));
CalypsoPoCommands CalypsoPoCommands::DECREASE("DECREASE", InnerEnum::DECREASE, "Decrease", static_cast<char>(0x30));
CalypsoPoCommands CalypsoPoCommands::SELECT_FILE("SELECT_FILE", InnerEnum::SELECT_FILE, "Select File", static_cast<char>(0xA4));
CalypsoPoCommands CalypsoPoCommands::CHANGE_KEY("CHANGE_KEY", InnerEnum::CHANGE_KEY, "Change Key", static_cast<char>(0xD8));

std::vector<CalypsoPoCommands> CalypsoPoCommands::valueList;

CalypsoPoCommands::StaticConstructor::StaticConstructor()
{
    valueList.push_back(GET_DATA_FCI);
    valueList.push_back(OPEN_SESSION_10);
    valueList.push_back(OPEN_SESSION_24);
    valueList.push_back(OPEN_SESSION_31);
    valueList.push_back(OPEN_SESSION_32);
    valueList.push_back(CLOSE_SESSION);
    valueList.push_back(READ_RECORDS);
    valueList.push_back(UPDATE_RECORD);
    valueList.push_back(APPEND_RECORD);
    valueList.push_back(GET_CHALLENGE);
    valueList.push_back(INCREASE);
    valueList.push_back(DECREASE);
    valueList.push_back(SELECT_FILE);
    valueList.push_back(CHANGE_KEY);
}

CalypsoPoCommands::StaticConstructor CalypsoPoCommands::staticConstructor;
int CalypsoPoCommands::nextOrdinal = 0;

CalypsoPoCommands::CalypsoPoCommands(const std::string &nameValue, InnerEnum innerEnum, const std::string &name, char instructionByte)
: innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++), name(name), instructionbyte(instructionByte)
{
}

CalypsoPoCommands::CalypsoPoCommands(const CalypsoPoCommands& other)
: CommandsTable(),
  innerEnumValue(other.innerEnumValue), nameValue(other.nameValue), ordinalValue(other.ordinalValue), name(other.name),
  instructionbyte(other.instructionbyte)
{
	(void)other;
}

std::string CalypsoPoCommands::getName()
{
    return name;
}

char CalypsoPoCommands::getInstructionByte()
{
    return instructionbyte;
}

CalypsoPoCommands& CalypsoPoCommands::getOpenSessionForRev(PoRevision rev)
{
    switch (rev) {
    case org::eclipse::keyple::calypso::command::po::PoRevision::REV1_0:
        return OPEN_SESSION_10;
    case org::eclipse::keyple::calypso::command::po::PoRevision::REV2_4:
        return OPEN_SESSION_24;
    case org::eclipse::keyple::calypso::command::po::PoRevision::REV3_1:
    case org::eclipse::keyple::calypso::command::po::PoRevision::REV3_1_CLAP:
        return OPEN_SESSION_31;
    case org::eclipse::keyple::calypso::command::po::PoRevision::REV3_2:
        return OPEN_SESSION_32;
    default:
        throw IllegalStateException("Any revision should have a matching command");
    }
}

bool CalypsoPoCommands::operator == (const CalypsoPoCommands &other)
{
    return this->ordinalValue == other.ordinalValue;
}

bool CalypsoPoCommands::operator != (const CalypsoPoCommands &other)
{
    return this->ordinalValue != other.ordinalValue;
}

std::vector<CalypsoPoCommands> CalypsoPoCommands::values()
{
    return valueList;
}

int CalypsoPoCommands::ordinal()
{
    return ordinalValue;
}

std::string CalypsoPoCommands::toString()
{
    return nameValue;
}

CalypsoPoCommands CalypsoPoCommands::valueOf(const std::string &name)
{
    for (auto enumInstance : CalypsoPoCommands::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Compiler fix */
    return CalypsoPoCommands("Dummy", InnerEnum::APPEND_RECORD, "Dummy", 0);
}

}
}
}
}
}
}

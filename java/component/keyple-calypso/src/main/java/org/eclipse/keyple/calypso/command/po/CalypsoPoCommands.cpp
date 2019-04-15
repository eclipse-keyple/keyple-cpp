#include <typeinfo>

#include "CalypsoPoCommands.h"
#include "PoRevision.h"
#include "AbstractApduCommandBuilder.h"
#include "AbstractApduResponseParser.h"
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

                        using AbstractApduCommandBuilder = org::eclipse::keyple::command::AbstractApduCommandBuilder;
                        using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                        using CommandsTable              = org::eclipse::keyple::command::CommandsTable;
                        using GetDataFciCmdBuild         = org::eclipse::keyple::calypso::command::po::builder::GetDataFciCmdBuild;
                        using OpenSession10CmdBuild      = org::eclipse::keyple::calypso::command::po::builder::session::OpenSession10CmdBuild;
                        using OpenSession24CmdBuild      = org::eclipse::keyple::calypso::command::po::builder::session::OpenSession24CmdBuild;
                        using OpenSession31CmdBuild      = org::eclipse::keyple::calypso::command::po::builder::session::OpenSession31CmdBuild;
                        using OpenSession32CmdBuild      = org::eclipse::keyple::calypso::command::po::builder::session::OpenSession32CmdBuild;
                        using CloseSessionCmdBuild       = org::eclipse::keyple::calypso::command::po::builder::session::CloseSessionCmdBuild;
                        using ReadRecordsCmdBuild        = org::eclipse::keyple::calypso::command::po::builder::ReadRecordsCmdBuild;
                        using UpdateRecordCmdBuild       = org::eclipse::keyple::calypso::command::po::builder::UpdateRecordCmdBuild;
                        using AppendRecordCmdBuild       = org::eclipse::keyple::calypso::command::po::builder::AppendRecordCmdBuild;                        
                        using PoGetChallengeCmdBuild     = org::eclipse::keyple::calypso::command::po::builder::session::PoGetChallengeCmdBuild;
                        using IncreaseCmdBuild           = org::eclipse::keyple::calypso::command::po::builder::IncreaseCmdBuild;
                        using DecreaseCmdBuild           = org::eclipse::keyple::calypso::command::po::builder::DecreaseCmdBuild;
                        using SelectFileCmdBuild         = org::eclipse::keyple::calypso::command::po::builder::SelectFileCmdBuild;
                        using GetDataFciRespPars         = org::eclipse::keyple::calypso::command::po::parser::GetDataFciRespPars;
                        using OpenSession10RespPars      = org::eclipse::keyple::calypso::command::po::parser::session::OpenSession10RespPars;
                        using OpenSession24RespPars      = org::eclipse::keyple::calypso::command::po::parser::session::OpenSession24RespPars;
                        using OpenSession31RespPars      = org::eclipse::keyple::calypso::command::po::parser::session::OpenSession31RespPars;
                        using OpenSession32RespPars      = org::eclipse::keyple::calypso::command::po::parser::session::OpenSession32RespPars;
                        using CloseSessionRespPars       = org::eclipse::keyple::calypso::command::po::parser::session::CloseSessionRespPars;
                        using ReadRecordsRespPars        = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
                        using UpdateRecordRespPars       = org::eclipse::keyple::calypso::command::po::parser::UpdateRecordRespPars;
                        using AppendRecordRespPars       = org::eclipse::keyple::calypso::command::po::parser::AppendRecordRespPars;
                        using PoGetChallengeRespPars     = org::eclipse::keyple::calypso::command::po::parser::session::PoGetChallengeRespPars;
                        using IncreaseRespPars           = org::eclipse::keyple::calypso::command::po::parser::IncreaseRespPars;
                        using DecreaseRespPars           = org::eclipse::keyple::calypso::command::po::parser::DecreaseRespPars;
                        using SelectFileRespPars         = org::eclipse::keyple::calypso::command::po::parser::SelectFileRespPars;

CalypsoPoCommands CalypsoPoCommands::GET_DATA_FCI(   "GET_DATA_FCI",    InnerEnum::GET_DATA_FCI,    "Get Data'FCI'",            static_cast<char>(0xCA), typeid(GetDataFciCmdBuild),     typeid(GetDataFciRespPars));
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_10("OPEN_SESSION_10", InnerEnum::OPEN_SESSION_10, "Open Secure Session V1",   static_cast<char>(0x8A), typeid(OpenSession10CmdBuild),  typeid(OpenSession10RespPars));
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_24("OPEN_SESSION_24", InnerEnum::OPEN_SESSION_24, "Open Secure Session V2.4", static_cast<char>(0x8A), typeid(OpenSession24CmdBuild),  typeid(OpenSession24RespPars));
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_31("OPEN_SESSION_31", InnerEnum::OPEN_SESSION_31, "Open Secure Session V3.1", static_cast<char>(0x8A), typeid(OpenSession31CmdBuild),  typeid(OpenSession31RespPars));
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_32("OPEN_SESSION_32", InnerEnum::OPEN_SESSION_32, "Open Secure Session V3.2", static_cast<char>(0x8A), typeid(OpenSession32CmdBuild),  typeid(OpenSession32RespPars));
CalypsoPoCommands CalypsoPoCommands::CLOSE_SESSION(  "CLOSE_SESSION",   InnerEnum::CLOSE_SESSION,   "Close Secure Session",     static_cast<char>(0x8E), typeid(CloseSessionCmdBuild),   typeid(CloseSessionRespPars));
CalypsoPoCommands CalypsoPoCommands::READ_RECORDS(   "READ_RECORDS",    InnerEnum::READ_RECORDS,    "Read Records",             static_cast<char>(0xB2), typeid(ReadRecordsCmdBuild),    typeid(ReadRecordsRespPars));
CalypsoPoCommands CalypsoPoCommands::UPDATE_RECORD(  "UPDATE_RECORD",   InnerEnum::UPDATE_RECORD,   "Update Record",            static_cast<char>(0xDC), typeid(UpdateRecordCmdBuild),   typeid(UpdateRecordRespPars));
CalypsoPoCommands CalypsoPoCommands::APPEND_RECORD(  "APPEND_RECORD",   InnerEnum::APPEND_RECORD,   "Append Record",            static_cast<char>(0xE2), typeid(AppendRecordCmdBuild),   typeid(AppendRecordRespPars));
CalypsoPoCommands CalypsoPoCommands::GET_CHALLENGE(  "GET_CHALLENGE",   InnerEnum::GET_CHALLENGE,   "Get Challenge",            static_cast<char>(0x84), typeid(PoGetChallengeCmdBuild), typeid(PoGetChallengeRespPars));
CalypsoPoCommands CalypsoPoCommands::INCREASE(       "INCREASE",        InnerEnum::INCREASE,        "Increase",                 static_cast<char>(0x32), typeid(IncreaseCmdBuild),       typeid(IncreaseRespPars));
CalypsoPoCommands CalypsoPoCommands::DECREASE(       "DECREASE",        InnerEnum::DECREASE,        "Decrease",                 static_cast<char>(0x30), typeid(DecreaseCmdBuild),       typeid(DecreaseRespPars));
CalypsoPoCommands CalypsoPoCommands::SELECT_FILE(    "SELECT_FILE",     InnerEnum::SELECT_FILE,     "Select File",              static_cast<char>(0xA4), typeid(SelectFileCmdBuild),     typeid(SelectFileRespPars));

std::vector<CalypsoPoCommands> CalypsoPoCommands::valueList;

CalypsoPoCommands::StaticConstructor::StaticConstructor() {
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
}

CalypsoPoCommands::StaticConstructor CalypsoPoCommands::staticConstructor;
int CalypsoPoCommands::nextOrdinal = 0;

                        CalypsoPoCommands::CalypsoPoCommands(const std::string &nameValue, InnerEnum innerEnum, const std::string &name, char instructionByte, const std::type_info& commandBuilderClass, const std::type_info& responseParserClass)
                        : innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++), name(name), instructionbyte(instructionByte), commandBuilderClass(commandBuilderClass), responseParserClass(responseParserClass) {
                            
                        }

                        std::string CalypsoPoCommands::getName() {
                            return name;
                        }

                        char CalypsoPoCommands::getInstructionByte() {
                            return instructionbyte;
                        }

                        const std::type_info& CalypsoPoCommands::getCommandBuilderClass() {
                            return commandBuilderClass;
                        }

                        const std::type_info& CalypsoPoCommands::getResponseParserClass() {
                            return responseParserClass;
                        }

                        CalypsoPoCommands CalypsoPoCommands::getOpenSessionForRev(PoRevision rev) {
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
                                    throw std::make_shared<IllegalStateException>("Any revision should have a matching command");
                            }
                        }

bool CalypsoPoCommands::operator == (const CalypsoPoCommands &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool CalypsoPoCommands::operator != (const CalypsoPoCommands &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<CalypsoPoCommands> CalypsoPoCommands::values() {
    return valueList;
}

int CalypsoPoCommands::ordinal() {
    return ordinalValue;
}

std::string CalypsoPoCommands::toString() {
    return nameValue;
}

CalypsoPoCommands CalypsoPoCommands::valueOf(const std::string &name) {
    for (auto enumInstance : CalypsoPoCommands::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Compiler fix */
    return CalypsoPoCommands("Dummy", InnerEnum::APPEND_RECORD, "Dummy", 0, typeid(CalypsoPoCommands), typeid(CalypsoPoCommands));
}
                    }
                }
            }
        }
    }
}

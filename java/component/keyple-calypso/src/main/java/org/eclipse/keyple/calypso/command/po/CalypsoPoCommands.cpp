#include "CalypsoPoCommands.h"
#include "PoRevision.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        using AbstractApduCommandBuilder = org::eclipse::keyple::command::AbstractApduCommandBuilder;
                        using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                        using CommandsTable = org::eclipse::keyple::command::CommandsTable;

CalypsoPoCommands CalypsoPoCommands::GET_DATA_FCI("GET_DATA_FCI", InnerEnum::GET_DATA_FCI, "Get Data'FCI'", static_cast<char>(0xCA), org::eclipse::keyple::calypso::command::po::builder::GetDataFciCmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::GetDataFciRespPars::typeid);
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_10("OPEN_SESSION_10", InnerEnum::OPEN_SESSION_10, "Open Secure Session V1", static_cast<char>(0x8A), org::eclipse::keyple::calypso::command::po::builder::session::OpenSession10CmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::session::OpenSession10RespPars::typeid);
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_24("OPEN_SESSION_24", InnerEnum::OPEN_SESSION_24, "Open Secure Session V2.4", static_cast<char>(0x8A), org::eclipse::keyple::calypso::command::po::builder::session::OpenSession24CmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::session::OpenSession24RespPars::typeid);
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_31("OPEN_SESSION_31", InnerEnum::OPEN_SESSION_31, "Open Secure Session V3.1", static_cast<char>(0x8A), org::eclipse::keyple::calypso::command::po::builder::session::OpenSession31CmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::session::OpenSession31RespPars::typeid);
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_32("OPEN_SESSION_32", InnerEnum::OPEN_SESSION_32, "Open Secure Session V3.2", static_cast<char>(0x8A), org::eclipse::keyple::calypso::command::po::builder::session::OpenSession32CmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::session::OpenSession32RespPars::typeid);
CalypsoPoCommands CalypsoPoCommands::CLOSE_SESSION("CLOSE_SESSION", InnerEnum::CLOSE_SESSION, "Close Secure Session", static_cast<char>(0x8E), org::eclipse::keyple::calypso::command::po::builder::session::CloseSessionCmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::session::CloseSessionRespPars::typeid);
CalypsoPoCommands CalypsoPoCommands::READ_RECORDS("READ_RECORDS", InnerEnum::READ_RECORDS, "Read Records", static_cast<char>(0xB2), org::eclipse::keyple::calypso::command::po::builder::ReadRecordsCmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars::typeid);
CalypsoPoCommands CalypsoPoCommands::UPDATE_RECORD("UPDATE_RECORD", InnerEnum::UPDATE_RECORD, "Update Record", static_cast<char>(0xDC), org::eclipse::keyple::calypso::command::po::builder::UpdateRecordCmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::UpdateRecordRespPars::typeid);
CalypsoPoCommands CalypsoPoCommands::APPEND_RECORD("APPEND_RECORD", InnerEnum::APPEND_RECORD, "Append Record", static_cast<char>(0xE2), org::eclipse::keyple::calypso::command::po::builder::AppendRecordCmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::AppendRecordRespPars::typeid);
CalypsoPoCommands CalypsoPoCommands::GET_CHALLENGE("GET_CHALLENGE", InnerEnum::GET_CHALLENGE, "Get Challenge", static_cast<char>(0x84), org::eclipse::keyple::calypso::command::po::builder::session::PoGetChallengeCmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::session::PoGetChallengeRespPars::typeid);
CalypsoPoCommands CalypsoPoCommands::INCREASE("INCREASE", InnerEnum::INCREASE, "Increase", static_cast<char>(0x32), org::eclipse::keyple::calypso::command::po::builder::IncreaseCmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::IncreaseRespPars::typeid);
CalypsoPoCommands CalypsoPoCommands::DECREASE("DECREASE", InnerEnum::DECREASE, "Decrease", static_cast<char>(0x30), org::eclipse::keyple::calypso::command::po::builder::DecreaseCmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::DecreaseRespPars::typeid);
CalypsoPoCommands CalypsoPoCommands::SELECT_FILE("SELECT_FILE", InnerEnum::SELECT_FILE, "Select File", static_cast<char>(0xA4), org::eclipse::keyple::calypso::command::po::builder::SelectFileCmdBuild::typeid, org::eclipse::keyple::calypso::command::po::parser::SelectFileRespPars::typeid);

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

                        CalypsoPoCommands::CalypsoPoCommands(const std::string &name, InnerEnum innerEnum, const std::string &name, char instructionByte, std::type_info commandBuilderClass, std::type_info responseParserClass) : nameValue(name), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum) {
                            this->name = name;
                            this->instructionbyte = instructionByte;
                            this->commandBuilderClass = commandBuilderClass;
                            this->responseParserClass = responseParserClass;
                        }

                        std::string CalypsoPoCommands::getName() {
                            return name;
                        }

                        char CalypsoPoCommands::getInstructionByte() {
                            return instructionbyte;
                        }

                        std::type_info CalypsoPoCommands::getCommandBuilderClass() {
                            return commandBuilderClass;
                        }

                        std::type_info CalypsoPoCommands::getResponseParserClass() {
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
}
                    }
                }
            }
        }
    }
}

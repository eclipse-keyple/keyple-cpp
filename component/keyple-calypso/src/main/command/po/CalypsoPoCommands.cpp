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

#include <typeinfo>

#include "CalypsoPoCommands.h"
#include "PoRevision.h"

/* Common */
#include "exceptionhelper.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {

using namespace keyple::common;
using namespace keyple::core::command;

CalypsoPoCommands CalypsoPoCommands::GET_DATA_FCI("GET_DATA_FCI",
                                                  InnerEnum::GET_DATA_FCI,
                                                  "Get Data'FCI'", 0xCA);
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_10("OPEN_SESSION_10",
                                                     InnerEnum::OPEN_SESSION_10,
                                                     "Open Secure Session V1",
                                                     0x8A);
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_24("OPEN_SESSION_24",
                                                     InnerEnum::OPEN_SESSION_24,
                                                     "Open Secure Session V2.4",
                                                     0x8A);
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_31("OPEN_SESSION_31",
                                                     InnerEnum::OPEN_SESSION_31,
                                                     "Open Secure Session V3.1",
                                                     0x8A);
CalypsoPoCommands CalypsoPoCommands::OPEN_SESSION_32("OPEN_SESSION_32",
                                                     InnerEnum::OPEN_SESSION_32,
                                                     "Open Secure Session V3.2",
                                                     0x8A);
CalypsoPoCommands CalypsoPoCommands::CLOSE_SESSION("CLOSE_SESSION",
                                                   InnerEnum::CLOSE_SESSION,
                                                   "Close Secure Session",
                                                   0x8E);
CalypsoPoCommands CalypsoPoCommands::READ_RECORDS("READ_RECORDS",
                                                  InnerEnum::READ_RECORDS,
                                                  "Read Records", 0xB2);
CalypsoPoCommands CalypsoPoCommands::UPDATE_RECORD("UPDATE_RECORD",
                                                   InnerEnum::UPDATE_RECORD,
                                                   "Update Record", 0xDC);
CalypsoPoCommands CalypsoPoCommands::WRITE_RECORD("WRITE_RECORD",
                                                  InnerEnum::WRITE_RECORD,
                                                  "Write Record", 0xD2);
CalypsoPoCommands CalypsoPoCommands::APPEND_RECORD("APPEND_RECORD",
                                                   InnerEnum::APPEND_RECORD,
                                                   "Append Record", 0xE2);
CalypsoPoCommands CalypsoPoCommands::GET_CHALLENGE("GET_CHALLENGE",
                                                   InnerEnum::GET_CHALLENGE,
                                                   "Get Challenge", 0x84);
CalypsoPoCommands CalypsoPoCommands::INCREASE("INCREASE", InnerEnum::INCREASE,
                                              "Increase", 0x32);
CalypsoPoCommands CalypsoPoCommands::DECREASE("DECREASE", InnerEnum::DECREASE,
                                              "Decrease", 0x30);
CalypsoPoCommands CalypsoPoCommands::SELECT_FILE("SELECT_FILE",
                                                 InnerEnum::SELECT_FILE,
                                                 "Select File", 0xA4);
CalypsoPoCommands CalypsoPoCommands::CHANGE_KEY("CHANGE_KEY",
                                                InnerEnum::CHANGE_KEY,
                                                "Change Key", 0xD8);

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

CalypsoPoCommands::CalypsoPoCommands(const std::string& nameValue,
                                     InnerEnum innerEnum,
                                     const std::string& name,
                                     uint8_t instructionByte)
: innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++),
  name(name), instructionbyte(instructionByte)
{
}

CalypsoPoCommands::CalypsoPoCommands(const CalypsoPoCommands& other)
: CommandsTable(), innerEnumValue(other.innerEnumValue),
  nameValue(other.nameValue), ordinalValue(other.ordinalValue),
  name(other.name), instructionbyte(other.instructionbyte)
{
    (void)other;
}

const std::string& CalypsoPoCommands::getName() const
{
    return name;
}

uint8_t CalypsoPoCommands::getInstructionByte() const
{
    return instructionbyte;
}

CalypsoPoCommands& CalypsoPoCommands::getOpenSessionForRev(PoRevision rev)
{
    switch (rev) {
    case PoRevision::REV1_0:
        return OPEN_SESSION_10;
    case PoRevision::REV2_4:
        return OPEN_SESSION_24;
    case PoRevision::REV3_1:
    case PoRevision::REV3_1_CLAP:
        return OPEN_SESSION_31;
    case PoRevision::REV3_2:
        return OPEN_SESSION_32;
    default:
        throw IllegalStateException("Any revision should have a matching "
                                    "command");
    }
}

bool CalypsoPoCommands::operator==(const CalypsoPoCommands& other)
{
    return this->ordinalValue == other.ordinalValue;
}

bool CalypsoPoCommands::operator!=(const CalypsoPoCommands& other)
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

CalypsoPoCommands CalypsoPoCommands::valueOf(const std::string& name)
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

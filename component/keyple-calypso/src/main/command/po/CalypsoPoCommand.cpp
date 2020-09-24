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

#include "CalypsoPoCommand.h"

/* Common */
#include "IllegalStateException.h"

/* Calypso */
#include "PoRevision.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {

using namespace keyple::common;
using namespace keyple::core::command;

const CalypsoPoCommand CalypsoPoCommand::GET_DATA_FCI(
    "Get Data'FCI'", 0xCA);
const CalypsoPoCommand CalypsoPoCommand::OPEN_SESSION_10(
    "Open Secure Session V1", 0x8A);
const CalypsoPoCommand CalypsoPoCommand::OPEN_SESSION_24(
    "Open Secure Session V2.4", 0x8A);
const CalypsoPoCommand CalypsoPoCommand::OPEN_SESSION_31(
    "Open Secure Session V3.1", 0x8A);
const CalypsoPoCommand CalypsoPoCommand::OPEN_SESSION_32(
    "Open Secure Session V3.2", 0x8A);
const CalypsoPoCommand CalypsoPoCommand::CLOSE_SESSION(
    "Close Secure Session", 0x8E);
const CalypsoPoCommand CalypsoPoCommand::READ_RECORDS(
    "Read Records", 0xB2);
const CalypsoPoCommand CalypsoPoCommand::UPDATE_RECORD(
    "Update Record", 0xDC);
const CalypsoPoCommand CalypsoPoCommand::WRITE_RECORD(
    "Write Record", 0xD2);
const CalypsoPoCommand CalypsoPoCommand::APPEND_RECORD(
    "Append Record", 0xE2);
const CalypsoPoCommand CalypsoPoCommand::GET_CHALLENGE(
    "Get Challenge", 0x84);
const CalypsoPoCommand CalypsoPoCommand::INCREASE(
    "Increase", 0x32);
const CalypsoPoCommand CalypsoPoCommand::DECREASE(
    "Decrease", 0x30);
const CalypsoPoCommand CalypsoPoCommand::SELECT_FILE(
    "Select File", 0xA4);
const CalypsoPoCommand CalypsoPoCommand::CHANGE_KEY(
    "Change Key", 0xD8);
const CalypsoPoCommand CalypsoPoCommand::GET_DATA_TRACE(
    "Get Data 'Trace'", 0xD8);

CalypsoPoCommand::CalypsoPoCommand(const std::string& name,
                                   const uint8_t instructionByte)
: mName(name), mInstructionbyte(instructionByte) {}

const std::string& CalypsoPoCommand::getName() const
{
    return mName;
}

uint8_t CalypsoPoCommand::getInstructionByte() const
{
    return mInstructionbyte;
}

const CalypsoPoCommand& CalypsoPoCommand::getOpenSessionForRev(PoRevision rev)
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

bool CalypsoPoCommand::operator==(const CalypsoPoCommand& other) const
{
    return this->mInstructionbyte == other.mInstructionbyte &&
           this->mName == other.mName;
}

bool CalypsoPoCommand::operator!=(const CalypsoPoCommand& other) const
{
    return !(*this == other);
}

}
}
}
}

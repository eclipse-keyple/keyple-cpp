/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "CalypsoSamCommand.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

const CalypsoSamCommand CalypsoSamCommand::SELECT_DIVERSIFIER("Select Diversifier", 0x14);
const CalypsoSamCommand CalypsoSamCommand::GET_CHALLENGE("Get Challenge", 0x84);
const CalypsoSamCommand CalypsoSamCommand::DIGEST_INIT("Digest Init", 0x8A);
const CalypsoSamCommand CalypsoSamCommand::DIGEST_UPDATE("Digest Update", 0x8C);
const CalypsoSamCommand CalypsoSamCommand::DIGEST_UPDATE_MULTIPLE("Digest Update Multiple", 0x8C);
const CalypsoSamCommand CalypsoSamCommand::DIGEST_CLOSE("Digest Close", 0x8E);
const CalypsoSamCommand CalypsoSamCommand::DIGEST_AUTHENTICATE("Digest Authenticate", 0x82);
const CalypsoSamCommand CalypsoSamCommand::GIVE_RANDOM("Give Random", 0x86);
const CalypsoSamCommand CalypsoSamCommand::CARD_GENERATE_KEY("Card Generate Key", 0x12);
const CalypsoSamCommand CalypsoSamCommand::CARD_CIPHER_PIN("Card Cipher Pin", 0x12);
const CalypsoSamCommand CalypsoSamCommand::UNLOCK("Unlock", 0x20);
const CalypsoSamCommand CalypsoSamCommand::WRITE_KEY("Write Key", 0x1A);
const CalypsoSamCommand CalypsoSamCommand::READ_KEY_PARAMETERS("Read Key Parameters", 0xBC);
const CalypsoSamCommand CalypsoSamCommand::READ_EVENT_COUNTER("Read Event Counter", 0xBE);
const CalypsoSamCommand CalypsoSamCommand::READ_CEILINGS( "Read Ceilings", 0xBE);
const CalypsoSamCommand CalypsoSamCommand::SV_CHECK("SV Check", 0x58);
const CalypsoSamCommand CalypsoSamCommand::SV_PREPARE_DEBIT("SV Prepare Debit", 0x54);
const CalypsoSamCommand CalypsoSamCommand::SV_PREPARE_LOAD("SV Prepare Load", 0x56);
const CalypsoSamCommand CalypsoSamCommand::SV_PREPARE_UNDEBIT("SV Prepare Undebit", 0x5C);

CalypsoSamCommand::CalypsoSamCommand(const std::string& name, const uint8_t instructionByte)
: mName(name), mInstructionByte(instructionByte) {}

const std::string& CalypsoSamCommand::getName() const
{
    return mName;
}

uint8_t CalypsoSamCommand::getInstructionByte() const
{
    return mInstructionByte;
}

}
}
}
}

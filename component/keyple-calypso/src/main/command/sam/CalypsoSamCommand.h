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

#pragma once

#include <type_traits>

/* Core */
#include "SeCommand.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::core::command;

class CalypsoSamCommand final : public SeCommand {
public:
    /**
     * The sam select diversifier
     */
    static const CalypsoSamCommand SELECT_DIVERSIFIER;

    /**
     * The sam get challenge
     */
    static const CalypsoSamCommand GET_CHALLENGE;

    /**
     * The sam digest init
     */
    static const CalypsoSamCommand DIGEST_INIT;

    /**
     * The sam digest update
     */
    static const CalypsoSamCommand DIGEST_UPDATE;

    /**
     * The sam digest update multiple
     */
    static const CalypsoSamCommand DIGEST_UPDATE_MULTIPLE;

    /**
     * The sam digest close
     */
    static const CalypsoSamCommand DIGEST_CLOSE;

    /**
     * The sam digest authenticate
     */
    static const CalypsoSamCommand DIGEST_AUTHENTICATE;

    /**
     * The sam digest authenticate
     */
    static const CalypsoSamCommand GIVE_RANDOM;

    /**
     * The sam digest authenticate
     */
    static const CalypsoSamCommand CARD_GENERATE_KEY;

    /**
     * The sam card cipher PIN
     */
    static const CalypsoSamCommand CARD_CIPHER_PIN;

    /**
     * The sam unlock
     */
    static const CalypsoSamCommand UNLOCK;

    /**
     * The sam write key command
     */
    static const CalypsoSamCommand WRITE_KEY;

    /**
     *
     */
    static const CalypsoSamCommand READ_KEY_PARAMETERS;

    /**
     *
     */
    static const CalypsoSamCommand READ_EVENT_COUNTER;

    /**
     *
     */
    static const CalypsoSamCommand READ_CEILINGS;

    /**
     *
     */
    static const CalypsoSamCommand SV_CHECK;

    /**
     *
     */
    static const CalypsoSamCommand SV_PREPARE_DEBIT;

    /**
     *
     */
    static const CalypsoSamCommand SV_PREPARE_LOAD;

    /**
     *
     */
    static const CalypsoSamCommand SV_PREPARE_UNDEBIT;

    /**
     * Gets the name.
     *
     * @return the command name
     */
    const std::string& getName() const;

    /**
     * Gets the instruction byte.
     *
     * @return the value of INS byte
     */
    uint8_t getInstructionByte() const;

private:
    /**
     * The name
     */
    const std::string mName;

    /**
     * The instruction byte
     */
    const uint8_t mInstructionByte;

    /**
     * The generic constructor of CalypsoCommands.
     *
     * @param name the name
     * @param instructionByte the instruction byte
     */
    CalypsoSamCommand(const std::string& name, const uint8_t instructionByte);
};

}
}
}
}

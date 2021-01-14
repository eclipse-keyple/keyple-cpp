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

#include <string>
#include <vector>
#include <memory>

/* Core */
#include "SeCommand.h"

/* Calypso */
#include "KeypleCalypsoExport.h"
#include "PoRevision.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {

using namespace keyple::core::command;

class KEYPLECALYPSO_API CalypsoPoCommand final : public SeCommand {
public:
    /** The po get data. */
    static const CalypsoPoCommand GET_DATA_FCI;

    /** The po open session. */
    static const CalypsoPoCommand OPEN_SESSION_10;

    /** The po open session. */
    static const CalypsoPoCommand OPEN_SESSION_24;

    /** The po open session. */
    static const CalypsoPoCommand OPEN_SESSION_31;

    /** The po open session. */
    static const CalypsoPoCommand OPEN_SESSION_32;

    /** The po close session. */
    static const CalypsoPoCommand CLOSE_SESSION;

    /** The po read records. */
    static const CalypsoPoCommand READ_RECORDS;

    /** The po update record. */
    static const CalypsoPoCommand UPDATE_RECORD;

    /** The po write record. */
    static const CalypsoPoCommand WRITE_RECORD;

    /** The po append record. */
    static const CalypsoPoCommand APPEND_RECORD;

    /** The po get challenge. */
    static const CalypsoPoCommand GET_CHALLENGE;

    /** The po increase counter. */
    static const CalypsoPoCommand INCREASE;

    /** The po decrease counter. */
    static const CalypsoPoCommand DECREASE;

    /** The po decrease counter. */
    static const CalypsoPoCommand SELECT_FILE;

    /** The po change key */
    static const CalypsoPoCommand CHANGE_KEY;

    /* The po verify PIN */
    static const CalypsoPoCommand VERIFY_PIN;

    /* The po get data for traceability */
    static const CalypsoPoCommand GET_DATA_TRACE;

    /* The po SV Get */
    static const CalypsoPoCommand SV_GET;

    /* The po SV Debit */
    static const CalypsoPoCommand SV_DEBIT;

    /* The po SV Reload */
    static const CalypsoPoCommand SV_RELOAD;

    /* The po SV Undebit */
    static const CalypsoPoCommand SV_UNDEBIT;

    /* The po Invalidate */
    static const CalypsoPoCommand INVALIDATE;

    /* The po Rehabilitate */
    static const CalypsoPoCommand REHABILITATE;

    /**
     * The generic constructor of CalypsoCommands.
     *
     * @param name the name
     * @param instructionByte the instruction byte
     */
    CalypsoPoCommand(const std::string& name, const uint8_t instructionByte);

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

    /**
     * Get the right open-session command for a given keyple::calypso::command::po::PoRevision
     *
     * @param rev Command revision
     * @return Returned command
     */
    static const CalypsoPoCommand& getOpenSessionForRev(PoRevision rev);

    /**
     *
     */
    bool operator==(const CalypsoPoCommand& other) const;

    /**
     *
     */
    bool operator!=(const CalypsoPoCommand& other) const;

private:
    /**
     * The name
     */
    const std::string mName;

    /**
     * The instruction byte
     */
    const char mInstructionbyte;
};

}
}
}
}

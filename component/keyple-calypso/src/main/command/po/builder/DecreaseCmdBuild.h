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

#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommand.h"
#include "PoClass.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace po {
    namespace parser { class DecreaseRespPars; } } } } }

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

/**
 * The Class DecreaseCmdBuild. This class provides the dedicated constructor to
 * build the Decrease APDU command.
 *
 */
class KEYPLECALYPSO_API DecreaseCmdBuild final
: public AbstractPoCommandBuilder<DecreaseRespPars> {
public:
    /**
     * Instantiates a new decrease cmd build from command parameters.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param sfi SFI of the file to select or 00h for current EF
     * @param counterNumber &gt;= 01h: Counters file, number of the counter.
     *        00h: Simulated Counter file.
     * @param decValue Value to subtract to the counter (defined as a positive
     *        int &lt;= 16777215 [FFFFFFh])
     * @throw IllegalArgumentException - if the decrement value is out of range
     * @throw IllegalArgumentException - if the command is inconsistent
     */
    DecreaseCmdBuild(const PoClass poClass,
                     const uint8_t sfi,
                     const uint8_t counterNumber,
                     const int decValue);

    /**
     *
     */
    std::shared_ptr<DecreaseRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     * This command can modify the contents of the PO in session and therefore
     * uses the session buffer.
     *
     * @return true
     */
    bool isSessionBufferUsed() const override;

    /**
     * @return the SFI of the accessed file
     */
    uint8_t getSfi() const;

    /**
     * @return the counter number
     */
    int getCounterNumber() const;

    /**
     * @return the decrement value
     */
    int getDecValue() const;

private:
    /**
     * The command
     */
    const CalypsoPoCommand& command = CalypsoPoCommand::DECREASE;

    /**
     * Construction arguments
     */
    const uint8_t mSfi;
    const uint8_t mCounterNumber;
    const int mDecValue;
};

}
}
}
}
}

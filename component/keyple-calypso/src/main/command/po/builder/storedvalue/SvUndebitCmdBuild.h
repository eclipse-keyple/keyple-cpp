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

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "PoClass.h"
#include "PoTransaction.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace po {
    namespace parser { namespace storedvalue { class SvUndebitRespPars; } } } } } }

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace storedvalue {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser::storedvalue;
using namespace keyple::calypso::transaction;

using SvSettings = PoTransaction::SvSettings;
using Operation = PoTransaction::SvSettings::Operation;

/**
 * The Class SvUndebitCmdBuild. This class provides the dedicated constructor to build the SV
 * Undebit command. Note: {@link SvUndebitCmdBuild} and {@link SvDebitCmdBuild} shares the same
 * parser {@link SvDebitRespPars}
 */
class SvUndebitCmdBuild final : public AbstractPoCommandBuilder<SvUndebitRespPars> {
public:
    /**
     * Instantiates a new SvUndebitCmdBuild.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param poRevision the PO revision
     * @param amount amount to undebit (positive integer from 0 to 32767)
     * @param kvc the KVC
     * @param date debit date (not checked by the PO)
     * @param time debit time (not checked by the PO)
     * @throws IllegalArgumentException - if the command is inconsistent
     */
    SvUndebitCmdBuild(const PoClass& poClass,
                     const PoRevision& poRevision,
                     const int amount,
                     const uint8_t kvc,
                     const std::vector<uint8_t>& date,
                     const std::vector<uint8_t>& time);

    /**
      * Complete the construction of the APDU to be sent to the PO with the elements received from
     * the SAM:
     * <p>
     * 4-byte SAM id
     * <p>
     * 3-byte challenge
     * <p>
     * 3-byte transaction number
     * <p>
     * 5 or 10 byte signature (hi part)
     *
     *
     * @param undebitComplementaryData the data out from the SvPrepareDebit SAM command
     */
    void finalizeBuilder(const std::vector<uint8_t>& undebitComplementaryData);

    /**
     * Gets the SV Undebit part of the data to include in the SAM SV Prepare Debit command
     *
     * @return a byte array containing the SV undebit data
     */
    const std::vector<uint8_t> getSvUndebitData() const;

    /**
     * Create the response parser.
     * <p>
     * A check is made to see if the object has been finalized. If not, an exception
     * {@link IllegalStateException} is thrown.
     *
     * @param apduResponse the response data from the SE
     * @return a {@link SvUndebitRespPars} object
     */
    std::shared_ptr<SvUndebitRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     *
     */
    bool isSessionBufferUsed() const override;

private:
    /**
     * The command
     */
    const CalypsoPoCommand& mCommand = CalypsoPoCommand::SV_RELOAD;

    /**
     *
     */
    const PoClass mPoClass;

    /**
     *
     */
    const PoRevision mPoRevision;

    /**
     *
     */
    std::vector<uint8_t> mDataIn;
};

}
}
}
}
}
}

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
#include "KeypleCalypsoExport.h"
#include "PoClass.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace po {
    namespace parser { namespace storedvalue { class SvDebitRespPars; } } } } } }

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace storedvalue {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser::storedvalue;

/**
 * The Class SvDebitCmdBuild. This class provides the dedicated constructor to build the SV Debit
 * command. Note: {@link SvDebitCmdBuild} and {@link SvUndebitCmdBuild} shares the same parser
 * {@link SvDebitRespPars}
 */
class KEYPLECALYPSO_API SvDebitCmdBuild final : public AbstractPoCommandBuilder<SvDebitRespPars> {
public:
    /**
     * Instantiates a new SvDebitCmdBuild.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param poRevision the PO revision
     * @param amount amount to debit (positive integer from 0 to 32767)
     * @param kvc the KVC
     * @param date debit date (not checked by the PO)
     * @param time debit time (not checked by the PO)
     * @throw IllegalArgumentException - if the command is inconsistent
     */
    SvDebitCmdBuild(const PoClass& poClass,
                    const PoRevision& poRevision,
                    const int amount,
                    const uint8_t kvc,
                    const std::vector<uint8_t> date,
                    const std::vector<uint8_t> time);


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
     * @param debitComplementaryData the data out from the SvPrepareDebit SAM command
     */
    void finalizeBuilder(const std::vector<uint8_t> debitComplementaryData);

    /**
     * Gets the SV Debit part of the data to include in the SAM SV Prepare Debit command
     *
     * @return a byte array containing the SV debit data
     */
    const std::vector<uint8_t> getSvDebitData() const;

    /**
     *
     */
    std::shared_ptr<SvDebitRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     *
     */
    virtual bool isSessionBufferUsed() const override;

private:
    /**
     * The command
     */
    const CalypsoPoCommand& mCommand = CalypsoPoCommand::SV_DEBIT;

    /**
     *
     */
    const PoClass mPoClass;

    /**
     *
     */
    const PoRevision mPoRevision;

    /**
     * Apdu data array
     */
    std::vector<uint8_t> mDataIn;
};

}
}
}
}
}
}

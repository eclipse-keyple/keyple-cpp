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
    namespace parser { namespace storedvalue { class SvReloadRespPars; } } } } } }

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
 * The Class SvReloadCmdBuild. This class provides the dedicated constructor to build the SV Reload
 * command.
 */
class SvReloadCmdBuild final : public AbstractPoCommandBuilder<SvReloadRespPars> {
public:
    /**
     * Instantiates a new SvReloadCmdBuild.
     * <p>
     * The process is carried out in two steps: first to check and store the PO and application
     * data, then to create the final APDU with the data from the SAM (see finalizeBuilder).
     *
     * @param poClass the PO class
     * @param poRevision the PO revision
     * @param amount amount to debit (signed integer from -8388608 to 8388607)
     * @param kvc debit key KVC (not checked by the PO)
     * @param date debit date (not checked by the PO)
     * @param time debit time (not checked by the PO)
     * @param free 2 free bytes stored in the log but not processed by the PO
     * @throw IllegalArgumentException - if the command is inconsistent
     */
    SvReloadCmdBuild(const PoClass& poClass,
                     const PoRevision& poRevision,
                     const int amount,
                     const uint8_t kvc,
                     const std::vector<uint8_t>& date,
                     const std::vector<uint8_t>& time,
                     const std::vector<uint8_t>& free);

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
     * @param reloadComplementaryData the sam id and the data out from the SvPrepareReload SAM
     *        command
     */
    void finalizeBuilder(const std::vector<uint8_t>& reloadComplementaryData);

    /**
     * Gets the SV Reload part of the data to include in the SAM SV Prepare Load command
     *
     * @return a byte array containing the SV reload data
     */
    const std::vector<uint8_t> getSvReloadData() const;

    /**
     * Create the response parser.
     * <p>
     * A check is made to see if the object has been finalized. If not, an exception
     * {@link IllegalStateException} is thrown.
     *
     * @param apduResponse the response data from the SE
     * @return a {@link SvReloadRespPars} object
     */
    std::shared_ptr<SvReloadRespPars> createResponseParser(
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

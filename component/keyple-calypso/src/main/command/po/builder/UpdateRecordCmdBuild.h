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
#include "PoModificationCommand.h"
#include "PoSendableInSession.h"
#include "PoClass.h"
#include "SelectFileRespPars.h"
#include "UpdateRecordRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser;

/**
 * The Class UpdateRecordCmdBuild. This class provides the dedicated constructor
 * to build the Update Record APDU command.
 */
class KEYPLECALYPSO_API UpdateRecordCmdBuild final
: public AbstractPoCommandBuilder<UpdateRecordRespPars> {
public:
    /**
     * Instantiates a new UpdateRecordCmdBuild.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param sfi the sfi to select
     * @param recordNumber the record number to update
     * @param newRecordData the new record data to write
     * @throw IllegalArgumentException - if record number is &lt; 1
     * @throw IllegalArgumentException - if the request is inconsistent
     */
    UpdateRecordCmdBuild(const PoClass poClass, const uint8_t sfi,
                         const uint8_t recordNumber,
                         const std::vector<uint8_t>& newRecordData);

    /**
     *
     */
    std::shared_ptr<UpdateRecordRespPars> createResponseParser(
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
     * @return the number of the accessed record
     */
    uint8_t getRecordNumber() const;

    /**
     * @return the data sent to the PO
     */
    const std::vector<uint8_t>& getData() const;

protected:
    /**
     *
     */
    std::shared_ptr<UpdateRecordCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<UpdateRecordCmdBuild>(
            AbstractPoCommandBuilder<UpdateRecordRespPars>::shared_from_this());
    }

private:
    /**
     * The command
     */
    CalypsoPoCommand& command = CalypsoPoCommand::UPDATE_RECORD;

    /**
     * Construction arguments
     */
    const uint8_t mSfi;
    const uint8_t mRecordNumber;
    const std::vector<uint8_t< mData;
};

}
}
}
}
}

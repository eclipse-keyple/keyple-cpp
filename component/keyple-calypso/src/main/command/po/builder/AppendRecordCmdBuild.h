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
#include <memory>

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "AppendRecordRespPars.h"
#include "CalypsoPoCommand.h"
#include "PoModificationCommand.h"
#include "PoSendableInSession.h"
#include "PoClass.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

/**
 * The Class AppendRecordCmdBuild. This class provides the dedicated constructor
 * to build the Update Record APDU command.
 */
class KEYPLECALYPSO_API AppendRecordCmdBuild final
: public AbstractPoCommandBuilder<AppendRecordRespPars> {
public:
    /**
     * Instantiates a new UpdateRecordCmdBuild.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param sfi the sfi to select
     * @param newRecordData the new record data to write
     * @throw IllegalArgumentException - if the command is inconsistent
     */
    AppendRecordCmdBuild(PoClass poClass, uint8_t sfi,
                         const std::vector<uint8_t>& newRecordData);

    /**
     *
     */
    std::shared_ptr<AppendRecordRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     *
     * This command can modify the contents of the PO in session and therefore
     * uses the session buffer.
     *
     * @return true
     */
    virtual bool isSessionBufferUsed() const override;

    /**
     * @return the SFI of the accessed file
     */
    uint8_t getSfi() const;

    /**
     * @return the data sent to the PO
     */
    const std::vector<uint8_t>& getData() const;

protected:
    /**
     *
     */
    std::shared_ptr<AppendRecordCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<AppendRecordCmdBuild>(
            AbstractPoCommandBuilder<AppendRecordRespPars>::shared_from_this());
    }

private:
    /**
     * The command
     */
    CalypsoPoCommands& command = CalypsoPoCommands::APPEND_RECORD;

    /**
     * Construction arguments
     */
    const uint8_t mSfi;
    const std::vector<uint8_t> mData;
};

}
}
}
}
}

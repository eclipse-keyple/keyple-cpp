/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
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

/* Calypso */
#include "PoClass.h"
#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommands.h"
#include "PoModificationCommand.h"
#include "PoSendableInSession.h"
#include "WriteRecordRespPars.h"

/* Core */
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser;

/**
 * The Class WriteRecordCmdBuild. This class provides the dedicated constructor
 * to build the Write Record APDU command.
 */
class WriteRecordCmdBuild
: public AbstractPoCommandBuilder<WriteRecordRespPars>,
  public PoSendableInSession, public PoModificationCommand {
public:
    /**
     * Instantiates a new WriteRecordCmdBuild.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param sfi the sfi to select
     * @param recordNumber the record number to write
     * @param newRecordData the new record data to write
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @throws IllegalArgumentException - if record number is &lt; 1
     * @throws IllegalArgumentException - if the request is inconsistent
     */
    WriteRecordCmdBuild(PoClass poClass, uint8_t sfi, uint8_t recordNumber,
                        const std::vector<uint8_t>& newRecordData,
                        const std::string& extraInfo);

    /**
     *
     */
    std::shared_ptr<WriteRecordRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

protected:
    /**
     *
     */
    std::shared_ptr<WriteRecordCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<WriteRecordCmdBuild>(
                   AbstractPoCommandBuilder<WriteRecordRespPars>
                       ::shared_from_this());
    }

private:
    /**
     * The command
     */
    CalypsoPoCommands& command = CalypsoPoCommands::WRITE_RECORD;


};

}
}
}
}
}

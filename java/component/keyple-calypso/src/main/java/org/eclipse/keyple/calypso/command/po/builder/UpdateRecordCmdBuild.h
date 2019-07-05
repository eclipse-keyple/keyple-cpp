/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommands.h"
#include "PoModificationCommand.h"
#include "PoSendableInSession.h"
#include "PoClass.h"
#include "SelectFileRespPars.h"
#include "UpdateRecordRespPars.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace org::eclipse::keyple::calypso::command::po;

using PoClass              = org::eclipse::keyple::calypso::command::PoClass;
using UpdateRecordRespPars = org::eclipse::keyple::calypso::command::po::parser::UpdateRecordRespPars;
using ApduResponse         = org::eclipse::keyple::core::seproxy::message::ApduResponse;
using SelectFileResPars    = org::eclipse::keyple::calypso::command::po::parser::SelectFileRespPars;

/**
 * The Class UpdateRecordCmdBuild. This class provides the dedicated constructor to build the Update
 * Record APDU command.
 *
 */
class UpdateRecordCmdBuild final
: public AbstractPoCommandBuilder<UpdateRecordRespPars>, public PoSendableInSession,
  public PoModificationCommand {

    /** The command. */
private:
    const CalypsoPoCommands command = CalypsoPoCommands::UPDATE_RECORD;

    /**
     * Instantiates a new UpdateRecordCmdBuild.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param sfi the sfi to select
     * @param recordNumber the record number to update
     * @param newRecordData the new record data to write
     * @param extraInfo extra information included in the logs (can be null or empty)
     * @throws IllegalArgumentException - if record number is &lt; 1
     * @throws IllegalArgumentException - if the request is inconsistent
     */
public:
    UpdateRecordCmdBuild(PoClass poClass, char sfi, char recordNumber, std::vector<char> &newRecordData, const std::string &extraInfo);
std::shared_ptr<UpdateRecordRespPars> createResponseParser(std::shared_ptr<ApduResponse> apduResponse) override;

protected:
    std::shared_ptr<UpdateRecordCmdBuild> shared_from_this() {
        return std::static_pointer_cast<UpdateRecordCmdBuild>(AbstractPoCommandBuilder<UpdateRecordRespPars>::shared_from_this());
    }
};

}
}
}
}
}
}
}

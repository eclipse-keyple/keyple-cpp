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

#include <memory>
#include <vector>

#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommands.h"
#include "PoClass.h"
#include "PoSendableInSession.h"
#include "SelectFileRespPars.h"

#include "exceptionhelper.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace org::eclipse::keyple::calypso::command::po;

using PoClass                  = org::eclipse::keyple::calypso::command::PoClass;
using CalypsoPoCommands        = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
using PoSendableInSession      = org::eclipse::keyple::calypso::command::po::PoSendableInSession;
using SelectFileRespPars       = org::eclipse::keyple::calypso::command::po::parser::SelectFileRespPars;
using ApduResponse             = org::eclipse::keyple::core::seproxy::message::ApduResponse;

/**
 * This class provides the dedicated constructor to build the Select File APDU commands.
 *
 */
class SelectFileCmdBuild final
: public AbstractPoCommandBuilder<SelectFileRespPars>, public PoSendableInSession {

private:
    const CalypsoPoCommands command = CalypsoPoCommands::SELECT_FILE;

public:
    enum class SelectControl {
        FIRST,
        NEXT,
        CURRENT_DF
    };

    /**
     * Instantiates a new SelectFileCmdBuild to select the first, next or current file in the
     * current DF.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param selectControl the selection mode control: FIRST, NEXT or CURRENT
     */
public:
    SelectFileCmdBuild(PoClass poClass, SelectControl selectControl);

    /**
     * Instantiates a new SelectFileCmdBuild to select the first, next or current file in the
     * current DF.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param selectionPath the file identifier path
     */
    SelectFileCmdBuild(PoClass poClass, std::vector<char> &selectionPath);

    std::shared_ptr<SelectFileRespPars> createResponseParser(std::shared_ptr<ApduResponse> apduResponse) override;

protected:
    std::shared_ptr<SelectFileCmdBuild> shared_from_this() {
        return std::static_pointer_cast<SelectFileCmdBuild>(AbstractPoCommandBuilder<SelectFileRespPars>::shared_from_this());
    }
};

}
}
}
}
}
}
}

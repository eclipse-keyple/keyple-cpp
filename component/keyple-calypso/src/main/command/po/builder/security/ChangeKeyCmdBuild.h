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

#include <vector>
#include <stdexcept>
#include <memory>

#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommands.h"
#include "ChangeKeyRespPars.h"
#include "PoClass.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser::security;
using namespace keyple::core::seproxy::message;

class ChangeKeyCmdBuild : public AbstractPoCommandBuilder<ChangeKeyRespPars> {
private:
    /**
     *
     */
    CalypsoPoCommands& command = CalypsoPoCommands::CHANGE_KEY;

public:
    /**
     * Change Key Calypso command
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param keyIndex index of the key of the current DF to change
     * @param cryptogram key encrypted with Issuer key (key #1)
     */
    ChangeKeyCmdBuild(PoClass poClass, uint8_t keyIndex,
                      std::vector<uint8_t>& cryptogram);

    /**
     *
     */
    std::shared_ptr<ChangeKeyRespPars>
        createResponseParser(std::shared_ptr<ApduResponse> apduResponse)
        override;

protected:
    /**
     *
     */
    std::shared_ptr<ChangeKeyCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<ChangeKeyCmdBuild>(
                   AbstractPoCommandBuilder<ChangeKeyRespPars>
                       ::shared_from_this());
    }
};

}
}
}
}
}
}

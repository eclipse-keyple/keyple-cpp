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

#include "OpenSession24CmdBuild.h"
#include "OpenSession24RespPars.h"
#include "PoClass.h"
#include "CalypsoPoCommands.h"
#include "PoRevision.h"

/* Core */
#include "ApduResponse.h"

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

OpenSession24CmdBuild::OpenSession24CmdBuild(
    uint8_t keyIndex, const std::vector<uint8_t>& samChallenge,
    uint8_t sfiToSelect, uint8_t recordNumberToRead,
    const std::string& extraInfo)
: AbstractOpenSessionCmdBuild<OpenSession24RespPars>(PoRevision::REV2_4)
{

    if (keyIndex == 0x00) {
        throw std::invalid_argument("Key index can't be null for rev 2.4!");
    }

    uint8_t p1 = 0x80 + (recordNumberToRead * 8) + keyIndex;
    uint8_t p2 = sfiToSelect * 8;

    /*
     * case 4: this command contains incoming and outgoing data. We define
     * le = 0, the actual length will be processed by the lower layers.
     */
    uint8_t le = 0;

    this->request = setApduRequest(
        PoClass::LEGACY.getValue(),
        CalypsoPoCommands::getOpenSessionForRev(PoRevision::REV2_4), p1, p2,
        samChallenge, le);
    if (extraInfo != "") {
        this->addSubName(extraInfo);
    }
}

std::shared_ptr<OpenSession24RespPars>
OpenSession24CmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<OpenSession24RespPars>(apduResponse);
}

}
}
}
}
}
}

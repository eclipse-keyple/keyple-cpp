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

/* Common */
#include "System.h"

/* Calypso */
#include "OpenSession32CmdBuild.h"
#include "OpenSession32RespPars.h"
#include "PoClass.h"
#include "CalypsoPoCommands.h"
#include "PoRevision.h"
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

OpenSession32CmdBuild::OpenSession32CmdBuild(
    uint8_t keyIndex, const std::vector<uint8_t>& samChallenge,
    uint8_t sfiToSelect, uint8_t recordNumberToRead,
    const std::string& extraInfo)
: AbstractOpenSessionCmdBuild<OpenSession32RespPars>(PoRevision::REV3_2)
{

    uint8_t p1 = (recordNumberToRead * 8) + keyIndex;
    uint8_t p2 = (sfiToSelect * 8) + 2;

    /*
     * case 4: this command contains incoming and outgoing data. We define
     * le = 0, the actual  length will be processed by the lower layers.
     */
    uint8_t le = 0;

    std::vector<uint8_t> dataIn(samChallenge.size() + 1);
    dataIn[0] = 0x00;
    System::arraycopy(samChallenge, 0, dataIn, 1, samChallenge.size());

    this->request = setApduRequest(
        PoClass::ISO.getValue(),
        CalypsoPoCommands::getOpenSessionForRev(PoRevision::REV3_2), p1, p2,
        dataIn, le);
    if (extraInfo != "") {
        this->addSubName(extraInfo);
    }
}

std::shared_ptr<OpenSession32RespPars>
OpenSession32CmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<OpenSession32RespPars>(apduResponse);
}
}
}
}
}
}
}

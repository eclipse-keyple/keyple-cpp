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

#include "OpenSession31CmdBuild.h"
#include "OpenSession31RespPars.h"
#include "PoClass.h"
#include "CalypsoPoCommand.h"
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

OpenSession31CmdBuild::OpenSession31CmdBuild(
  const uint8_t keyIndex,
  const std::vector<uint8_t>& samChallenge,
  const uint8_t sfi,
  const uint8_t recordNumber)
: AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>(PoRevision::REV3_1),
  mSfi(sfi),
  mRecordNumber(recordNumber)
{
    const uint8_t p1 = (recordNumber * 8) + keyIndex;
    const uint8_t p2 = (sfi * 8) + 1;

    /*
     * case 4: this command contains incoming and outgoing data. We define
     * le = 0, the actual length will be processed by the lower layers.
     */
    const uint8_t le = 0;

    mRequest = setApduRequest(
                     PoClass::ISO.getValue(),
                     CalypsoPoCommand::getOpenSessionForRev(PoRevision::REV3_1),
                     p1,
                     p2,
                     samChallenge,
                     le);

    const std::string extraInfo =
        StringHelper::formatSimple(
            "KEYINDEX=%d, SFI=%02X, REC=%d", keyIndex, sfi, recordNumber);

    addSubName(extraInfo);
}

std::shared_ptr<AbstractOpenSessionRespPars>
    OpenSession31CmdBuild::createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<OpenSession31RespPars>(apduResponse, this);
}

bool OpenSession31CmdBuild::isSessionBufferUsed() const
{
    return false;
}

uint8_t OpenSession31CmdBuild::getSfi() const
{
    return mSfi;
}

uint8_t OpenSession31CmdBuild::getRecordNumber() const
{
    return mRecordNumber;
}

}
}
}
}
}
}

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

#include "OpenSession10CmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"

/* Calypso */
#include "OpenSession10RespPars.h"
#include "PoClass.h"
#include "CalypsoPoCommand.h"
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
using namespace keyple::common;
using namespace keyple::core::seproxy::message;

OpenSession10CmdBuild::OpenSession10CmdBuild(
  const uint8_t keyIndex,
  const std::vector<uint8_t>& samChallenge,
  const uint8_t sfi,
  const uint8_t recordNumber)
: AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>(PoRevision::REV1_0),
  mSfi(sfi),
  mRecordNumber(recordNumber)
{
    if (keyIndex == 0x00)
        throw IllegalArgumentException("Key index can't be null for rev 1.0!");

    const uint8_t p1 = (recordNumber * 8) + keyIndex;
    const uint8_t p2 = sfi * 8;

    /*
     * case 4: this command contains incoming and outgoing data. We define
     * le = 0, the actual length will be processed by the lower layers.
     */
    const uint8_t le = 0;

    mRequest = setApduRequest(
                     PoClass::LEGACY.getValue(),
                     CalypsoPoCommand::getOpenSessionForRev(PoRevision::REV1_0),
                     p1,
                     p2,
                     samChallenge,
                     le);

    const std::string extraInfo =
        "KEYINDEX=" + std::to_string(keyIndex) + ", " +
        "SFI=" + StringHelper::uint8ToHexString(sfi) + ", " +
        "REC=" + std::to_string(recordNumber);
    addSubName(extraInfo);
}

std::shared_ptr<AbstractOpenSessionRespPars>
    OpenSession10CmdBuild::createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<OpenSession10RespPars>(apduResponse, this);
}

bool OpenSession10CmdBuild::isSessionBufferUsed() const
{
    return false;
}

uint8_t OpenSession10CmdBuild::getSfi() const
{
    return mSfi;
}

uint8_t OpenSession10CmdBuild::getRecordNumber() const
{
    return mRecordNumber;
}

}
}
}
}
}
}

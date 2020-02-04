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
#include "stringhelper.h"

/* Calypso */
#include "CloseSessionCmdBuild.h"
#include "CloseSessionRespPars.h"

/* Core */
#include "ByteArrayUtil.h"
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {


using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

CloseSessionCmdBuild::CloseSessionCmdBuild(
  PoClass poClass, bool ratificationAsked,
  std::vector<uint8_t>& terminalSessionSignature)
: AbstractPoCommandBuilder<CloseSessionRespPars>(
      CalypsoPoCommands::CLOSE_SESSION, nullptr)
{
    /*
     * The optional parameter terminalSessionSignature could contain 4 or 8
     * bytes.
     */
    if (terminalSessionSignature.size() > 0  &&
        terminalSessionSignature.size() != 4 &&
        terminalSessionSignature.size() != 8) {
        throw std::invalid_argument(
                  StringHelper::formatSimple(
                      "Invalid terminal sessionSignature: %s",
                       ByteArrayUtil::toHex(terminalSessionSignature)));
    }

    uint8_t p1 = ratificationAsked ? 0x80 : 0x00;

    /*
     * case 4: this command contains incoming and outgoing data. We define
     * le = 0, the actual length will be processed by the lower layers.
     */
    uint8_t le = 0;

    request = setApduRequest(poClass.getValue(), command, p1, 0x00,
                             terminalSessionSignature, le);
}

CloseSessionCmdBuild::CloseSessionCmdBuild(PoClass poClass)
: AbstractPoCommandBuilder<CloseSessionRespPars>(
      CalypsoPoCommands::CLOSE_SESSION, nullptr)
{
    request = setApduRequest(poClass.getValue(), command, 0x00, 0x00, 0x00);

    /* Add "Abort session" to command name for logging purposes */
    this->addSubName("Abort session");
}

std::shared_ptr<CloseSessionRespPars>
    CloseSessionCmdBuild::createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<CloseSessionRespPars>(apduResponse);
}

}
}
}
}
}
}

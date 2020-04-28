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

#include "DecreaseCmdBuild.h"
#include "DecreaseRespPars.h"
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

DecreaseCmdBuild::DecreaseCmdBuild(PoClass poClass, char sfi,
                                   char counterNumber, int decValue,
                                   const std::string& extraInfo)
: AbstractPoCommandBuilder<DecreaseRespPars>(CalypsoPoCommands::DECREASE,
                                             nullptr)
{
    // only counter number >= 1 are allowed
    if (counterNumber < 1) {
        throw std::invalid_argument("Counter number out of range!");
    }

    // check if the incValue is in the allowed interval
    if (decValue < 0 || decValue > 0xFFFFFF) {
        throw std::invalid_argument("Decrement value out of range!");
    }

    // convert the integer value into a 3-byte buffer
    std::vector<uint8_t> decValueBuffer(3);
    decValueBuffer[0] = static_cast<char>((decValue >> 16) & 0xFF);
    decValueBuffer[1] = static_cast<char>((decValue >> 8) & 0xFF);
    decValueBuffer[2] = static_cast<char>(decValue & 0xFF);

    /* this is a case4 command, we set Le = 0 */
    this->request = setApduRequest(poClass.getValue(), command, counterNumber,
                                   sfi * 8, decValueBuffer, 0x00);
    if (extraInfo != "") {
        this->addSubName(extraInfo);
    }
}

std::shared_ptr<DecreaseRespPars> DecreaseCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<DecreaseRespPars>(apduResponse);
}

}
}
}
}
}

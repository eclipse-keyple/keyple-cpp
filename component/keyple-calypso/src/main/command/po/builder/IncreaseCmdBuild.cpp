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

#include "IncreaseCmdBuild.h"
#include "IncreaseRespPars.h"
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

IncreaseCmdBuild::IncreaseCmdBuild(PoClass poClass, char sfi,
                                   char counterNumber, int incValue,
                                   const std::string& extraInfo)
: AbstractPoCommandBuilder<IncreaseRespPars>(command, nullptr)
{

    // only counter number >= 1 are allowed
    if (counterNumber < 1) {
        throw std::invalid_argument("Counter number out of range!");
    }

    // check if the incValue is in the allowed interval
    if (incValue < 0 || incValue > 0xFFFFFF) {
        throw std::invalid_argument("Increment value out of range!");
    }

    // convert the integer value into a 3-byte buffer
    std::vector<uint8_t> incValueBuffer(3);
    incValueBuffer[0] = (incValue >> 16) & 0xFF;
    incValueBuffer[1] = (incValue >> 8) & 0xFF;
    incValueBuffer[2] = incValue & 0xFF;

    /* this is a case4 command, we set Le = 0 */
    this->request = setApduRequest(poClass.getValue(), command, counterNumber,
                                   sfi * 8, incValueBuffer, 0x00);
    if (extraInfo != "") {
        this->addSubName(extraInfo);
    }
}

std::shared_ptr<IncreaseRespPars> IncreaseCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<IncreaseRespPars>(apduResponse);
}
}
}
}
}
}

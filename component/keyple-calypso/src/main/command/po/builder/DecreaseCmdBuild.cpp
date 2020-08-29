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

DecreaseCmdBuild::DecreaseCmdBuild(
  PoClass poClass, uint8_t sfi, uint8_t counterNumber, int decValue)
: AbstractPoCommandBuilder<DecreaseRespPars>(
    CalypsoPoCommand::DECREASE, nullptr),
  mSfi(sfi), mCounterNumber(counterNumber), mDecValue(decValue)
{
    const uint8_t cla = poClass.getValue();

    /* Convert the integer value into a 3-byte buffer */
    std::vector<uint8_t> decValueBuffer(3);
    decValueBuffer[0] = static_cast<char>((decValue >> 16) & 0xFF);
    decValueBuffer[1] = static_cast<char>((decValue >> 8) & 0xFF);
    decValueBuffer[2] = static_cast<char>(decValue & 0xFF);

    const uint8_t p2 = sfi * 8;

    /* this is a case4 command, we set Le = 0 */
    request = setApduRequest(cla, command, counterNumber, p2, decValueBuffer,0);

    const std::string extraInfo =
        StringHelper::formatSimple("SFI=%02X, COUNTER=%d, DECREMENT=%d", sfi,
                                   counterNumber, decValue);
    addSubName(extraInfo);
}

std::shared_ptr<DecreaseRespPars> DecreaseCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<DecreaseRespPars>(apduResponse, this);
}

bool DecreaseCmdBuild::isSessionBufferUsed() const
{
    return true;
}

uint8_t DecreaseCmdBuild::getSfi() const
{
    return mSfi;
}

int DecreaseCmdBuild::getCounterNumber() const
{
    return mCounterNumber;
}

int DecreaseCmdBuild::getDecValue() const
{
    return mDecValue;
}

}
}
}
}
}

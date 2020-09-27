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

#include "IncreaseCmdBuild.h"

/* Calypso */
#include "IncreaseRespPars.h"

/* Core */
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

IncreaseCmdBuild::IncreaseCmdBuild(
  const PoClass poClass,
  const uint8_t sfi,
  const uint8_t counterNumber,
  const int incValue)
: AbstractPoCommandBuilder<IncreaseRespPars>(
      std::make_shared<CalypsoPoCommand>(CalypsoPoCommand::INCREASE), nullptr),
  mSfi(sfi),
  mCounterNumber(counterNumber),
  mIncValue(incValue)
{
    const uint8_t cla = poClass.getValue();

    /* Convert the integer value into a 3-byte buffer */
    std::vector<uint8_t> incValueBuffer(3);
    incValueBuffer[0] = (incValue >> 16) & 0xFF;
    incValueBuffer[1] = (incValue >> 8) & 0xFF;
    incValueBuffer[2] = incValue & 0xFF;

    const uint8_t p2 = sfi * 8;

    /* this is a case4 command, we set Le = 0 */
    mRequest = setApduRequest(cla,
                              command,
                              counterNumber,
                              p2,
                              incValueBuffer,
                              0x00);

    const std::string extraInfo =
        StringHelper::formatSimple("SFI=%02X, COUNTER=%d, INCREMENT=%d",
                                   sfi,
                                   counterNumber,
                                   incValue);
    addSubName(extraInfo);
}

std::shared_ptr<IncreaseRespPars> IncreaseCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<IncreaseRespPars>(apduResponse, this);
}

bool IncreaseCmdBuild::isSessionBufferUsed() const
{
    return true;
}

uint8_t IncreaseCmdBuild::getSfi() const
{
    return mSfi;
}

uint8_t IncreaseCmdBuild::getCounterNumber() const
{
    return mCounterNumber;
}

int IncreaseCmdBuild::getIncValue() const
{
    return mIncValue;
}


}
}
}
}
}

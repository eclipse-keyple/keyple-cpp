/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "VerifyPinCmdBuild.h"

/* Calypso */
#include "VerifyPinRespPars.h"

/* Common */
#include "IllegalArgumentException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace keyple::calypso::command::po::parser;
using namespace keyple::common::exception;

VerifyPinCmdBuild::VerifyPinCmdBuild(const PoClass& poClass,
                      		         const PinTransmissionMode& pinTransmissionMode,
                                     const std::vector<uint8_t>& pin)
: AbstractPoCommandBuilder<VerifyPinRespPars>(
      std::make_shared<CalypsoPoCommand>(CalypsoPoCommand::VERIFY_PIN),
      nullptr),
  mCla(poClass.getValue()), mReadCounterOnly(false)
{
    if ((pin.empty()) ||
        (pinTransmissionMode == PinTransmissionMode::PLAIN && pin.size() != 4) ||
        (pinTransmissionMode == PinTransmissionMode::ENCRYPTED && pin.size() != 8))
        throw IllegalArgumentException("The PIN must be 4 bytes long");


    const uint8_t p1 = 0x00;
    const uint8_t p2 = 0x00;

    mRequest = setApduRequest(mCla, mCommand, p1, p2, pin);

    addSubName(pinTransmissionMode == PinTransmissionMode::PLAIN ? "PLAIN" : "ENCRYPTED");
}

VerifyPinCmdBuild::VerifyPinCmdBuild(const PoClass& poClass)
: AbstractPoCommandBuilder<VerifyPinRespPars>(
      std::make_shared<CalypsoPoCommand>(CalypsoPoCommand::VERIFY_PIN),
      nullptr),
  mCla(poClass.getValue()), mReadCounterOnly(true)
{
    const uint8_t p1 = 0x00;
    const uint8_t p2 = 0x00;

    mRequest = setApduRequest(mCla, mCommand, p1, p2, {});

    addSubName("Read presentation counter");
}

std::shared_ptr<VerifyPinRespPars> VerifyPinCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<VerifyPinRespPars>(apduResponse, this);
}

bool VerifyPinCmdBuild::isSessionBufferUsed() const
{
    return false;
}

bool VerifyPinCmdBuild::isReadCounterOnly() const
{
    return mReadCounterOnly;
}

}
}
}
}
}
}

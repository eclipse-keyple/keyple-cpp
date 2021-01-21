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

#include "OpenSession24RespPars.h"
#include "ApduResponse.h"
#include "AbstractOpenSessionRespPars.h"

/* Common */
#include "Arrays.h"
#include "IllegalStateException.h"
#include "stringhelper.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po;
using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy::message;

using SecureSession = AbstractOpenSessionRespPars::SecureSession;

OpenSession24RespPars::OpenSession24RespPars(
  std::shared_ptr<ApduResponse> response, OpenSession24CmdBuild* builder)
: AbstractOpenSessionRespPars(response, builder, PoRevision::REV2_4)
{
    /**
     * C++ vs. Java: Do not remove. This code is required as the base class
     *               constructor cannot call a derived class member function.
     */
    const std::vector<uint8_t> dataOut = response->getDataOut();
    if (dataOut.size())
       mSecureSession = toSecureSession(dataOut);
}

std::shared_ptr<SecureSession> OpenSession24RespPars::toSecureSession(
    const std::vector<uint8_t>& apduResponseData)
{
    return createSecureSession(apduResponseData);
}

std::shared_ptr<SecureSession> OpenSession24RespPars::createSecureSession(
    const std::vector<uint8_t>& apduResponseData)
{
    bool previousSessionRatified;

    /**
     * In rev 2.4 mode, the response to the Open Secure Session command is as
     * follows:
     * <p>
     * <code>KK CC CC CC CC [RR RR] [NN..NN]</code>
     * <p>
     * Where:
     * <ul>
     * <li><code>KK</code> = KVC byte CC</li>
     * <li><code>CC CC CC CC</code> = PO challenge</li>
     * <li><code>RR RR</code> = ratification bytes (may be absent)</li>
     * <li><code>NN..NN</code> = record data (29 bytes)</li>
     * </ul>
     * Legal length values are:
     * <ul>
     * <li>5: ratified, 1-byte KCV, 4-byte challenge, no data</li>
     * <li>34: ratified, 1-byte KCV, 4-byte challenge, 29 bytes of data</li>
     * <li>7: not ratified (2 ratification bytes), 1-byte KCV, 4-byte
     * challenge, no data</li>
     * <li>35 not ratified (2 ratification bytes), 1-byte KCV, 4-byte
     * challenge, 29 bytes of
     * data</li>
     * </ul>
     */
    std::vector<uint8_t> data;

    switch (apduResponseData.size()) {
    case 5:
        previousSessionRatified = true;
        break;
    case 34:
        previousSessionRatified = true;
        data = Arrays::copyOfRange(apduResponseData, 5, 34);
        break;
    case 7:
        previousSessionRatified = false;
        break;
    case 36:
        previousSessionRatified = false;
        data = Arrays::copyOfRange(apduResponseData, 7, 36);
        break;
    default:
        throw IllegalStateException("Bad response length to Open Secure Session: " +
                                    std::to_string(apduResponseData.size()));
    }

    std::vector<uint8_t> challengeTransactionCounter = Arrays::copyOfRange(apduResponseData, 1, 4);
    std::vector<uint8_t> challengeRandomNumber = Arrays::copyOfRange(apduResponseData, 4, 5);

    return std::make_shared<SecureSession>(challengeTransactionCounter,
                                           challengeRandomNumber,
                                           previousSessionRatified,
                                           false,
                                           apduResponseData[0],
                                           data,
                                           apduResponseData);
}

}
}
}
}
}
}

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

#include "OpenSession10RespPars.h"

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

OpenSession10RespPars::OpenSession10RespPars(
  std::shared_ptr<ApduResponse> response, OpenSession10CmdBuild* builder)
: AbstractOpenSessionRespPars(response, builder, PoRevision::REV1_0)
{
    /**
     * C++ vs. Java: Do not remove. This code is required as the base class
     *               constructor cannot call a derived class member function.
     */
    const std::vector<uint8_t> dataOut = response->getDataOut();
    if (dataOut.size())
        mSecureSession = toSecureSession(dataOut);
}

std::shared_ptr<SecureSession> OpenSession10RespPars::toSecureSession(
    const std::vector<uint8_t>& apduResponseData)
{
    return createSecureSession(apduResponseData);
}

std::shared_ptr<SecureSession> OpenSession10RespPars::createSecureSession(
    const std::vector<uint8_t>& apduResponseData)
{
    bool previousSessionRatified;

    /**
     * In rev 1.0 mode, the response to the Open Secure Session command is as
     * follows:
     * <p>
     * <code>CC CC CC CC [RR RR] [NN..NN]</code>
     * <p>
     * Where:
     * <ul>
     * <li><code>CC CC CC CC</code> = PO challenge</li>
     * <li><code>RR RR</code> = ratification bytes (may be absent)</li>
     * <li><code>NN..NN</code> = record data (29 bytes)</li>
     * </ul>
     * Legal length values are:
     * <ul>
     * <li>4: ratified, 4-byte challenge, no data</li>
     * <li>33: ratified, 4-byte challenge, 29 bytes of data</li>
     * <li>6: not ratified (2 ratification bytes), 4-byte challenge, no data
     * </li>
     * <li>35 not ratified (2 ratification bytes), 4-byte challenge, 29 bytes of
     * data</li>
     * </ul>
     */
    std::vector<uint8_t> data;

    switch (apduResponseData.size()) {
    case 4:
        previousSessionRatified = true;
        break;
    case 33:
        previousSessionRatified = true;
        data = Arrays::copyOfRange(apduResponseData, 4, 33);
        break;
    case 6:
        previousSessionRatified = false;
        break;
    case 35:
        previousSessionRatified = false;
        data = Arrays::copyOfRange(apduResponseData, 6, 35);
        break;
    default:
        throw IllegalStateException("Bad response length to Open Secure Session: " +
                                    std::to_string(apduResponseData.size()));
    }

    /* KVC doesn't exist and is set to null for this type of PO */
    std::vector<uint8_t> challengeTransactionCounter =
        Arrays::copyOfRange(apduResponseData, 0, 3);
    std::vector<uint8_t> challengeRandomNumber =
        Arrays::copyOfRange(apduResponseData, 3, 4);

    return std::make_shared<SecureSession>(challengeTransactionCounter,
                                           challengeRandomNumber,
                                           previousSessionRatified,
                                           false,
                                           data,
                                           apduResponseData);
}

}
}
}
}
}
}

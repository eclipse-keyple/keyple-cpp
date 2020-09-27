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

#include "OpenSession31RespPars.h"
#include "ApduResponse.h"
#include "AbstractOpenSessionRespPars.h"

#include "Arrays.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po;
using namespace keyple::core::seproxy::message;

OpenSession31RespPars::OpenSession31RespPars(
  std::shared_ptr<ApduResponse> response, OpenSession31CmdBuild* builder)
: AbstractOpenSessionRespPars(response, builder, PoRevision::REV3_1) {}

std::shared_ptr<AbstractOpenSessionRespPars::SecureSession>
    OpenSession31RespPars::toSecureSession(
        const std::vector<uint8_t>& apduResponseData)
{
    bool previousSessionRatified       = (apduResponseData[4] == 0x00);
    bool manageSecureSessionAuthorized = false;

    uint8_t kif    = apduResponseData[5];
    int dataLength = apduResponseData[7];
    std::vector<uint8_t> data =
        Arrays::copyOfRange(apduResponseData, 8, 8 + dataLength);

    std::vector<uint8_t> challengeTransactionCounter =
        Arrays::copyOfRange(apduResponseData, 0, 3);
    std::vector<uint8_t> challengeRandomNumber =
        Arrays::copyOfRange(apduResponseData, 3, 4);

    return std::make_shared<SecureSession>(
               challengeTransactionCounter,
               challengeRandomNumber,
               previousSessionRatified,
               manageSecureSessionAuthorized,
               kif,
               apduResponseData[6],
               data,
               apduResponseData);
}

}
}
}
}
}
}

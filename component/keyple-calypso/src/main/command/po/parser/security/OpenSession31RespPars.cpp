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
  std::shared_ptr<ApduResponse> response)
: AbstractOpenSessionRespPars(response, PoRevision::REV3_1)
{
    std::vector<char> data = response->getDataOut();
    this->secureSession = toSecureSession(data);
}

std::shared_ptr<AbstractOpenSessionRespPars::SecureSession>
OpenSession31RespPars::toSecureSession(std::vector<char> &apduResponseData)
{
    std::shared_ptr<SecureSession> secureSession;
    bool previousSessionRatified = (apduResponseData[4] == static_cast<char>(0x00));
    bool manageSecureSessionAuthorized = false;

    char kif = apduResponseData[5];
    int dataLength = apduResponseData[7];
    std::vector<char> data =  Arrays::copyOfRange(apduResponseData, 8, 8 + dataLength);

    std::vector<char> challengeTransactionCounter = Arrays::copyOfRange(apduResponseData, 0, 3);
    std::vector<char> challengeRandomNumber = Arrays::copyOfRange(apduResponseData, 3, 4);

    return std::make_shared<SecureSession>( challengeTransactionCounter,
               challengeRandomNumber,
               previousSessionRatified, manageSecureSessionAuthorized,
               kif, std::make_shared<Byte>(apduResponseData[6]), data,
               apduResponseData);
}

}
}
}
}
}
}

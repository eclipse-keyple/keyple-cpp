/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#include "OpenSession10RespPars.h"
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

OpenSession10RespPars::OpenSession10RespPars(std::shared_ptr<ApduResponse> response)
: AbstractOpenSessionRespPars(response, PoRevision::REV1_0)
{
    std::vector<char> data = response->getDataOut();
    this->secureSession = toSecureSession(data);
}

std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> OpenSession10RespPars::toSecureSession(std::vector<char> &apduResponseData)
{
    return createSecureSession(apduResponseData);
}

std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> OpenSession10RespPars::createSecureSession(std::vector<char> &apduResponseData)
{
    bool previousSessionRatified = true;

    /**
     * In rev 1.0 mode, the response to the Open Secure Session command is as follows:
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
     * <li>6: not ratified (2 ratification bytes), 4-byte challenge, no data</li>
     * <li>35 not ratified (2 ratification bytes), 4-byte challenge, 29 bytes of data</li>
     * </ul>
     */
    switch (apduResponseData.size()) {
        case 4:
        case 33:
            previousSessionRatified = true;
            break;
        case 6:
        case 35:
            previousSessionRatified = false;
            break;
        default:
            throw IllegalStateException("Bad response length to Open Secure Session: " +
                                        std::to_string(apduResponseData.size()));
    }

    /* KVC doesn't exist and is set to null for this type of PO */
    std::vector<char> emptyVector;
    std::vector<char> challengeTransactionCounter = Arrays::copyOfRange(apduResponseData, 1, 4);
    std::vector<char> challengeRandomNumber = Arrays::copyOfRange(apduResponseData, 4, 5);

    return std::make_shared<SecureSession>(challengeTransactionCounter,
                                           challengeRandomNumber,
                                           previousSessionRatified, false, nullptr, emptyVector,
                                           apduResponseData);
}

}
}
}
}
}
}

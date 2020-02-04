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

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "CloseSessionRespPars.h"

/* Common */
#include "Arrays.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

std::unordered_map<int, std::shared_ptr<StatusProperties>>
    CloseSessionRespPars::STATUS_TABLE;

CloseSessionRespPars::StaticConstructor::StaticConstructor()
 {
    std::unordered_map<int, std::shared_ptr<StatusProperties>>
        m(AbstractApduResponseParser::STATUS_TABLE);

    m.emplace(0x6700,
              std::make_shared<StatusProperties>(
                  false,
                  "Lc signatureLo not supported (e.g. Lc=4 with a Revision " \
                  "3.2 mode for Open Secure Session)."));
    m.emplace(0x6B00,
              std::make_shared<StatusProperties>(
                  false, "P1 or P2 signatureLo not supported."));
    m.emplace(0x6988,
             std::make_shared<StatusProperties>(false,
                                                "incorrect signatureLo."));
    m.emplace(0x6985,
              std::make_shared<StatusProperties>(false,
                                                 "No security was opened."));

    STATUS_TABLE = m;
}

CloseSessionRespPars::StaticConstructor CloseSessionRespPars::staticConstructor;

std::unordered_map<int, std::shared_ptr<
    AbstractApduResponseParser::StatusProperties>>
        CloseSessionRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

CloseSessionRespPars::CloseSessionRespPars(
  std::shared_ptr<ApduResponse> response)
: AbstractApduResponseParser(response)
{
    std::vector<uint8_t> data = response->getDataOut();
    parse(data);
}

void CloseSessionRespPars::parse(const std::vector<uint8_t>& response)
{
    if (response.size() == 8) {
        signatureLo = Arrays::copyOfRange(response, 4, 8);
        postponedData = Arrays::copyOfRange(response, 0, 4);
    }
    else if (response.size() == 4) {
        signatureLo = Arrays::copyOfRange(response, 0, 4);
    }
    else {
        if (response.size() != 0) {
            throw std::invalid_argument("Unexpected length in response to " \
                                        "CloseSecureSession command: " +
                                        std::to_string(response.size()));
        }
    }
}

const std::vector<uint8_t>& CloseSessionRespPars::getSignatureLo() const
{
    return signatureLo;
}

const std::vector<uint8_t>& CloseSessionRespPars::getPostponedData() const
{
    return postponedData;
}

}
}
}
}
}
}

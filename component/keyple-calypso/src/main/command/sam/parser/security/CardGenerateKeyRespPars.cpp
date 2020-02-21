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

/* Calypso */
#include "CardGenerateKeyRespPars.h"

/* Core */
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::command;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

std::unordered_map<int, std::shared_ptr<StatusProperties>>
    CardGenerateKeyRespPars::STATUS_TABLE;

CardGenerateKeyRespPars::StaticConstructor::StaticConstructor()
{
    std::unordered_map<int, std::shared_ptr<StatusProperties>>
        m(AbstractSamResponseParser::STATUS_TABLE);

    m.emplace(0x6700,
              std::make_shared<StatusProperties>(
                  false, "Lc value not supported"));
    m.emplace(0x6985,
              std::make_shared<StatusProperties>(
                  false, "Preconditions not satisfied"));
    m.emplace(0x6A00,
              std::make_shared<StatusProperties>(
                  false, "Incorrect P1 or P2"));
    m.emplace(0x6A80,
              std::make_shared<StatusProperties>(
                  false,
                  "Incorrect incoming data: unknown or incorrect format"));
    m.emplace(0x6A83,
              std::make_shared<StatusProperties>(
                  false,
                  "Record not found: ciphering key or key to cipher not found"));
    m.emplace(0x9000,
              std::make_shared<StatusProperties>(
                  true, "Successful execution."));

    STATUS_TABLE = m;
}

CardGenerateKeyRespPars::StaticConstructor
    CardGenerateKeyRespPars::staticConstructor;

std::unordered_map<int, std::shared_ptr<StatusProperties>>
    CardGenerateKeyRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

CardGenerateKeyRespPars::CardGenerateKeyRespPars(
  std::shared_ptr<ApduResponse> response)
: AbstractSamResponseParser(response) {
}

std::vector<uint8_t> CardGenerateKeyRespPars::getCipheredData() const
{
    return isSuccessful() ? response->getDataOut() : std::vector<uint8_t>();
}

}
}
}
}
}
}

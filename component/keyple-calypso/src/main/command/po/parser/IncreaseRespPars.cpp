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

#include "AbstractApduResponseParser.h"
#include "ApduResponse.h"
#include "IncreaseRespPars.h"

/* Common */
#include "stringhelper.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

std::unordered_map<
    int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
    IncreaseRespPars::STATUS_TABLE;

IncreaseRespPars::StaticConstructor::StaticConstructor()
{
    std::unordered_map<
        int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
        m(AbstractApduResponseParser::STATUS_TABLE);
    m.emplace(0x6400,
              std::make_shared<AbstractApduResponseParser::StatusProperties>(
                  false, "Too many modifications in session."));
    m.emplace(0x6700,
              std::make_shared<AbstractApduResponseParser::StatusProperties>(
                  false, "Lc value not supported."));
    m.emplace(0x6981,
              std::make_shared<AbstractApduResponseParser::StatusProperties>(
                  false,
                  "The current EF is not a Counters or Simulated Counter EF."));
    m.emplace(0x6982,
              std::make_shared<AbstractApduResponseParser::StatusProperties>(
                  false, "Security conditions not fulfilled (no session, wrong "
                         "key, encryption required)."));
    m.emplace(
        0x6985,
        std::make_shared<AbstractApduResponseParser::StatusProperties>(
            false,
            "Access forbidden (Never access mode, DF is invalidated, etc.)"));
    m.emplace(0x6986,
              std::make_shared<AbstractApduResponseParser::StatusProperties>(
                  false, "Command not allowed (no current EF)."));
    m.emplace(0x6A80,
              std::make_shared<AbstractApduResponseParser::StatusProperties>(
                  false, "Overflow error."));
    m.emplace(0x6A82,
              std::make_shared<AbstractApduResponseParser::StatusProperties>(
                  false, "File not found."));
    m.emplace(0x6B00,
              std::make_shared<AbstractApduResponseParser::StatusProperties>(
                  false, "P1 or P2 value not supported."));
    m.emplace(0x6103,
              std::make_shared<AbstractApduResponseParser::StatusProperties>(
                  true, "Successful execution."));
    m.emplace(0x9000,
              std::make_shared<AbstractApduResponseParser::StatusProperties>(
                  true, "Successful execution."));
    STATUS_TABLE = m;
}

IncreaseRespPars::StaticConstructor IncreaseRespPars::staticConstructor;

IncreaseRespPars::IncreaseRespPars(std::shared_ptr<ApduResponse> response)
: AbstractPoResponseParser(response)
{
}

std::unordered_map<
    int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
IncreaseRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

int IncreaseRespPars::getNewValue()
{
    std::vector<uint8_t> newValueBuffer = getApduResponse()->getDataOut();
    if (newValueBuffer.size() == 3) {
        return (newValueBuffer[0] << 16) + (newValueBuffer[1] << 8) +
               newValueBuffer[2];
    } else {
        throw IllegalStateException("No counter value available in response "
                                    "to the Increase command.");
    }
}

std::string IncreaseRespPars::toString()
{
    return StringHelper::formatSimple("New counter value: %d", getNewValue());
}

}
}
}
}
}

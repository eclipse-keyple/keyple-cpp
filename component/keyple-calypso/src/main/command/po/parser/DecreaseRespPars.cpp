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

#include "DecreaseRespPars.h"

#include "stringhelper.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

std::unordered_map<int, std::shared_ptr<StatusProperties>> DecreaseRespPars::STATUS_TABLE;

DecreaseRespPars::StaticConstructor::StaticConstructor()
{
    std::unordered_map<int, std::shared_ptr<StatusProperties>> m(AbstractApduResponseParser::STATUS_TABLE);
    m.emplace(0x6400, std::make_shared<StatusProperties>(false, "Too many modifications in session."));
    m.emplace(0x6700, std::make_shared<StatusProperties>(false, "Lc value not supported."));
    m.emplace(0x6981, std::make_shared<StatusProperties>(false, "The current EF is not a Counters or Simulated Counter EF."));
    m.emplace(0x6982, std::make_shared<StatusProperties>(false, "Security conditions not fulfilled (no session, wrong key, encryption required)."));
    m.emplace(0x6985, std::make_shared<StatusProperties>(false, "Access forbidden (Never access mode, DF is invalidated, etc.)"));
    m.emplace(0x6986, std::make_shared<StatusProperties>(false, "Command not allowed (no current EF)."));
    m.emplace(0x6A80, std::make_shared<StatusProperties>(false, "Overflow error."));
    m.emplace(0x6A82, std::make_shared<StatusProperties>(false, "File not found."));
    m.emplace(0x6B00, std::make_shared<StatusProperties>(false, "P1 or P2 value not supported."));
    m.emplace(0x6103, std::make_shared<StatusProperties>(true, "Successful execution."));
    m.emplace(0x9000, std::make_shared<StatusProperties>(true, "Successful execution."));
    STATUS_TABLE = m;
}

DecreaseRespPars::StaticConstructor DecreaseRespPars::staticConstructor;

std::unordered_map<int,
    std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
        DecreaseRespPars::getStatusTable()
{
    return STATUS_TABLE;
}

DecreaseRespPars::DecreaseRespPars(std::shared_ptr<ApduResponse> response)
: AbstractPoResponseParser(response)
{
}

int DecreaseRespPars::getNewValue()
{
    std::vector<char> newValueBuffer = getApduResponse()->getDataOut();
    if (newValueBuffer.size() == 3) {
        return (newValueBuffer[0] << 16) + (newValueBuffer[1] << 8) + newValueBuffer[2];
    }
    else {
        throw IllegalStateException("No counter value available in response " \
                                    "to the Decrease command.");
    }
}

std::string DecreaseRespPars::toString()
{
    return StringHelper::formatSimple("New counter value: %d", getNewValue());
}

}
}
}
}
}

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

#include "ChangeKeyRespPars.h"
#include "AbstractApduResponseParser_Import.h"
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> ChangeKeyRespPars::STATUS_TABLE;

ChangeKeyRespPars::StaticConstructor::StaticConstructor()
{
    std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> m(AbstractApduResponseParser::STATUS_TABLE);
    m.emplace(0x6700, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, std::string("Lc value not supported (not 04h, 10h, 18h, 20h or 18h not ") + "supported)."));
    m.emplace(0x6900, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Transaction Counter is 0."));
    m.emplace(0x6982, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Security conditions not fulfilled (Get Challenge not done: challenge unavailable)."));
    m.emplace(0x6985, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Access forbidden (a session is open or DF is invalidated)."));
    m.emplace(0x6988, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Incorrect Cryptogram."));
    m.emplace(0x6A80, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Decrypted message incorrect (key algorithm not supported, incorrect padding, etc.)."));
    m.emplace(0x6A87, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Lc not compatible with P2."));
    m.emplace(0x6B00, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Incorrect P1, P2."));
    m.emplace(0x9000, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Successful execution."));
    STATUS_TABLE = m;
}

ChangeKeyRespPars::StaticConstructor ChangeKeyRespPars::staticConstructor;

ChangeKeyRespPars::ChangeKeyRespPars(std::shared_ptr<ApduResponse> response)
: AbstractPoResponseParser(response)
{

}

std::unordered_map<int,
    std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
        ChangeKeyRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

}
}
}
}
}
}

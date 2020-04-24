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

#include "AppendRecordRespPars.h"
#include "AbstractApduResponseParser.h"
#include "ApduResponse.h"

#include "Integer.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

std::unordered_map<int, std::shared_ptr<StatusProperties>>
    AppendRecordRespPars::STATUS_TABLE;

AppendRecordRespPars::StaticConstructor::StaticConstructor()
{
    std::unordered_map<int, std::shared_ptr<StatusProperties>> m(
        AbstractApduResponseParser::STATUS_TABLE);

    m.emplace(0x6B00, std::make_shared<StatusProperties>(
                          false, "P1 or P2 vaue not supported."));
    m.emplace(0x6700, std::make_shared<StatusProperties>(
                          false, "Lc value not supported."));
    m.emplace(0x6400, std::make_shared<StatusProperties>(
                          false, "Too many modifications in "
                                 "session."));
    m.emplace(0x6981, std::make_shared<StatusProperties>(
                          false, "The current EF is not a Cyclic "
                                 "EF."));
    m.emplace(0x6982, std::make_shared<StatusProperties>(
                          false, "Security conditions not fulfilled "
                                 "(no session, wrong key)."));
    m.emplace(0x6985, std::make_shared<StatusProperties>(
                          false, "Access forbidden (Never access "
                                 "mode, DF is invalidated, etc..)."));
    m.emplace(0x6986, std::make_shared<StatusProperties>(
                          false, "Command not allowed (no current "
                                 "EF)."));
    m.emplace(0x6A82,
              std::make_shared<StatusProperties>(false, "File not found."));

    STATUS_TABLE = m;
}

AppendRecordRespPars::StaticConstructor AppendRecordRespPars::staticConstructor;

AppendRecordRespPars::AppendRecordRespPars(
    std::shared_ptr<ApduResponse> response)
: AbstractPoResponseParser(response)
{
}

std::unordered_map<
    int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
AppendRecordRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

}
}
}
}
}

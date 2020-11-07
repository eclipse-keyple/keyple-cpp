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

#include "SvPrepareDebitCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"
#include "System.h"

/* Calypso */
#include "SvPrepareOperationRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::common;
using namespace keyple::common::exception;

SvPrepareDebitCmdBuild::SvPrepareDebitCmdBuild(const SamRevision& samRevision,
                                               const std::vector<uint8_t>& svGetHeader,
                                               const std::vector<uint8_t>& svGetData,
                                               const std::vector<uint8_t>& svDebitCmdBuildData)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::SV_PREPARE_DEBIT),
    nullptr)
{
    const uint8_t cla = samRevision.getClassByte();
    const uint8_t p1 = 0x01;
    const uint8_t p2 = 0xFF;
    std::vector<uint8_t> data(16 + svGetData.size()); /* header(4) + SvDebit data (12) = 16 bytes */

    System::arraycopy(svGetHeader, 0, data, 0, 4);
    System::arraycopy(svGetData, 0, data, 4, svGetData.size());
    System::arraycopy(svDebitCmdBuildData, 0, data, 4+svGetData.size(), svDebitCmdBuildData.size());

    mRequest = setApduRequest(cla, mCommand, p1, p2, data);
}

std::shared_ptr<SvPrepareOperationRespPars> SvPrepareDebitCmdBuild::createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SvPrepareOperationRespPars>(
               apduResponse,
               reinterpret_cast<AbstractSamCommandBuilder<AbstractSamResponseParser>*>(this));
}

}
}
}
}
}
}

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

#include "SvCheckCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"
#include "System.h"

/* Calypso */
#include "SvCheckRespPars.h"

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

SvCheckCmdBuild::SvCheckCmdBuild(const SamRevision& revision,
                                 const std::vector<uint8_t>& svPoSignature)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::SV_CHECK),
    nullptr)
{
    if (!svPoSignature.empty() && svPoSignature.size() != 3 && svPoSignature.size() != 6)
        throw IllegalArgumentException("Invalid svPoSignature.");

    if (revision != SamRevision::NO_REV)
        mDefaultRevision = revision;

    const uint8_t cla = mDefaultRevision.getClassByte();
    const uint8_t p1 = 0x00;
    const uint8_t p2 = 0x00;

    if (!svPoSignature.empty()) {
        /* The operation is not "abort" */
        std::vector<uint8_t> data(svPoSignature.size());
        System::arraycopy(svPoSignature, 0, data, 0, svPoSignature.size());
        mRequest = setApduRequest(cla, mCommand, p1, p2, data);
    } else {
        mRequest = setApduRequest(cla, mCommand, p1, p2, 0x00);
    }
}

std::shared_ptr<SvCheckRespPars> SvCheckCmdBuild::createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SvCheckRespPars>(apduResponse, this);
}

}
}
}
}
}
}

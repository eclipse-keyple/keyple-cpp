/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include "DigestInitCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"
#include "System.h"

/* Calypso */
#include "DigestInitRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

DigestInitCmdBuild::DigestInitCmdBuild(
  const SamRevision& revision,
  const bool verificationMode,
  const bool confidentialSessionMode,
  const uint8_t workKeyRecordNumber,
  const uint8_t workKeyKif,
  const uint8_t workKeyKVC,
  const std::vector<uint8_t>& digestData)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::DIGEST_INIT),
    nullptr)
{
    if (revision != SamRevision::NO_REV)
        mDefaultRevision = revision;

    if (workKeyRecordNumber == 0x00 &&
        (workKeyKif == 0x00 || workKeyKVC == 0x00))
        throw IllegalArgumentException("Bad key record number, kif or kvc!");

    if (digestData.empty())
        throw IllegalArgumentException("Digest data is null!");

    const uint8_t cla = SamRevision::S1D == (mDefaultRevision) ? 0x94 : 0x80;
    uint8_t p1  = 0x00;

    if (verificationMode)
        p1 += 1;

    if (confidentialSessionMode)
        p1 += 2;

    uint8_t p2 = 0xFF;

    if (workKeyKif == 0xFF)
        p2 = workKeyRecordNumber;

    std::vector<uint8_t> dataIn;

    if (p2 == 0xFF) {
        dataIn    = std::vector<uint8_t>(2 + digestData.size());
        dataIn[0] = workKeyKif;
        dataIn[1] = workKeyKVC;
        System::arraycopy(digestData, 0, dataIn, 2, digestData.size());
    } else {
        dataIn = digestData;
    }

    mRequest = setApduRequest(cla, mCommand, p1, p2, dataIn);
}

std::shared_ptr<DigestInitRespPars> DigestInitCmdBuild::createResponseParser(
    const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<DigestInitRespPars>(apduResponse, this);
}

}
}
}
}
}
}

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

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "CalypsoPoAccessForbiddenException.h"
#include "CalypsoPoIllegalParameterException.h"
#include "CalypsoPoSecurityDataException.h"
#include "CloseSessionRespPars.h"

/* Common */
#include "Arrays.h"
#include "IllegalArgumentException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::common;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;
using namespace keyple::calypso::command::po::exception;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

const std::map<int, std::shared_ptr<StatusProperties>>
    CloseSessionRespPars::STATUS_TABLE = {
    {
        0x6700,
        std::make_shared<StatusProperties>(
            "Lc signatureLo not supported (e.g. Lc=4 with a Revision " \
            "3.2 mode for Open Secure Session).",
            typeid(CalypsoPoIllegalParameterException))
    }, {
        0x6B00,
        std::make_shared<StatusProperties>(
            "P1 or P2 signatureLo not supported.",
            typeid(CalypsoPoIllegalParameterException))
    }, {
        0x6988,
        std::make_shared<StatusProperties>(
            "incorrect signatureLo.",
             typeid(CalypsoPoSecurityDataException))
    }, {
        0x6985,
        std::make_shared<StatusProperties>(
            "No security was opened.",
            typeid(CalypsoPoAccessForbiddenException))
    }, {
        0x9000,
        std::make_shared<StatusProperties>("Success")
    }
};

const std::map<int, std::shared_ptr<StatusProperties>>&
    CloseSessionRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

CloseSessionRespPars::CloseSessionRespPars(
  std::shared_ptr<ApduResponse> response, CloseSessionCmdBuild* builder)
: AbstractPoResponseParser(
    response,
    dynamic_cast<AbstractPoCommandBuilder<AbstractPoResponseParser>*>(builder))
{
    parse(response->getDataOut());
}

void CloseSessionRespPars::parse(const std::vector<uint8_t>& response)
{
    if (response.size() == 8) {
        mSignatureLo   = Arrays::copyOfRange(response, 4, 8);
        mPostponedData = Arrays::copyOfRange(response, 0, 4);
    } else if (response.size() == 4) {
        mSignatureLo = Arrays::copyOfRange(response, 0, 4);
    } else {
        if (response.size() != 0) {
            throw IllegalArgumentException(
                      "Unexpected length in response to CloseSecureSession " \
                      "command: " +
                      std::to_string(response.size()));
        }
    }
}

const std::vector<uint8_t>& CloseSessionRespPars::getSignatureLo() const
{
    return mSignatureLo;
}

const std::vector<uint8_t>& CloseSessionRespPars::getPostponedData() const
{
    return mPostponedData;
}

}
}
}
}
}
}

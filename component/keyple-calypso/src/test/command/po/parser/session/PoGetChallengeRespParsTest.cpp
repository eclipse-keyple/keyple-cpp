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

#include "PoGetChallengeRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "ByteArrayUtil.h"
#include "PoGetChallengeRespPars.h"
#include "AbstractApduResponseParser_Import.h"

using namespace keyple::calypso::command::po::parser::security;

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {
using AbstractApduResponseParser =
    keyple::core::command::AbstractApduResponseParser;
using ApduResponse    = keyple::core::seproxy::message::ApduResponse;
using SeResponse      = keyple::core::seproxy::message::SeResponse;
using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
using ByteArrayUtils  = keyple::core::util::ByteArrayUtil;

void PoGetChallengeRespParsTest::POGetChallengetRespPars()
{
    std::vector<uint8_t> response = {
        0x03, 0x0D, 0x0E, 0xFA, 0x9C, 0x8C, 0xB7, 0x27, 0x90, 0x00};

    std::vector<std::shared_ptr<ApduResponse>> responses;
    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(response, nullptr);
    responses.push_back(apduResponse);
    std::vector<uint8_t> cResp1 = ByteArrayUtils::fromHex("9000");

    std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, std::make_shared<ApduResponse>(cResp1, nullptr), true),
            responses);

    std::shared_ptr<AbstractApduResponseParser> apduResponseParser =
        std::make_shared<PoGetChallengeRespPars>(
            seResponse->getApduResponses()[0]);

    ASSERT_EQ(ByteArrayUtils::toHex(response),
              ByteArrayUtils::toHex(
                  apduResponseParser->getApduResponse()->getBytes()));
    ASSERT_EQ("Success", apduResponseParser->getStatusInformation());
}
}
}
}
}
}
}

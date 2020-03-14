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

#include "DigestInitRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "ByteArrayUtil.h"
#include "DigestInitRespPars.h"
#include "AbstractApduResponseParser_Import.h"

using namespace keyple::calypso::command::sam::parser::security;

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using DigestInitRespPars =
    keyple::calypso::command::sam::parser::security::DigestInitRespPars;
using AbstractApduResponseParser =
    keyple::core::command::AbstractApduResponseParser;
using ApduResponse    = keyple::core::seproxy::message::ApduResponse;
using SeResponse      = keyple::core::seproxy::message::SeResponse;
using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
using ByteArrayUtils  = keyple::core::util::ByteArrayUtil;

void DigestInitRespParsTest::digestInitRespPars()
{
    std::vector<std::shared_ptr<ApduResponse>> responses;
    std::vector<uint8_t> cResp1 = {0x90, 0x00};
    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(cResp1, nullptr);
    responses.push_back(apduResponse);

    std::vector<uint8_t> cResp2 = ByteArrayUtils::fromHex("9000");
    std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, std::make_shared<ApduResponse>(cResp2, nullptr), true),
            responses);

    std::shared_ptr<AbstractApduResponseParser> apduResponseParser =
        std::make_shared<DigestInitRespPars>(
            seResponse->getApduResponses()[0]);

    std::vector<uint8_t> cResp = {0x90, 0x00};
    ASSERT_EQ(ByteArrayUtils::toHex(cResp),
              ByteArrayUtils::toHex(
                  apduResponseParser->getApduResponse()->getBytes()));
}
}
}
}
}
}
}

TEST(DigestInitRespParsTest, testA)
{
    std::shared_ptr<DigestInitRespParsTest> LocalTest =
        std::make_shared<DigestInitRespParsTest>();
    LocalTest->digestInitRespPars();
}

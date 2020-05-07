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

#include "DigestCloseRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "ByteArrayUtil.h"
#include "DigestCloseRespPars.h"
#include "AbstractApduResponseParser.h"

using namespace keyple::calypso::command::sam::parser::security;

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using DigestCloseRespPars =
    keyple::calypso::command::sam::parser::security::DigestCloseRespPars;
using AbstractApduResponseParser =
    keyple::core::command::AbstractApduResponseParser;
using ApduResponse    = keyple::core::seproxy::message::ApduResponse;
using SeResponse      = keyple::core::seproxy::message::SeResponse;
using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
using ByteArrayUtils  = keyple::core::util::ByteArrayUtil;

void DigestCloseRespParsTest::digestCloseRespPars()
{
    std::vector<std::shared_ptr<ApduResponse>> responses;
    std::vector<uint8_t> cResp0 = {0xA8, 0x31, 0xC3, 0x3E, 0x90, 0x00};

    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(cResp0, nullptr);
    responses.push_back(apduResponse);

    std::vector<uint8_t> cResp1 = ByteArrayUtils::fromHex("9000");

    std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, std::make_shared<ApduResponse>(cResp1, nullptr), true),
            responses);

    std::shared_ptr<AbstractApduResponseParser> apduResponseParser =
        std::make_shared<DigestCloseRespPars>(
            seResponse->getApduResponses()[0]);

    std::vector<uint8_t> responseActual =
        apduResponseParser->getApduResponse()->getBytes();
    std::vector<uint8_t> cResp = {0xA8, 0x31,0xC3, 0x3E, 0x90, 0x00};

    ASSERT_EQ(ByteArrayUtils::toHex(cResp),
              ByteArrayUtils::toHex(responseActual));
}
}
}
}
}
}
}

TEST(DigestCloseRespParsTest, testA)
{
    std::shared_ptr<DigestCloseRespParsTest> LocalTest =
        std::make_shared<DigestCloseRespParsTest>();
    LocalTest->digestCloseRespPars();
}

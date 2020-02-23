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

#include "DigestAuthenticateRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "ByteArrayUtil.h"
#include "DigestAuthenticateRespPars.h"
#include "AbstractApduResponseParser_Import.h"

using namespace keyple::calypso::command::sam::parser::security;

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using DigestAuthenticateRespPars =
    keyple::calypso::command::sam::parser::security::DigestAuthenticateRespPars;
using AbstractApduResponseParser =
    keyple::core::command::AbstractApduResponseParser;
using ApduResponse    = keyple::core::seproxy::message::ApduResponse;
using SeResponse      = keyple::core::seproxy::message::SeResponse;
using SeResponseSet   = keyple::core::seproxy::message::SeResponseSet;
using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
using ByteArrayUtils  = keyple::core::util::ByteArrayUtil;

void DigestAuthenticateRespParsTest::digestAuthenticateResp()
{

    std::vector<std::shared_ptr<ApduResponse>> responses;
    std::vector<char> cResp = std::vector<char>{90, 0};
    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(cResp, nullptr);
    responses.push_back(apduResponse);
    std::vector<char> cResp1 = ByteArrayUtils::fromHex("9000");
    std::shared_ptr<SeResponseSet> seResponse =
        std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, std::make_shared<ApduResponse>(cResp1, nullptr), true),
            responses));

    std::shared_ptr<AbstractApduResponseParser> apduResponseParser =
        std::make_shared<DigestAuthenticateRespPars>(
            seResponse->getSingleResponse()->getApduResponses()[0]);
    std::vector<char> responseActual =
        apduResponseParser->getApduResponse()->getBytes();
    std::vector<char> ucResp = std::vector<char>{90, 0};
    ASSERT_EQ(ByteArrayUtils::toHex(ucResp),
              ByteArrayUtils::toHex(responseActual));
}
}
}
}
}
}
}

TEST(DigestAuthenticateRespParsTest, testA)
{
    std::shared_ptr<DigestAuthenticateRespParsTest> LocalTest =
        std::make_shared<DigestAuthenticateRespParsTest>();
    LocalTest->digestAuthenticateResp();
}

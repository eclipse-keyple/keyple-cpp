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

#include "DigestUpdateRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "ByteArrayUtil.h"
#include "DigestUpdateRespPars.h"
#include "AbstractApduResponseParser_Import.h"

using namespace keyple::calypso::command::sam::parser;

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
using DigestUpdateRespPars =
    keyple::calypso::command::sam::parser::security::DigestUpdateRespPars;
using AbstractApduResponseParser =
    keyple::core::command::AbstractApduResponseParser;
using ApduResponse    = keyple::core::seproxy::message::ApduResponse;
using SeResponse      = keyple::core::seproxy::message::SeResponse;
using SeResponseSet   = keyple::core::seproxy::message::SeResponseSet;
using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
using ByteArrayUtils  = keyple::core::util::ByteArrayUtil;

void DigestUpdateRespParsTest::digestUpdateRespPars()
{
    std::vector<std::shared_ptr<ApduResponse>> responses;
    std::vector<char> cResp0 = std::vector<char>{90, 0};
    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(cResp0, nullptr);
    responses.push_back(apduResponse);
    std::vector<char> cResp1 = ByteArrayUtils::fromHex("9000");
    std::shared_ptr<SeResponseSet> seResponse =
        std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, std::make_shared<ApduResponse>(cResp1, nullptr), true),
            responses));

    std::shared_ptr<AbstractApduResponseParser> apduResponseParser =
        std::make_shared<DigestUpdateRespPars>(
            seResponse->getSingleResponse()->getApduResponses()[0]);
    std::vector<char> responseActual =
        apduResponseParser->getApduResponse()->getBytes();
    std::vector<char> cResp = std::vector<char>{90, 0};
    ASSERT_EQ(ByteArrayUtils::toHex(cResp),
              ByteArrayUtils::toHex(responseActual));
}
}
}
}
}
}

TEST(DigestUpdateRespParsTest, testA)
{
    std::shared_ptr<DigestUpdateRespParsTest> LocalTest =
        std::make_shared<DigestUpdateRespParsTest>();
    LocalTest->digestUpdateRespPars();
}

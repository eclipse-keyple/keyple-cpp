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

#include "AppendRecordRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "ByteArrayUtil.h"
#include "AppendRecordRespPars.h"
#include "AbstractApduResponseParser.h"

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

void AppendRecordRespParsTest::appendRecordRespPars()
{
    std::vector<std::shared_ptr<ApduResponse>> responses;
    std::vector<uint8_t> ApduRequest = {90, 0};
    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(ApduRequest, nullptr);
    responses.push_back(apduResponse);
    std::vector<uint8_t> cResp1 = ByteArrayUtils::fromHex("9000");
    std::shared_ptr<keyple::core::seproxy::message::AnswerToReset> atrBytes = {
        0};

    std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                atrBytes, std::make_shared<ApduResponse>(cResp1, nullptr),
                true),
            responses);

    std::shared_ptr<AbstractApduResponseParser> apduResponseParser =
        std::make_shared<AppendRecordRespPars>(apduResponse);
    apduResponseParser->setApduResponse(
        seResponse->getApduResponses()[0]);
    ASSERT_EQ(ByteArrayUtils::toHex(ApduRequest),
              ByteArrayUtils::toHex(
                  apduResponseParser->getApduResponse()->getBytes()));
}
}
}
}
}
}
}

TEST(AppendRecordRespParsTest, testA)
{
    std::shared_ptr<AppendRecordRespParsTest> LocalTest =
        std::make_shared<AppendRecordRespParsTest>();
    LocalTest->appendRecordRespPars();
}

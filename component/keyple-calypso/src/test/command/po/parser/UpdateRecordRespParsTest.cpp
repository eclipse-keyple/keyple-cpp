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

#include "UpdateRecordRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "ByteArrayUtil.h"
#include "UpdateRecordRespPars.h"
#include "AbstractApduResponseParser_Import.h"

using namespace keyple::calypso::command::po::parser;

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
using AbstractApduResponseParser =
    keyple::calypso::command::po::AbstractApduResponseParser;
using ApduResponse    = keyple::core::seproxy::message::ApduResponse;
using SeResponse      = keyple::core::seproxy::message::SeResponse;
using SeResponseSet   = keyple::core::seproxy::message::SeResponseSet;
using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
using ByteArrayUtils  = keyple::core::util::ByteArrayUtil;

void UpdateRecordRespParsTest::updateRecordRespPars()
{
    std::vector<std::shared_ptr<ApduResponse>> responses;
    std::vector<char> ApduRequest = {90, 0};
    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(ApduRequest, nullptr);
    responses.push_back(apduResponse);
    std::vector<char> Apdu1 = ByteArrayUtils::fromHex("9000");
    std::shared_ptr<SeResponseSet> seResponse =
        std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, std::make_shared<ApduResponse>(Apdu1, nullptr), true),
            responses));

    std::shared_ptr<AbstractApduResponseParser> apduResponseParser =
        std::make_shared<UpdateRecordRespPars>(apduResponse);
    apduResponseParser->setApduResponse(
        seResponse->getSingleResponse()->getApduResponses()[0]);
    ASSERT_EQ(ByteArrayUtils::toHex(ApduRequest),
              ByteArrayUtils::toHex(
                  apduResponseParser->getApduResponse()->getBytes()));
}
}
}
}
}
}

TEST(UpdateRecordRespParsTest, testA)
{
    std::shared_ptr<UpdateRecordRespParsTest> LocalTest =
        std::make_shared<UpdateRecordRespParsTest>();
    LocalTest->updateRecordRespPars();
}

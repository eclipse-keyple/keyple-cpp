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

#include <unordered_map>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "AbstractApduResponseParser.h"

using namespace keyple::core::command;

using namespace testing;

class AbstractApduResponseParserMock : public AbstractApduResponseParser {
public:
    AbstractApduResponseParserMock(
        const std::shared_ptr<ApduResponse>& response)
    : AbstractApduResponseParser(response)
    {
    }

    std::unordered_map<int, std::shared_ptr<StatusProperties>>
        getMockStatusTable()
    {
        return AbstractApduResponseParser::getStatusTable();
    }
};

TEST(AbstractApduResponseParserTest, AbstractApduResponseParser)
{
    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0X44};
    std::shared_ptr<std::set<int>> successfulStatusCodes =
        std::make_shared<std::set<int>>();
    successfulStatusCodes->insert(0x9000);
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(apdu, successfulStatusCodes);

    AbstractApduResponseParser parser(response);
}

TEST(AbstractApduResponseParserTest,
     AbstractApduResponseParser_getApduResponse)
{
    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0X44};
    std::shared_ptr<std::set<int>> successfulStatusCodes =
        std::make_shared<std::set<int>>();
    successfulStatusCodes->insert(0x9000);
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(apdu, successfulStatusCodes);

    AbstractApduResponseParser parser(response);

    /* Tests that response is properly set in contstructor */

    ASSERT_EQ(parser.getApduResponse(), response);
}

TEST(AbstractApduResponseParserTest,
     AbstractApduResponseParser_setApduResponse)
{
    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0x44};
    std::shared_ptr<std::set<int>> successfulStatusCodes =
        std::make_shared<std::set<int>>();
    successfulStatusCodes->insert(0x9000);
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(apdu, successfulStatusCodes);

    AbstractApduResponseParser parser(response);

    const std::vector<uint8_t> newApdu = {0x11, 0x22, 0x33, 0x55};
    std::shared_ptr<ApduResponse> newResponse =
        std::make_shared<ApduResponse>(newApdu, successfulStatusCodes);

    parser.setApduResponse(newResponse);

    /* Tests that response is modified after call to setApduResponse */

    ASSERT_EQ(parser.getApduResponse(), newResponse);
    ASSERT_NE(parser.getApduResponse(), response);
}

TEST(AbstractApduResponseParserTest,
     AbstractApduResponseParser_isSuccessful)
{
    const std::vector<uint8_t> okApdu = {0x90, 0x00};
    std::shared_ptr<std::set<int>> codes = std::make_shared<std::set<int>>();
    codes->insert(0x9000);
    std::shared_ptr<ApduResponse> okResp =
        std::make_shared<ApduResponse>(okApdu, codes);

    AbstractApduResponseParser parser(okResp);

    ASSERT_TRUE(parser.isSuccessful());

    const std::vector<uint8_t> badApdu = {0x6a, 0x82};
    std::shared_ptr<ApduResponse> badResp =
        std::make_shared<ApduResponse>(badApdu, codes);

    parser.setApduResponse(badResp);

    ASSERT_FALSE(parser.isSuccessful());
}

TEST(AbstractApduResponseParserTest,
     AbstractApduResponseParser_getStatusInformation)
{
    const std::vector<uint8_t> okApdu = {0x90, 0x00};
    std::shared_ptr<std::set<int>> codes = std::make_shared<std::set<int>>();
    codes->insert(0x9000);
    std::shared_ptr<ApduResponse> okResp =
        std::make_shared<ApduResponse>(okApdu, codes);

    AbstractApduResponseParser parser(okResp);

    ASSERT_EQ(parser.getStatusInformation(), "Success");

    const std::vector<uint8_t> badApdu = {0x6a, 0x82};
    std::shared_ptr<ApduResponse> badResp =
        std::make_shared<ApduResponse>(badApdu, codes);

    parser.setApduResponse(badResp);

    ASSERT_EQ(parser.getStatusInformation(), "");
}

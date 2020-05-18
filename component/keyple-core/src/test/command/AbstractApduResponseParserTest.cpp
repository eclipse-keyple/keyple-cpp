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

using StatusProperties = AbstractApduResponseParser::StatusProperties;

class AbstractApduResponseParserMock : public AbstractApduResponseParser {
public:
    AbstractApduResponseParserMock(
        const std::shared_ptr<ApduResponse>& response)
    : AbstractApduResponseParser(response)
    {
        STATUS_TABLE.emplace(
	    0x9999, std::make_shared<StatusProperties>(true, "sw 9999"));
        STATUS_TABLE.emplace(
	    0x6500, std::make_shared<StatusProperties>(false, "sw 6500"));
        STATUS_TABLE.emplace(
	    0x6400, std::make_shared<StatusProperties>(false, "sw 6400"));
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

TEST(AbstractApduResponseParserTest, getApduResponse)
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

TEST(AbstractApduResponseParserTest, setApduResponse)
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

TEST(AbstractApduResponseParserTest, isSuccessful)
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

TEST(AbstractApduResponseParserTest, getStatusInformation)
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

TEST(AbstractApduResponseParserTest, getStatusTable)
{
    const std::vector<uint8_t> apdu = {0x90, 0x00};
    std::shared_ptr<ApduResponse> resp =
        std::make_shared<ApduResponse>(apdu, nullptr);

    AbstractApduResponseParserMock parser(resp);

    std::unordered_map<int, std::shared_ptr<StatusProperties>> statusTable =
        parser.getMockStatusTable();

    ASSERT_EQ(4, statusTable.size());

    ASSERT_NE(statusTable.find(0x9000), statusTable.end());
    ASSERT_NE(statusTable.find(0x9999), statusTable.end());
    ASSERT_NE(statusTable.find(0x6500), statusTable.end());
    ASSERT_NE(statusTable.find(0x6400), statusTable.end());

    ASSERT_TRUE(statusTable[0x9000]->isSuccessful());
    ASSERT_TRUE(statusTable[0x9999]->isSuccessful());
    ASSERT_FALSE(statusTable[0x6500]->isSuccessful());
    ASSERT_FALSE(statusTable[0x6400]->isSuccessful());
}

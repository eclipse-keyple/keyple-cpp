/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "CloseSessionRespPars.h"

/* Common */
#include "IllegalArgumentException.h"

/* Core */
#include "ByteArrayUtil.h"
#include "SeResponse.h"
#include "SelectionStatus.h"

using namespace testing;

using namespace keyple::calypso::command::po::parser::security;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

TEST(CloseSessionRespParsTest, closeSessionRespPars)
{
    const std::vector<uint8_t> response = {0x4D, 0xBD, 0xC9, 0x60, 0x90, 0x00};
    std::vector<std::shared_ptr<ApduResponse>> responses;
    auto apduResponse = std::make_shared<ApduResponse>(response, nullptr);
    responses.push_back(apduResponse);
    const std::vector<uint8_t> resp = ByteArrayUtil::fromHex("9000");

    auto apdu = std::make_shared<ApduResponse>(resp, nullptr);
    auto status = std::make_shared<SelectionStatus>(nullptr, apdu, true);
    auto seResponse = std::make_shared<SeResponse>(true, true, status, responses);

    CloseSessionRespPars apduResponseParser(seResponse->getApduResponses()[0], nullptr);

    ASSERT_EQ(apduResponseParser.getApduResponse()->getBytes(), response);
}

TEST(CloseSessionRespParsTest, TestToPOHalfSessionSignature)
{
    const std::vector<uint8_t> apduResponse = {0x4D,0xBD,0xC9,0x60,0x90,0x00};
    const std::vector<uint8_t> apduResponseCaseThree = {0xA8, 0x31, 0xC3, 0x90, 0x00};
    const std::vector<uint8_t> sessionSignature = {0x4D, 0xBD, 0xC9, 0x60};
    const std::vector<uint8_t> sessionSignatureCaseTwo = {0xA7,0x21,0xC2,0x2E};
    const std::vector<uint8_t> apduResponseCaseTwo = {0xA8, 0x31, 0xC3, 0x3E, 0xA7, 0x21, 0xC2,
                                                      0x2E, 0x90, 0x00};

    /* Case Length = 4 */
    auto apdu = std::make_shared<ApduResponse>(apduResponse, nullptr);
    CloseSessionRespPars pars1(apdu, nullptr);
    ASSERT_EQ(pars1.getSignatureLo(), sessionSignature);

    /* Case Length = 8 */
    apdu = std::make_shared<ApduResponse>(apduResponseCaseTwo, nullptr);
    CloseSessionRespPars pars2(apdu, nullptr);
    ASSERT_EQ(pars2.getSignatureLo(), sessionSignatureCaseTwo);

    /* Case Other */
    apdu = std::make_shared<ApduResponse>(apduResponseCaseThree, nullptr);
    EXPECT_THROW(std::make_shared<CloseSessionRespPars>(apdu, nullptr),
                 IllegalArgumentException);
}

TEST(CloseSessionRespParsTest, existingTestConverted)
{
    const std::vector<uint8_t> resp = ByteArrayUtil::fromHex("9000");
    auto apdu = std::make_shared<ApduResponse>(resp, nullptr);
    CloseSessionRespPars parser(apdu, nullptr);

    /* This assert wasn't passing */
    ASSERT_EQ(ByteArrayUtil::toHex(parser.getSignatureLo()), "");
    ASSERT_EQ(ByteArrayUtil::toHex(parser.getPostponedData()), "");
}

TEST(CloseSessionRespParsTest, abortingASession)
{
    const std::vector<uint8_t> resp = ByteArrayUtil::fromHex("FEDCBA98 9000");
    auto apdu = std::make_shared<ApduResponse>(resp, nullptr);
    CloseSessionRespPars parser(apdu, nullptr);
}

TEST(CloseSessionRespParsTest, lc4withoutPostponedData)
{
    const std::vector<uint8_t> resp = ByteArrayUtil::fromHex("FEDCBA98 9000");
    auto apdu = std::make_shared<ApduResponse>(resp, nullptr);
    CloseSessionRespPars parser(apdu, nullptr);

    ASSERT_EQ(ByteArrayUtil::toHex(parser.getSignatureLo()), "FEDCBA98");
    ASSERT_EQ(ByteArrayUtil::toHex(parser.getPostponedData()), "");
}

TEST(CloseSessionRespParsTest, lc4WithPostponedData)
{
    const std::vector<uint8_t> resp = ByteArrayUtil::fromHex("04 345678 FEDCBA98 9000");
    auto apdu = std::make_shared<ApduResponse>(resp, nullptr);
    CloseSessionRespPars parser(apdu, nullptr);

    ASSERT_EQ(ByteArrayUtil::toHex(parser.getSignatureLo()), "FEDCBA98");
    ASSERT_EQ(ByteArrayUtil::toHex(parser.getPostponedData()), "345678");
}

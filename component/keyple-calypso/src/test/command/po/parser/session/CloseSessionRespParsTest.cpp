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

#include "CloseSessionRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "ByteArrayUtil.h"
#include "CloseSessionRespPars.h"
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

void CloseSessionRespParsTest::closeSessionRespPars()
{
    std::vector<uint8_t> response = {
        0x4D, 0xBD, 0xC9, 0x60, 0x90, 0x00};
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
        std::make_shared<CloseSessionRespPars>(
            seResponse->getApduResponses()[0]);

    ASSERT_EQ(ByteArrayUtils::toHex(response),
              ByteArrayUtils::toHex(
                  apduResponseParser->getApduResponse()->getBytes()));
}

void CloseSessionRespParsTest::TestToPOHalfSessionSignature()
{

    std::vector<uint8_t> apduResponse = {
        0x4D, 0xBD, 0xC9, 0x60, 0x90, 0x00};

    std::vector<uint8_t> apduResponseCaseTwo = {
        0xA8, 0x31, 0xC3, 0x3E, 0xA7, 0x21, 0xC2, 0x2E, 0x90, 0x00};

    std::vector<uint8_t> apduResponseCaseThree = {0xA8, 0x31, 0xC3, 0x90, 0x00};
    std::vector<uint8_t> sessionSignature = {0x4D, 0xBD, 0xC9, 0x60};
    std::vector<uint8_t> sessionSignatureCaseTwo = {0xA7, 0x21, 0xC2, 0x2E};

    { // Case Length = 4
        std::shared_ptr<CloseSessionRespPars> pars =
            std::make_shared<CloseSessionRespPars>(
                std::make_shared<ApduResponse>(apduResponse, nullptr));
        ASSERT_EQ(ByteArrayUtils::toHex(sessionSignature),
                  ByteArrayUtils::toHex(pars->getSignatureLo()));
    }

    { // Case Length = 8
        std::shared_ptr<CloseSessionRespPars> pars =
            std::make_shared<CloseSessionRespPars>(
                std::make_shared<ApduResponse>(apduResponseCaseTwo, nullptr));
        ASSERT_EQ(ByteArrayUtils::toHex(sessionSignatureCaseTwo),
                  ByteArrayUtils::toHex(pars->getSignatureLo()));
    }

    { // Case Other
        try {
            std::shared_ptr<CloseSessionRespPars> pars =
                std::make_shared<CloseSessionRespPars>(
                    std::make_shared<ApduResponse>(apduResponseCaseThree,
                                                   nullptr));
            ASSERT_NO_FATAL_FAILURE();
        } catch (const std::invalid_argument& ex) {
            /* expected case */
        }
    }
}

void CloseSessionRespParsTest::existingTestConverted()
{
    std::vector<uint8_t> cResp1 = ByteArrayUtils::fromHex("9000");
    std::shared_ptr<CloseSessionRespPars> parser =
        std::make_shared<CloseSessionRespPars>(
            std::make_shared<ApduResponse>(cResp1, nullptr));

    // This assert wasn't passing
    ASSERT_EQ("", ByteArrayUtils::toHex(parser->getSignatureLo()));
    ASSERT_EQ("", ByteArrayUtils::toHex(parser->getPostponedData()));
}

void CloseSessionRespParsTest::abortingASession()
{
    std::vector<uint8_t> cResp1 = ByteArrayUtils::fromHex("FEDCBA98 9000");
    std::shared_ptr<CloseSessionRespPars> parser =
        std::make_shared<CloseSessionRespPars>(
            std::make_shared<ApduResponse>(cResp1, nullptr));
}

void CloseSessionRespParsTest::lc4withoutPostponedData()
{
    std::vector<uint8_t> cResp1 = ByteArrayUtils::fromHex("FEDCBA98 9000");
    std::shared_ptr<CloseSessionRespPars> parser =
        std::make_shared<CloseSessionRespPars>(
            std::make_shared<ApduResponse>(cResp1, nullptr));
    ASSERT_EQ("FEDCBA98", ByteArrayUtils::toHex(parser->getSignatureLo()));
    ASSERT_EQ("", ByteArrayUtils::toHex(parser->getPostponedData()));
}

void CloseSessionRespParsTest::lc4WithPostponedData()
{
    std::vector<uint8_t> cResp1 =
        ByteArrayUtils::fromHex("04 345678 FEDCBA98 9000");
    std::shared_ptr<CloseSessionRespPars> parser =
        std::make_shared<CloseSessionRespPars>(
            std::make_shared<ApduResponse>(cResp1, nullptr));
    ASSERT_EQ("FEDCBA98", ByteArrayUtils::toHex(parser->getSignatureLo()));
    ASSERT_EQ("04345678", ByteArrayUtils::toHex(parser->getPostponedData()));
}
}
}
}
}
}
}

TEST(CloseSessionRespParsTest, testA)
{
    std::shared_ptr<CloseSessionRespParsTest> LocalTest =
        std::make_shared<CloseSessionRespParsTest>();
    LocalTest->closeSessionRespPars();
}

TEST(CloseSessionRespParsTest, testB)
{
    std::shared_ptr<CloseSessionRespParsTest> LocalTest =
        std::make_shared<CloseSessionRespParsTest>();
    LocalTest->TestToPOHalfSessionSignature();
}

TEST(CloseSessionRespParsTest, testC)
{
    std::shared_ptr<CloseSessionRespParsTest> LocalTest =
        std::make_shared<CloseSessionRespParsTest>();
    LocalTest->existingTestConverted();
}

TEST(CloseSessionRespParsTest, testD)
{
    std::shared_ptr<CloseSessionRespParsTest> LocalTest =
        std::make_shared<CloseSessionRespParsTest>();
    LocalTest->abortingASession();
}

TEST(CloseSessionRespParsTest, testE)
{
    std::shared_ptr<CloseSessionRespParsTest> LocalTest =
        std::make_shared<CloseSessionRespParsTest>();
    LocalTest->lc4withoutPostponedData();
}

TEST(CloseSessionRespParsTest, testF)
{
    std::shared_ptr<CloseSessionRespParsTest> LocalTest =
        std::make_shared<CloseSessionRespParsTest>();
    LocalTest->lc4WithPostponedData();
}

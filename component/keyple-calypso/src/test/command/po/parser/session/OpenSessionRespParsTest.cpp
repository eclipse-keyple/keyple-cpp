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

#include "OpenSessionRespParsTest.h"
#include "AbstractOpenSessionRespPars.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "PoRevision.h"
#include "util/TestsUtilsResponseTabByteGenerator.h"

using namespace keyple::calypso::command::po::parser::security;

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {
using PoRevision = keyple::calypso::command::po::PoRevision;
using TestsUtilsResponseTabByteGenerator =
    keyple::calypso::util::TestsUtilsResponseTabByteGenerator;
using ApduResponse    = keyple::core::seproxy::message::ApduResponse;
using SeResponse      = keyple::core::seproxy::message::SeResponse;
using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
using ByteArrayUtils  = keyple::core::util::ByteArrayUtil;

void OpenSessionRespParsTest::check(
    std::shared_ptr<AbstractOpenSessionRespPars> resp)
{
    ASSERT_TRUE(resp->isSuccessful());
}

void OpenSessionRespParsTest::testgetResponse_rev2_4()
{

    // expected response
    std::vector<uint8_t> cResp1 =
        ByteArrayUtils::fromHex("CC 11223344 00112233445566778899AABBCCDDEEFF "
                                "00112233445566778899AABBCC 9000");
    std::shared_ptr<ApduResponse> responseMockOpenSecureSession =
        std::make_shared<ApduResponse>(cResp1, nullptr);
    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(responseMockOpenSecureSession);

    std::shared_ptr<SeResponse> responseMock =
        std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, responseMockOpenSecureSession, true),
            apduResponses);

    std::shared_ptr<ApduResponse> response =
        responseMock->getApduResponses()[0];

    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV2_4));
}

void OpenSessionRespParsTest::testgetResponse_rev2_4_no_data()
{

    // expected response
    std::vector<uint8_t> cResp1 = ByteArrayUtils::fromHex("CC 11223344 9000");
    std::shared_ptr<ApduResponse> responseMockOpenSecureSession =
        std::make_shared<ApduResponse>(cResp1, nullptr);
    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(responseMockOpenSecureSession);

    std::shared_ptr<SeResponse> responseMock =
        std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, responseMockOpenSecureSession, true),
            apduResponses);
    std::shared_ptr<ApduResponse> response =
        responseMock->getApduResponses()[0];

    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV2_4));
}

void OpenSessionRespParsTest::testgetResponse_rev2_4_non_ratified()
{

    // expected response
    std::vector<uint8_t> cResp1 = ByteArrayUtils::fromHex(
        "CC 11223344 9999 00112233445566778899AABBCCDDEEFF "
        "00112233445566778899AABBCC 9000");
    std::shared_ptr<ApduResponse> responseMockOpenSecureSession =
        std::make_shared<ApduResponse>(cResp1, nullptr);
    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(responseMockOpenSecureSession);

    std::shared_ptr<SeResponse> responseMock =
        std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, responseMockOpenSecureSession, true),
            apduResponses);
    std::shared_ptr<ApduResponse> response =
        responseMock->getApduResponses()[0];

    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV2_4));
}

void OpenSessionRespParsTest::testgetResponse_rev2_4_no_data_non_ratified()
{

    // expected response
    std::vector<uint8_t> cResp1 = ByteArrayUtils::fromHex("CC 11223344 9999 9000");
    std::shared_ptr<ApduResponse> responseMockOpenSecureSession =
        std::make_shared<ApduResponse>(cResp1, nullptr);
    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(responseMockOpenSecureSession);

    std::shared_ptr<SeResponse> responseMock =
        std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, responseMockOpenSecureSession, true),
            apduResponses);
    std::shared_ptr<ApduResponse> response =
        responseMock->getApduResponses()[0];

    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV2_4));
}

void OpenSessionRespParsTest::testgetResponse_rev2_4_bad_length_inf()
{

    // expected response
    std::vector<uint8_t> cResp1 = ByteArrayUtils::fromHex(
        "CC 11223344 9999 00112233445566778899AABBCCDDEEFF "
        "00112233445566778899AABBCCDDEEFF 9000");
    std::shared_ptr<ApduResponse> responseMockOpenSecureSession =
        std::make_shared<ApduResponse>(cResp1, nullptr);
    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(responseMockOpenSecureSession);

    std::shared_ptr<SeResponse> responseMock =
        std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, responseMockOpenSecureSession, true),
            apduResponses);
    std::shared_ptr<ApduResponse> response =
        responseMock->getApduResponses()[0];

    try {
        /* code */
        check(
            AbstractOpenSessionRespPars::create(response, PoRevision::REV2_4));
    } catch (...) {
    }
}

void OpenSessionRespParsTest::testgetResponse_rev2_4_bad_length_sup()
{

    // expected response
    std::vector<uint8_t> cResp1 = ByteArrayUtils::fromHex(
        "CC 11223344 9999 00112233445566778899AABBCCDDEEFF 9000");
    std::shared_ptr<ApduResponse> responseMockOpenSecureSession =
        std::make_shared<ApduResponse>(cResp1, nullptr);
    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(responseMockOpenSecureSession);

    std::shared_ptr<SeResponse> responseMock =
        std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, responseMockOpenSecureSession, true),
            apduResponses);
    std::shared_ptr<ApduResponse> response =
        responseMock->getApduResponses()[0];

    try {
        /* code */
        check(
            AbstractOpenSessionRespPars::create(response, PoRevision::REV2_4));
    } catch (...) {
    }
}

void OpenSessionRespParsTest::testgetResponse_rev3_1()
{

    // expected response
    std::shared_ptr<ApduResponse> responseMockFci =
        TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev3_1();
    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(responseMockFci);

    std::shared_ptr<SeResponse> responseMock =
        std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(nullptr, responseMockFci, true),
            apduResponses);
    std::shared_ptr<ApduResponse> response =
        responseMock->getApduResponses()[0];

    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV3_1));
}

void OpenSessionRespParsTest::testgetResponse_rev3_2()
{

    // expected response
    std::shared_ptr<ApduResponse> responseMockOS =
        TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev3_2();
    std::shared_ptr<ApduResponse> responseMockFci =
        TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev3_2();
    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    apduResponses.push_back(responseMockOS);

    std::shared_ptr<SeResponse> responseMock =
        std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(nullptr, responseMockFci, true),
            apduResponses);
    std::shared_ptr<ApduResponse> response =
        responseMock->getApduResponses()[0];

    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV3_2));
}
}
}
}
}
}
}

TEST(OpenSessionRespParsTest, testA)
{
    std::shared_ptr<OpenSessionRespParsTest> LocalTest =
        std::make_shared<OpenSessionRespParsTest>();
    LocalTest->testgetResponse_rev2_4();
}

TEST(OpenSessionRespParsTest, testB)
{
    std::shared_ptr<OpenSessionRespParsTest> LocalTest =
        std::make_shared<OpenSessionRespParsTest>();
    LocalTest->testgetResponse_rev2_4_no_data();
}

TEST(OpenSessionRespParsTest, testC)
{
    std::shared_ptr<OpenSessionRespParsTest> LocalTest =
        std::make_shared<OpenSessionRespParsTest>();
    LocalTest->testgetResponse_rev2_4_non_ratified();
}

TEST(OpenSessionRespParsTest, testD)
{
    std::shared_ptr<OpenSessionRespParsTest> LocalTest =
        std::make_shared<OpenSessionRespParsTest>();
    LocalTest->testgetResponse_rev2_4_no_data_non_ratified();
}

TEST(OpenSessionRespParsTest, testE)
{
    std::shared_ptr<OpenSessionRespParsTest> LocalTest =
        std::make_shared<OpenSessionRespParsTest>();
    LocalTest->testgetResponse_rev2_4_bad_length_inf();
}

TEST(OpenSessionRespParsTest, testF)
{
    std::shared_ptr<OpenSessionRespParsTest> LocalTest =
        std::make_shared<OpenSessionRespParsTest>();
    LocalTest->testgetResponse_rev2_4_bad_length_sup();
}

TEST(OpenSessionRespParsTest, testG)
{
    std::shared_ptr<OpenSessionRespParsTest> LocalTest =
        std::make_shared<OpenSessionRespParsTest>();
    LocalTest->testgetResponse_rev3_1();
}

TEST(OpenSessionRespParsTest, testH)
{
    std::shared_ptr<OpenSessionRespParsTest> LocalTest =
        std::make_shared<OpenSessionRespParsTest>();
    LocalTest->testgetResponse_rev3_2();
}

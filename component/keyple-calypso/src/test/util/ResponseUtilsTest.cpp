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

#include "ResponseUtilsTest.h"
#include "AbstractOpenSessionRespPars.h"
#include "OpenSession32RespPars.h"
#include "OpenSession24RespPars.h"
#include "ByteArrayUtil.h"
#include "Byte.h"

using namespace keyple::calypso::util;

namespace keyple {
namespace calypso {
namespace util {
using AbstractOpenSessionRespPars =
    keyple::calypso::command::po::parser::security::AbstractOpenSessionRespPars;
using OpenSession24RespPars =
    keyple::calypso::command::po::parser::security::OpenSession24RespPars;
using OpenSession32RespPars =
    keyple::calypso::command::po::parser::security::OpenSession32RespPars;
using ByteArrayUtils = keyple::core::util::ByteArrayUtil;
using Byte           = keyple::common::Byte;

void ResponseUtilsTest::TestToSecureSession()
{
    std::vector<char> apduResponse = {static_cast<char>(0x8F),
                                      0x05,
                                      0x75,
                                      0x1A,
                                      0x00,
                                      0x00,
                                      0x00,
                                      0x00,
                                      0x00,
                                      0x00,
                                      0x00,
                                      0x00,
                                      0x30,
                                      0x7E,
                                      static_cast<char>(0x1D),
                                      0x00,
                                      0x00,
                                      0x00,
                                      0x00,
                                      0x00,
                                      0x00,
                                      0x00,
                                      0x00,
                                      0x00};

    std::vector<char> transactionCounter = {static_cast<char>(0x8F), 0x05,
                                            0x75};
    std::vector<char> randomNumber       = {0x1A, 0x00, 0x00, 0x00, 0x00};
    char kif                             = 0x00;
    std::shared_ptr<Byte> kvc = std::make_shared<Byte>(static_cast<char>(0x00));

    bool isPreviousSessionRatifiedExpected       = true;
    bool isManageSecureSessionAuthorizedExpected = false;
    std::vector<char> originalData(0);

    std::shared_ptr<AbstractOpenSessionRespPars::SecureSession>
        SecureSessionExpected =
            std::make_shared<AbstractOpenSessionRespPars::SecureSession>(
                transactionCounter, randomNumber,
                isPreviousSessionRatifiedExpected,
                isManageSecureSessionAuthorizedExpected, kif, kvc, originalData,
                apduResponse);
    std::shared_ptr<AbstractOpenSessionRespPars::SecureSession>
        SecureSessionTested =
            OpenSession32RespPars::createSecureSession(apduResponse);

    ASSERT_EQ(ByteArrayUtils::toHex(SecureSessionExpected->getOriginalData()),
              ByteArrayUtils::toHex(SecureSessionTested->getOriginalData()));
    ASSERT_EQ(
        ByteArrayUtils::toHex(SecureSessionExpected->getSecureSessionData()),
        ByteArrayUtils::toHex(SecureSessionTested->getSecureSessionData()));
    ASSERT_EQ(SecureSessionExpected->getKIF(), SecureSessionTested->getKIF());
    ASSERT_EQ(SecureSessionExpected->getKVC()->byteValue(),
              SecureSessionTested->getKVC()->byteValue());
    ASSERT_EQ(
        ByteArrayUtils::toHex(
            SecureSessionExpected->getChallengeRandomNumber()),
        ByteArrayUtils::toHex(SecureSessionTested->getChallengeRandomNumber()));
    ASSERT_EQ(ByteArrayUtils::toHex(
                  SecureSessionExpected->getChallengeTransactionCounter()),
              ByteArrayUtils::toHex(
                  SecureSessionTested->getChallengeTransactionCounter()));
}

void ResponseUtilsTest::TestToSecureSessionRev2()
{

    // Case Else
    std::vector<char> apduResponse = {
        static_cast<char>(0x7E), static_cast<char>(0x03),
        static_cast<char>(0x0D), static_cast<char>(0x14),
        static_cast<char>(0x53)};

    std::vector<char> transactionCounter = {static_cast<char>(0x03),
                                            static_cast<char>(0x0D),
                                            static_cast<char>(0x14)};
    std::vector<char> randomNumber       = {static_cast<char>(0x53)};
    std::shared_ptr<Byte> kvc = std::make_shared<Byte>(static_cast<char>(0x7E));

    bool isPreviousSessionRatifiedExpected       = false;
    bool isManageSecureSessionAuthorizedExpected = false;
    std::vector<char> originalData;

    std::shared_ptr<AbstractOpenSessionRespPars::SecureSession>
        SecureSessionExpected =
            std::make_shared<AbstractOpenSessionRespPars::SecureSession>(
                transactionCounter, randomNumber,
                isPreviousSessionRatifiedExpected,
                isManageSecureSessionAuthorizedExpected, kvc, originalData,
                apduResponse);
    std::shared_ptr<AbstractOpenSessionRespPars::SecureSession>
        SecureSessionTested =
            OpenSession24RespPars::createSecureSession(apduResponse);

    ASSERT_EQ(
        ByteArrayUtils::toHex(SecureSessionExpected->getSecureSessionData()),
        ByteArrayUtils::toHex(SecureSessionTested->getSecureSessionData()));
    ASSERT_EQ(SecureSessionExpected->getKVC()->byteValue(),
              SecureSessionTested->getKVC()->byteValue());
    ASSERT_EQ(
        ByteArrayUtils::toHex(
            SecureSessionExpected->getChallengeRandomNumber()),
        ByteArrayUtils::toHex(SecureSessionTested->getChallengeRandomNumber()));
    ASSERT_EQ(ByteArrayUtils::toHex(
                  SecureSessionExpected->getChallengeTransactionCounter()),
              ByteArrayUtils::toHex(
                  SecureSessionTested->getChallengeTransactionCounter()));

    // Case If Else
    // byte[] apduResponseCaseTwo = new byte[] {(byte) 0x7E, (byte) 0x03,
    // (byte) 0x0D, (byte) 0x14,
    // (byte) 0x53, (byte) 0x30, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04};
    // byte[] originalDataCaseTwo = new byte[] {(byte) 0x7E, (byte) 0x03,
    // (byte) 0x0D, (byte) 0x14,
    // (byte) 0x53, (byte) 0xFF, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04};
    std::vector<char> apduResponseCaseTwo = ByteArrayUtils::fromHex(
        "7E 030D1453 9999 00112233445566778899AABBCCDDEEFF "
        "00112233445566778899AABBCC");
    std::vector<char> originalDataCaseTwo = ByteArrayUtils::fromHex(
        "7E 030D1453 9999 00112233445566778899AABBCCDDEEFF "
        "00112233445566778899AABBCC");

    std::shared_ptr<AbstractOpenSessionRespPars::SecureSession>
        SecureSessionExpectedCaseTwo =
            std::make_shared<AbstractOpenSessionRespPars::SecureSession>(
                transactionCounter, randomNumber,
                isPreviousSessionRatifiedExpected,
                isManageSecureSessionAuthorizedExpected, kvc,
                originalDataCaseTwo, apduResponseCaseTwo);
    std::shared_ptr<AbstractOpenSessionRespPars::SecureSession>
        SecureSessionTestedCaseTwo =
            OpenSession24RespPars::createSecureSession(apduResponseCaseTwo);

    ASSERT_EQ(ByteArrayUtils::toHex(
                  SecureSessionExpectedCaseTwo->getSecureSessionData()),
              ByteArrayUtils::toHex(
                  SecureSessionTestedCaseTwo->getSecureSessionData()));
    ASSERT_EQ(SecureSessionExpectedCaseTwo->getKVC()->byteValue(),
              SecureSessionTestedCaseTwo->getKVC()->byteValue());
    ASSERT_EQ(ByteArrayUtils::toHex(
                  SecureSessionExpectedCaseTwo->getChallengeRandomNumber()),
              ByteArrayUtils::toHex(
                  SecureSessionTestedCaseTwo->getChallengeRandomNumber()));
    ASSERT_EQ(
        ByteArrayUtils::toHex(
            SecureSessionExpectedCaseTwo->getChallengeTransactionCounter()),
        ByteArrayUtils::toHex(
            SecureSessionTestedCaseTwo->getChallengeTransactionCounter()));

    // Case If If
    // byte[] apduResponseCaseThree = new byte[] {(byte) 0x7E, (byte) 0x03,
    // (byte) 0x0D, (byte) 0x14, (byte) 0x53, (byte) 0xFF, 0x00, 0x04, 0x01,
    // 0x02, 0x03, 0x04};
    // byte[] originalDataCaseThree = new byte[] {(byte) 0x7E, (byte) 0x03,
    // (byte) 0x0D, (byte) 0x14, (byte) 0x53, (byte) 0xFF, 0x00, 0x04, 0x01,
    // 0x02, 0x03, 0x04};

    std::vector<char> apduResponseCaseThree = ByteArrayUtils::fromHex(
        "7E 030D1453 9999 00112233445566778899AABBCCDDEEFF "
        "00112233445566778899AABBCC");
    std::vector<char> originalDataCaseThree = ByteArrayUtils::fromHex(
        "7E 030D1453 9999 00112233445566778899AABBCCDDEEFF "
        "00112233445566778899AABBCC");

    std::shared_ptr<AbstractOpenSessionRespPars::SecureSession>
        SecureSessionExpectedCaseThree =
            std::make_shared<AbstractOpenSessionRespPars::SecureSession>(
                transactionCounter, randomNumber,
                isPreviousSessionRatifiedExpected,
                isManageSecureSessionAuthorizedExpected, kvc,
                originalDataCaseThree, apduResponseCaseThree);
    std::shared_ptr<AbstractOpenSessionRespPars::SecureSession>
        SecureSessionTestedCaseThree =
            OpenSession24RespPars::createSecureSession(apduResponseCaseThree);

    ASSERT_EQ(ByteArrayUtils::toHex(
                  SecureSessionExpectedCaseThree->getSecureSessionData()),
              ByteArrayUtils::toHex(
                  SecureSessionTestedCaseThree->getSecureSessionData()));
    ASSERT_EQ(SecureSessionExpectedCaseThree->getKVC()->byteValue(),
              SecureSessionTestedCaseThree->getKVC()->byteValue());
    ASSERT_EQ(ByteArrayUtils::toHex(
                  SecureSessionExpectedCaseThree->getChallengeRandomNumber()),
              ByteArrayUtils::toHex(
                  SecureSessionTestedCaseThree->getChallengeRandomNumber()));
    ASSERT_EQ(
        ByteArrayUtils::toHex(
            SecureSessionExpectedCaseThree->getChallengeTransactionCounter()),
        ByteArrayUtils::toHex(
            SecureSessionTestedCaseThree->getChallengeTransactionCounter()));
}
}
}
}

TEST(ResponseUtilsTest, testA)
{
    std::shared_ptr<ResponseUtilsTest> LocalTest =
        std::make_shared<ResponseUtilsTest>();
    LocalTest->TestToSecureSession();
}

TEST(ResponseUtilsTest, testB)
{
    std::shared_ptr<ResponseUtilsTest> LocalTest =
        std::make_shared<ResponseUtilsTest>();
    LocalTest->TestToSecureSessionRev2();
}

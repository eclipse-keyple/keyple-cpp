/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

/* Calypso */
#include "AbstractOpenSessionCmdBuild.h"
#include "AbstractOpenSessionRespPars.h"
#include "OpenSession10CmdBuild.h"
#include "OpenSession24CmdBuild.h"
#include "OpenSession31CmdBuild.h"
#include "OpenSession32CmdBuild.h"
#include "PoRevision.h"
#include "PoTransaction.h"

/* Common */
#include "stringhelper.h"

/* Core */
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "KeypleSeCommandException.h"

using namespace testing;

using namespace keyple::calypso::command::po::builder;
using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::calypso::command::po::parser::security;
using namespace keyple::calypso::transaction;
using namespace keyple::common;
using namespace keyple::core::command::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

using AccessLevel = PoTransaction::SessionSetting::AccessLevel;

static const std::string SW1SW2_OK = "9000";
static const std::string SW1SW2_KO = "6A83";
static const std::string SAM_CHALLENGE_4 = "11223344";
static const std::string SAM_CHALLENGE_8 = "1122334444332211";
static const std::string PO_TN = "112233";
static const std::string PO_RANDOM_1 = "44";
static const std::string PO_RANDOM_5 = "4433221100";
static const std::string RATIFIED = "00";
static const std::string NOT_RATIFIED = "01";
static const std::string REV10_24_RATIFICATION_BYTES = "AABB";
static const std::string KIF = "12";
static const std::string KIF_FF = "FF";
static const std::string KVC = "34";
static const std::string KVC_FF = "FF";
static const std::string DATA =
    "0011223344556677889900112233445566778899001122334455667788";
static const std::string DATA_LENGTH =
    StringHelper::uint8ToHexString(DATA.length() / 2);
static const std::string DATA_EMPTY = "";
static const std::string DATA_LENGTH_0 = "00";

static const uint8_t SFI = 0x01;
static const uint8_t REC = 1;

static const std::string OPEN_SECURE_SESSION_RESP_10_KVC_RATIFIED =
    PO_TN + PO_RANDOM_1;
static const std::string OPEN_SECURE_SESSION_RESP_10_KVC_NOT_RATIFIED =
    PO_TN + PO_RANDOM_1 + REV10_24_RATIFICATION_BYTES;
static const std::string OPEN_SECURE_SESSION_RESP_24_KVC_RATIFIED =
    KVC + PO_TN + PO_RANDOM_1;
static const std::string OPEN_SECURE_SESSION_RESP_24_KVC_NOT_RATIFIED =
    KVC + PO_TN + PO_RANDOM_1 + REV10_24_RATIFICATION_BYTES;
static const std::string OPEN_SECURE_SESSION_RESP_31_RATIFIED =
    PO_TN + PO_RANDOM_1 + RATIFIED + KIF + KVC;
static const std::string OPEN_SECURE_SESSION_RESP_31_NOT_RATIFIED =
    PO_TN + PO_RANDOM_1 + NOT_RATIFIED + KIF + KVC;
static const std::string OPEN_SECURE_SESSION_RESP_32_RATIFIED =
    PO_TN + PO_RANDOM_5 + RATIFIED + KIF + KVC;
static const std::string OPEN_SECURE_SESSION_RESP_32_NOT_RATIFIED =
    PO_TN + PO_RANDOM_5 + NOT_RATIFIED + KIF + KVC;


TEST(OpenSessionRespParsTest, openSessionRespPars_rev1_0_readingData)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_10_KVC_RATIFIED +
                                   DATA +
                                   SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV1_0,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                SFI,
                REC);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_1));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF_FF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC_FF)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA));
    ASSERT_TRUE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest, openSessionRespPars_rev1_0_notReadingData)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_10_KVC_RATIFIED +
                                   SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV1_0,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                0,
                0);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
            openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_1));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF_FF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC_FF)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA_EMPTY));
    ASSERT_TRUE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest,
     openSessionRespPars_rev1_0_readingData_notRatified)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_10_KVC_NOT_RATIFIED
                                   + DATA
                                   + SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV1_0,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                SFI,
                REC);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_1));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF_FF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC_FF)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA));
    ASSERT_FALSE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest,
     openSessionRespPars_rev1_0_notReadingData_notRatified)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_10_KVC_NOT_RATIFIED
                                   + SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV1_0,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                0,
                0);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_1));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF_FF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC_FF)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA_EMPTY));
    ASSERT_FALSE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest, openSessionRespPars_rev1_0_badStatus)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_KO),
                                       nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV1_0,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                0,
                0);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    EXPECT_THROW(responseParser->checkStatus(), KeypleSeCommandException);
}

TEST(OpenSessionRespParsTest, openSessionRespPars_rev2_4_readingData)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_24_KVC_RATIFIED +
                                   DATA +
                                   SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV2_4,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                SFI,
                REC);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_1));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF_FF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA));
    ASSERT_TRUE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest, openSessionRespPars_rev2_4_notReadingData)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_24_KVC_RATIFIED +
                                   SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV2_4,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                0,
                0);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_1));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF_FF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA_EMPTY));
    ASSERT_TRUE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest,
     openSessionRespPars_rev2_4_readingData_notRatified)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_24_KVC_NOT_RATIFIED
                                   + DATA
                                   + SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV2_4,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                SFI,
                REC);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_1));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF_FF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA));
    ASSERT_FALSE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest,
     openSessionRespPars_rev2_4_notReadingData_notRatified)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_24_KVC_NOT_RATIFIED
                                   + SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV2_4,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                0,
                0);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_1));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF_FF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA_EMPTY));
    ASSERT_FALSE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest, openSessionRespPars_rev2_4_badStatus)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(SW1SW2_KO), nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV2_4,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                0,
                0);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    EXPECT_THROW(responseParser->checkStatus(), KeypleSeCommandException);
}

TEST(OpenSessionRespParsTest, openSessionRespPars_rev3_1_readingData)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_31_RATIFIED +
                                   DATA_LENGTH +
                                   DATA +
                                   SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV3_1,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                SFI,
                REC);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_1));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA));
    ASSERT_TRUE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest, openSessionRespPars_rev3_1_notReadingData)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_31_RATIFIED +
                                   DATA_LENGTH_0 +
                                   SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV3_1,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                0,
                0);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_1));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA_EMPTY));
    ASSERT_TRUE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest,
     openSessionRespPars_rev3_1_readingData_notRatified)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_31_NOT_RATIFIED +
                                   DATA_LENGTH +
                                   DATA +
                                   SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV3_1,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                SFI,
                REC);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_1));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA));
    ASSERT_FALSE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest,
     openSessionRespPars_rev3_1_notReadingData_notRatified)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_31_NOT_RATIFIED +
                                   DATA_LENGTH_0 +
                                   SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV3_1,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                0,
                0);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);
    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_1));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA_EMPTY));
    ASSERT_FALSE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest, openSessionRespPars_rev3_1_badStatus)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(SW1SW2_KO), nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV3_1,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_4),
                0,
                0);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    EXPECT_THROW(responseParser->checkStatus(), KeypleSeCommandException);
}

TEST(OpenSessionRespParsTest, openSessionRespPars_rev3_2_readingData)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_32_RATIFIED +
                                   DATA_LENGTH +
                                   DATA +
                                   SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV3_2,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_8),
                SFI,
                REC);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_5));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA));
    ASSERT_TRUE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest, openSessionRespPars_rev3_2_notReadingData)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_32_RATIFIED +
                                   DATA_LENGTH_0 +
                                   SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV3_2,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_8),
                0,
                0);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_5));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA_EMPTY));
    ASSERT_TRUE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest,
     openSessionRespPars_rev3_2_readingData_notRatified)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_32_NOT_RATIFIED +
                                   DATA_LENGTH +
                                   DATA +
                                   SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV3_2,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_8),
                SFI,
                REC);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_5));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA));
    ASSERT_FALSE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest,
     openSessionRespPars_rev3_2_notReadingData_notRatified)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(OPEN_SECURE_SESSION_RESP_32_NOT_RATIFIED +
                                   DATA_LENGTH_0 +
                                   SW1SW2_OK),
            nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV3_2,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_8),
                0,
                0);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    responseParser->checkStatus();

    ASSERT_EQ(responseParser->getPoChallenge(),
              ByteArrayUtil::fromHex(PO_RANDOM_5));
    ASSERT_EQ(responseParser->getTransactionCounterValue(),
              ByteArrayUtil::threeBytesToInt(ByteArrayUtil::fromHex(PO_TN), 0));
    ASSERT_EQ(responseParser->getSelectedKif(),
              ByteArrayUtil::fromHex(KIF)[0]);
    ASSERT_EQ(responseParser->getSelectedKvc(),
              ByteArrayUtil::fromHex(KVC)[0]);
    ASSERT_EQ(responseParser->getRecordDataRead(),
              ByteArrayUtil::fromHex(DATA_EMPTY));
    ASSERT_FALSE(responseParser->wasRatified());
}

TEST(OpenSessionRespParsTest, openSessionRespPars_rev3_2_badStatus)
{
    std::shared_ptr<ApduResponse> response =
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(SW1SW2_KO), nullptr);

    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV3_2,
                AccessLevel::SESSION_LVL_DEBIT.getSessionKey(),
                ByteArrayUtil::fromHex(SAM_CHALLENGE_8),
                0,
                0);

    std::shared_ptr<AbstractOpenSessionRespPars> responseParser =
        openSessionCmdBuild->createResponseParser(response);

    EXPECT_THROW(responseParser->checkStatus(), KeypleSeCommandException);
}

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

#include "VerifyPinRespPars.h"

/* Calypso */
#include "CalypsoPoCommandException.h"
#include "CalypsoPoPinException.h"

/* Common */
#include "IllegalArgumentException.h"
#include "IllegalStateException.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::po::exception;
using namespace keyple::calypso::command::po::parser::security;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

static const std::vector<uint8_t> SW1SW2_KO = ByteArrayUtil::fromHex("6A82");
static const std::vector<uint8_t> SW1SW2_OK = ByteArrayUtil::fromHex("9000");
static const std::vector<uint8_t> ATTEMPTS_1 = ByteArrayUtil::fromHex("63C1");
static const std::vector<uint8_t> ATTEMPTS_2 = ByteArrayUtil::fromHex("63C2");
static const std::vector<uint8_t> PIN_BLOCKED = ByteArrayUtil::fromHex("6983");

TEST(VerifyPinRespParsTest, verifyPinRespPars_goodStatus)
{
    VerifyPinRespPars parser(std::make_shared<ApduResponse>(SW1SW2_OK, nullptr), nullptr);
    parser.checkStatus();

    ASSERT_EQ(parser.getRemainingAttemptCounter(), 3);
}

TEST(VerifyPinRespParsTest, verifyPinRespPars_badStatus)
{
    VerifyPinRespPars parser(std::make_shared<ApduResponse>(SW1SW2_KO, nullptr), nullptr);

    EXPECT_THROW(parser.checkStatus(), CalypsoPoCommandException);
}

TEST(VerifyPinRespParsTest, verifyPinRespPars_badStatus_2)
{
    VerifyPinRespPars parser(std::make_shared<ApduResponse>(SW1SW2_KO, nullptr), nullptr);

    EXPECT_THROW(parser.getRemainingAttemptCounter(), IllegalStateException);
}

TEST(VerifyPinRespParsTest, verifyPinRespPars_attempts_1)
{
    VerifyPinRespPars parser(std::make_shared<ApduResponse>(ATTEMPTS_1, nullptr), nullptr);

    ASSERT_EQ(parser.getRemainingAttemptCounter(), 1);
    EXPECT_THROW(parser.checkStatus(), CalypsoPoPinException);
}

TEST(VerifyPinRespParsTest, verifyPinRespPars_attempts_2)
{
    VerifyPinRespPars parser(std::make_shared<ApduResponse>(ATTEMPTS_2, nullptr), nullptr);

    ASSERT_EQ(parser.getRemainingAttemptCounter(), 2);
    EXPECT_THROW(parser.checkStatus(), CalypsoPoPinException);
}

TEST(VerifyPinRespParsTest, verifyPinRespPars_pin_blocked)
{
    VerifyPinRespPars parser(std::make_shared<ApduResponse>(PIN_BLOCKED, nullptr), nullptr);

    ASSERT_EQ(parser.getRemainingAttemptCounter(), 0);
    EXPECT_THROW(parser.checkStatus(), CalypsoPoPinException);
}

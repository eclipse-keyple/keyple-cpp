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

#include "IncreaseRespPars.h"

/* Common */
#include "stringhelper.h"

/* Core */
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "KeypleSeCommandException.h"

using namespace testing;

using namespace keyple::calypso::command::po::parser;
using namespace keyple::common;
using namespace keyple::core::command::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

static const std::string SW1SW2_KO = "6A82";
static const std::string SW1SW2_OK = "9000";
static const std::string APDU_BAD_SW1SW2 = SW1SW2_KO;
static const int NEW_VALUE = 0x123456;
static const std::string APDU_INCREASE =
    StringHelper::uint24ToHexString(NEW_VALUE) + SW1SW2_OK;

TEST(IncreaseRespParsTest, decreaseRespPars_badStatus)
{
    IncreaseRespPars increaseRespPars(
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(APDU_BAD_SW1SW2), nullptr),
        nullptr);

    EXPECT_THROW(increaseRespPars.checkStatus(), KeypleSeCommandException);
}

TEST(IncreaseRespParsTest, decreaseRespPars_goodStatus)
{
    IncreaseRespPars increaseRespPars(
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(APDU_INCREASE), nullptr),
        nullptr);

    increaseRespPars.checkStatus();
}

TEST(IncreaseRespParsTest, decreaseRespPars_getNewValue)
{
    IncreaseRespPars increaseRespPars(
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(APDU_INCREASE), nullptr),
        nullptr);

    increaseRespPars.checkStatus();

    ASSERT_EQ(increaseRespPars.getNewValue(), NEW_VALUE);
}

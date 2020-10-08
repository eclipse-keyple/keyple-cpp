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

#include "SamReadEventCounterRespPars.h"

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleSeCommandException.h"

using namespace testing;

using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::core::command::exception;
using namespace keyple::core::util;

static const std::string SW1SW2_KO = "6A00";
static const std::string SW1SW2_OK = "9000";
static const std::string COUNTER_DATA = "11223344";
static const std::string APDU_READ_COUNTER_DATA = COUNTER_DATA + SW1SW2_OK;

TEST(SamReadEventCounterRespParsTest, badStatus)
{
    SamReadEventCounterRespPars samReadEventCounterRespPars(
        std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_KO),
                                       nullptr),
        nullptr);

    EXPECT_THROW(samReadEventCounterRespPars.checkStatus(),
                 KeypleSeCommandException);
}

TEST(SamReadEventCounterRespParsTest, goodStatus_getSignature)
{
    SamReadEventCounterRespPars samReadEventCounterRespPars(
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(APDU_READ_COUNTER_DATA), nullptr),
        nullptr);

    samReadEventCounterRespPars.checkStatus();

    ASSERT_EQ(samReadEventCounterRespPars.getCounterData(),
              ByteArrayUtil::fromHex(COUNTER_DATA));
}

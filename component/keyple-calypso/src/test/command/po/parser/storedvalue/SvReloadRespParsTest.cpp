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

#include "SvReloadRespPars.h"

/* Common */
#include "IllegalStateException.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::po::parser::storedvalue;
using namespace keyple::common::exception;
using namespace keyple::core::util;

TEST(SvReloadRespParsTest, getSignatureLo_mode_compat)
{
    auto apduResponse = std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("001122 9000"), nullptr);
    SvReloadRespPars svReloadRespPars(apduResponse, nullptr);

    ASSERT_EQ(svReloadRespPars.getSignatureLo(), ByteArrayUtil::fromHex("001122"));
}

TEST(SvReloadRespParsTest, getSignatureLo_mode_rev3_2)
{
    auto apduResponse = std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("001122334455 9000"),
                                                       nullptr);
    SvReloadRespPars svReloadRespPars(apduResponse, nullptr);

    ASSERT_EQ(svReloadRespPars.getSignatureLo(), ByteArrayUtil::fromHex("001122334455"));
}

TEST(SvReloadRespParsTest, getSignatureLo_bad_length)
{
    auto apduResponse = std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("0011 9000"), nullptr);

    EXPECT_THROW(SvReloadRespPars(apduResponse, nullptr), IllegalStateException);
}
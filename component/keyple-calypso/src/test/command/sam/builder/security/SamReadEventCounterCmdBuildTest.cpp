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

#include "SamReadEventCounterCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "SamReadEventCounterRespPars.h"

/* Common */
#include "IllegalArgumentException.h"
#include "stringhelper.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::common;
using namespace keyple::core::util;

using SamEventCounterOperationType =
    SamReadEventCounterCmdBuild::SamEventCounterOperationType;

static const std::string SW1SW2_OK = "9000";
static const int INDEX_M1 = -1;
static const int INDEX1 = 1;
static const int INDEX4 = 4;
static const int INDEX27 = 27;
static const std::string APDU_CLA_80_SINGLE = "80BE008100";
static const std::string APDU_CLA_80_RECORD = "80BE00E100";
static const std::string APDU_CLA_94 = "94BE008100";

TEST(SamReadEventCounterCmdBuildTest,
     samReadEventCounterCmdBuild_defaultRevision_createParser)
{
    SamReadEventCounterCmdBuild samReadEventCounterCmdBuild(
            SamRevision::NO_REV,
            SamEventCounterOperationType::SINGLE_COUNTER,
            INDEX1);

    ASSERT_EQ(samReadEventCounterCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_SINGLE));

    std::shared_ptr<SamReadEventCounterRespPars> samReadEventCounterRespPars =
        samReadEventCounterCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_OK),
                                                                  nullptr));

    ASSERT_EQ(typeid(*samReadEventCounterRespPars.get()),
              typeid(SamReadEventCounterRespPars));
}

TEST(SamReadEventCounterCmdBuildTest, samReadEventCounterCmdBuild_cla94)
{
    SamReadEventCounterCmdBuild samReadEventCounterCmdBuild(
            SamRevision::S1D,
            SamEventCounterOperationType::SINGLE_COUNTER,
            INDEX1);

    ASSERT_EQ(samReadEventCounterCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_94));
}

TEST(SamReadEventCounterCmdBuildTest, samReadEventCounterCmdBuild_cla80_single)
{
    SamReadEventCounterCmdBuild samReadEventCounterCmdBuild(
            SamRevision::C1,
            SamEventCounterOperationType::SINGLE_COUNTER,
            INDEX1);

    ASSERT_EQ(samReadEventCounterCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_SINGLE));
}

TEST(SamReadEventCounterCmdBuildTest, samReadEventCounterCmdBuild_cla80_record)
{
    SamReadEventCounterCmdBuild samReadEventCounterCmdBuild(
            SamRevision::C1,
            SamEventCounterOperationType::COUNTER_RECORD,
            INDEX1);

    ASSERT_EQ(samReadEventCounterCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_RECORD));
}

TEST(SamReadEventCounterCmdBuildTest,
     samReadEventCounterCmdBuild_cla80_single_indexOutOfBound_1)
{
    EXPECT_THROW(
        std::make_shared<SamReadEventCounterCmdBuild>(
            SamRevision::C1,
            SamEventCounterOperationType::SINGLE_COUNTER,
            INDEX_M1),
        IllegalArgumentException);
}

TEST(SamReadEventCounterCmdBuildTest,
     samReadEventCounterCmdBuild_cla80_single_indexOutOfBound_2)
{
    EXPECT_THROW(
        std::make_shared<SamReadEventCounterCmdBuild>(
            SamRevision::C1,
            SamEventCounterOperationType::SINGLE_COUNTER,
            INDEX27),
        IllegalArgumentException);
}

TEST(SamReadEventCounterCmdBuildTest,
     samReadEventCounterCmdBuild_cla80_record_indexOutOfBound_1)
{
    EXPECT_THROW(
        std::make_shared<SamReadEventCounterCmdBuild>(
            SamRevision::C1,
            SamEventCounterOperationType::COUNTER_RECORD,
            INDEX_M1),
        IllegalArgumentException);
}

TEST(SamReadEventCounterCmdBuildTest,
     samReadEventCounterCmdBuild_cla80_record_indexOutOfBound_2)
{
    EXPECT_THROW(
        std::make_shared<SamReadEventCounterCmdBuild>(
            SamRevision::C1,
            SamEventCounterOperationType::COUNTER_RECORD,
            INDEX4),
        IllegalArgumentException);
}

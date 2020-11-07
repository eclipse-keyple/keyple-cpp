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

#include "SamReadCeilingsCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "SamReadCeilingsRespPars.h"

/* Common */
#include "IllegalArgumentException.h"
#include "stringhelper.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::util;

using CeilingsOperationType = SamReadCeilingsCmdBuild::CeilingsOperationType;

static const std::string SW1SW2_OK = "9000";
static const int INDEX_M1 = -1;
static const int INDEX1 = 1;
static const int INDEX4 = 4;
static const int INDEX27 = 27;
static const std::string APDU_CLA_80_SINGLE = "80BE01B800";
static const std::string APDU_CLA_80_RECORD = "80BE00B100";
static const std::string APDU_CLA_94 = "94BE01B800";

TEST(SamReadCeilingsCmdBuildTest,
     samReadCeilingsCmdBuild_defaultRevision_createParser)
{
    SamReadCeilingsCmdBuild samReadCeilingsCmdBuild(
        SamRevision::NO_REV,
        CeilingsOperationType::SINGLE_CEILING,
        INDEX1);

    ASSERT_EQ(samReadCeilingsCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_SINGLE));

    std::shared_ptr<SamReadCeilingsRespPars> samReadCeilingsRespPars =
        samReadCeilingsCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_OK),
                                           nullptr));

    auto& instance = *samReadCeilingsRespPars.get();

    ASSERT_EQ(typeid(instance), typeid(SamReadCeilingsRespPars));
}

TEST(SamReadCeilingsCmdBuildTest, samReadCeilingsCmdBuild_cla94)
{
    SamReadCeilingsCmdBuild samReadCeilingsCmdBuild(
        SamRevision::S1D,
        CeilingsOperationType::SINGLE_CEILING,
        INDEX1);

    ASSERT_EQ(samReadCeilingsCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_94));
}

TEST(SamReadCeilingsCmdBuildTest, samReadCeilingsCmdBuild_cla80_single)
{
    SamReadCeilingsCmdBuild samReadCeilingsCmdBuild(
        SamRevision::C1,
        CeilingsOperationType::SINGLE_CEILING,
        INDEX1);

    ASSERT_EQ(samReadCeilingsCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_SINGLE));
}

TEST(SamReadCeilingsCmdBuildTest, samReadCeilingsCmdBuild_cla80_record)
{
    SamReadCeilingsCmdBuild samReadCeilingsCmdBuild(
        SamRevision::C1,
        CeilingsOperationType::CEILING_RECORD,
        INDEX1);

    ASSERT_EQ(samReadCeilingsCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_RECORD));
}

TEST(SamReadCeilingsCmdBuildTest,
     samReadCeilingsCmdBuild_cla80_single_indexOutOfBound_1)
{
    EXPECT_THROW(
        std::make_shared<SamReadCeilingsCmdBuild>(
            SamRevision::C1,
            CeilingsOperationType::CEILING_RECORD, INDEX_M1),
        IllegalArgumentException);
}

TEST(SamReadCeilingsCmdBuildTest,
     samReadCeilingsCmdBuild_cla80_single_indexOutOfBound_2)
{
    EXPECT_THROW(
        std::make_shared<SamReadCeilingsCmdBuild>(
            SamRevision::C1,
            CeilingsOperationType::CEILING_RECORD,
            INDEX27),
        IllegalArgumentException);
}

TEST(SamReadCeilingsCmdBuildTest,
     samReadCeilingsCmdBuild_cla80_record_indexOutOfBound_1)
{
    EXPECT_THROW(
        std::make_shared<SamReadCeilingsCmdBuild>(
            SamRevision::C1,
            CeilingsOperationType::CEILING_RECORD,
            INDEX_M1),
        IllegalArgumentException);
}

TEST(SamReadCeilingsCmdBuildTest,
     samReadCeilingsCmdBuild_cla80_record_indexOutOfBound_2)
{
    EXPECT_THROW(
        std::make_shared<SamReadCeilingsCmdBuild>(
            SamRevision::C1,
            CeilingsOperationType::CEILING_RECORD,
            INDEX4),
        IllegalArgumentException);
}

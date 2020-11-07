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

#include "UnlockCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "UnlockRespPars.h"

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

static const std::string SW1SW2_OK = "9000";
static const std::string UNLOCK_DATA_8 = "12345678FEDCBA98";
static const std::string UNLOCK_DATA_16 = "12345678FEDCBA9889ABCDEF87654321";
static const std::string UNLOCK_DATA_BAD_LENGTH = "12345678FEDCBA";
static const std::string APDU_CLA_80 = "8020000008" + UNLOCK_DATA_8;
static const std::string APDU_CLA_80_16 = "8020000010" + UNLOCK_DATA_16;
static const std::string APDU_CLA_94 = "9420000008" + UNLOCK_DATA_8;

TEST(UnlockCmdBuildTest, unlockCmdBuild_defaultRevision_createParser)
{
    UnlockCmdBuild unlockCmdBuild(SamRevision::NO_REV,
                                  ByteArrayUtil::fromHex(UNLOCK_DATA_8));

    ASSERT_EQ(unlockCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));

    std::shared_ptr<UnlockRespPars> unlockRespPars =
        unlockCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_OK),
                                           nullptr));

    auto& instance = *unlockRespPars.get();

    ASSERT_EQ(typeid(instance), typeid(UnlockRespPars));
}

TEST(UnlockCmdBuildTest, unlockCmdBuild_cla94)
{
    UnlockCmdBuild unlockCmdBuild(SamRevision::S1D,
                                  ByteArrayUtil::fromHex(UNLOCK_DATA_8));

    ASSERT_EQ(unlockCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_94));
}

TEST(UnlockCmdBuildTest, unlockCmdBuild_cla80)
{
    UnlockCmdBuild unlockCmdBuild(SamRevision::C1,
                                  ByteArrayUtil::fromHex(UNLOCK_DATA_8));

    ASSERT_EQ(unlockCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));
}

TEST(UnlockCmdBuildTest, unlockCmdBuild_cla80_16)
{
    UnlockCmdBuild unlockCmdBuild(SamRevision::C1,
                                  ByteArrayUtil::fromHex(UNLOCK_DATA_16));

    ASSERT_EQ(unlockCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_16));
}

TEST(UnlockCmdBuildTest, unlockCmdBuild_nullptrDiversifier)
{
    EXPECT_THROW(
        std::make_shared<UnlockCmdBuild>(SamRevision::NO_REV,
                                         std::vector<uint8_t>{}),
        IllegalArgumentException);
}

TEST(UnlockCmdBuildTest, unlockCmdBuild_badDiversifierLength)
{
    EXPECT_THROW(
        std::make_shared<UnlockCmdBuild>(
            SamRevision::NO_REV,
            ByteArrayUtil::fromHex(UNLOCK_DATA_BAD_LENGTH)),
        IllegalArgumentException);
}

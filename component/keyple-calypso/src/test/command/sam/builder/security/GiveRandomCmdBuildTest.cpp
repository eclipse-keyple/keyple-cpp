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

#include "GiveRandomCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "GiveRandomRespPars.h"

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
static const std::string PO_CHALLENGE = "FEDCBA9889ABCDEF";
static const std::string PO_CHALLENGE_BAD_LENGTH = "FEDC";
static const std::string APDU_CLA_80 = "8086000008" + PO_CHALLENGE;
static const std::string APDU_CLA_94 = "9486000008" + PO_CHALLENGE;

TEST(GiveRandomCmdBuildTest,
     giveRandomCmdBuild_defaultRevision_createParser)
{
    GiveRandomCmdBuild giveRandomCmdBuild(
        SamRevision::NO_REV,
        ByteArrayUtil::fromHex(PO_CHALLENGE));

    ASSERT_EQ(giveRandomCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));

    std::shared_ptr<GiveRandomRespPars> giveRandomRespPars =
        giveRandomCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(
                ByteArrayUtil::fromHex(SW1SW2_OK), nullptr));

    const auto& instance = *giveRandomRespPars.get();

    ASSERT_EQ(typeid(instance), typeid(GiveRandomRespPars));
}

TEST(GiveRandomCmdBuildTest, giveRandomCmdBuild_cla94)
{
    GiveRandomCmdBuild giveRandomCmdBuild(
        SamRevision::S1D,
        ByteArrayUtil::fromHex(PO_CHALLENGE));

    ASSERT_EQ(giveRandomCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_94));
}

TEST(GiveRandomCmdBuildTest, giveRandomCmdBuild_cla80)
{
    GiveRandomCmdBuild giveRandomCmdBuild(
        SamRevision::C1,
        ByteArrayUtil::fromHex(PO_CHALLENGE));

    ASSERT_EQ(giveRandomCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));
}

TEST(GiveRandomCmdBuildTest, giveRandomCmdBuild_nullSignature)
{
    EXPECT_THROW(
        std::make_shared<GiveRandomCmdBuild>(SamRevision::NO_REV,
                                             std::vector<uint8_t>{}),
        IllegalArgumentException);
}

TEST(GiveRandomCmdBuildTest, giveRandomCmdBuild_badSignatureLength)
{
    EXPECT_THROW(
        std::make_shared<GiveRandomCmdBuild>(
            SamRevision::NO_REV,
            ByteArrayUtil::fromHex(PO_CHALLENGE_BAD_LENGTH)),
        IllegalArgumentException);
}

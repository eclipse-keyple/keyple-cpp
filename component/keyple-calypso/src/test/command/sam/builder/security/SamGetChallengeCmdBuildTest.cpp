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

#include "SamGetChallengeCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "SamGetChallengeRespPars.h"

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

static const std::string SW1SW2_OK = "9000";
static const uint8_t LENGTH_4 = 4;
static const uint8_t LENGTH_8 = 8;
static const std::string APDU_CLA_80_L4 = "8084000004";
static const std::string APDU_CLA_80_L8 = "8084000008";
static const std::string APDU_CLA_94 = "9484000004";

TEST(SamGetChallengeCmdBuildTest,
    samGetChallengeCmdBuild_defaultRevision_createParser)
{
    SamGetChallengeCmdBuild samGetChallengeCmdBuild(SamRevision::NO_REV,
                                                    LENGTH_4);

    ASSERT_EQ(samGetChallengeCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_L4));

    std::shared_ptr<SamGetChallengeRespPars> samGetChallengeRespPars =
        samGetChallengeCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(
                ByteArrayUtil::fromHex(SW1SW2_OK), nullptr));

    auto& instance = *samGetChallengeRespPars.get();

    ASSERT_EQ(typeid(instance), typeid(SamGetChallengeRespPars));
}

TEST(SamGetChallengeCmdBuildTest, samGetChallengeCmdBuild_cla94)
{
    SamGetChallengeCmdBuild samGetChallengeCmdBuild(SamRevision::S1D, LENGTH_4);

    ASSERT_EQ(samGetChallengeCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_94));
}

TEST(SamGetChallengeCmdBuildTest, samGetChallengeCmdBuild_length8)
{
    SamGetChallengeCmdBuild samGetChallengeCmdBuild(SamRevision::C1, LENGTH_8);

    ASSERT_EQ(samGetChallengeCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_L8));
}

TEST(SamGetChallengeCmdBuildTest, samGetChallengeCmdBuild_badLength)
{
    EXPECT_THROW(
        std::make_shared<SamGetChallengeCmdBuild>(SamRevision::NO_REV,
                                                  LENGTH_4 + 1),
        IllegalArgumentException);
}

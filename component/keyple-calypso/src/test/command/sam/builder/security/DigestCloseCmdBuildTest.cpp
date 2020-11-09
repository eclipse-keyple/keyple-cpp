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

#include "DigestCloseCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "DigestCloseRespPars.h"

/* Common */
#include "IllegalArgumentException.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::common::exception;
using namespace keyple::core::util;

static const std::string SW1SW2_OK = "9000";
static const uint8_t LENGTH_4 = 0x04;
static const std::string APDU_CLA_80 = "808E000004";
static const std::string APDU_CLA_94 = "948E000004";

TEST(DigestCloseCmdBuild, digestCloseCmdBuild_defaultRevision_createParser)
{
    DigestCloseCmdBuild digestCloseCmdBuild(SamRevision::NO_REV, LENGTH_4);

    ASSERT_EQ(digestCloseCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));

    std::shared_ptr<DigestCloseRespPars> digestCloseRespPars =
        digestCloseCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(
                ByteArrayUtil::fromHex(SW1SW2_OK), nullptr));

    auto& instance = *digestCloseRespPars.get();

    ASSERT_EQ(typeid(instance), typeid(DigestCloseRespPars));
}

TEST(DigestCloseCmdBuild, digestCloseCmdBuild_cla94)
{
    DigestCloseCmdBuild digestCloseCmdBuild(SamRevision::S1D, LENGTH_4);

    ASSERT_EQ(digestCloseCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_94));
}

TEST(DigestCloseCmdBuild, digestCloseCmdBuild_cla80)
{
    DigestCloseCmdBuild digestCloseCmdBuild(SamRevision::C1, LENGTH_4);

    ASSERT_EQ(digestCloseCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));
}

TEST(DigestCloseCmdBuild, digestCloseCmdBuild_badExpectedLength)
{
    EXPECT_THROW(std::make_shared<DigestCloseCmdBuild>(SamRevision::NO_REV,
                                                       static_cast<uint8_t>(LENGTH_4 + 1)),
                 IllegalArgumentException);
}
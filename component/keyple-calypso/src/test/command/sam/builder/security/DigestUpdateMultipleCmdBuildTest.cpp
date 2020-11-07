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

#include "DigestUpdateMultipleCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "DigestUpdateMultipleRespPars.h"

/* Common */
#include "IllegalArgumentException.h"
#include "stringhelper.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::common::exception;
using namespace keyple::core::util;

static const bool ENCRYPTED_SESSION_TRUE = true;
static const bool ENCRYPTED_SESSION_FALSE = false;
static const std::string DIGEST_DATA = "112233445566778899AA";
static const std::string SW1SW2_OK = "9000";
//static const uint8_t LENGTH_4 = 0x04;
static const std::string APDU_CLA_80 = "808C00000A" + DIGEST_DATA;
static const std::string APDU_CLA_80_ENCRYPTED_SESSION = "808C00800A" +
                                                         DIGEST_DATA;
static const std::string APDU_CLA_94 = "948C00000A" + DIGEST_DATA;

TEST(DigestUpdateMultipleCmdBuildTest,
     digestUpdateMultipleCmd_defaultRevision_createParser)
{
    DigestUpdateMultipleCmdBuild digestUpdateMultipleCmd(
        SamRevision::NO_REV,
        ENCRYPTED_SESSION_FALSE,
        ByteArrayUtil::fromHex(DIGEST_DATA));

    ASSERT_EQ(digestUpdateMultipleCmd.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));

    std::shared_ptr<DigestUpdateMultipleRespPars> digestUpdateMultipleRespPars =
        digestUpdateMultipleCmd.createResponseParser(
            std::make_shared<ApduResponse>(
                ByteArrayUtil::fromHex(SW1SW2_OK), nullptr));

    auto& instance = *digestUpdateMultipleRespPars.get();

    ASSERT_EQ(typeid(instance), typeid(DigestUpdateMultipleRespPars));
}

TEST(DigestUpdateMultipleCmdBuildTest, digestUpdateMultipleCmd_cla94)
{
    DigestUpdateMultipleCmdBuild digestUpdateMultipleCmd(
        SamRevision::S1D,
        ENCRYPTED_SESSION_FALSE,
        ByteArrayUtil::fromHex(DIGEST_DATA));

    ASSERT_EQ(digestUpdateMultipleCmd.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_94));
}

TEST(DigestUpdateMultipleCmdBuildTest, digestUpdateMultipleCmd_cla80)
{
    DigestUpdateMultipleCmdBuild digestUpdateMultipleCmd(
        SamRevision::C1,
        ENCRYPTED_SESSION_FALSE,
        ByteArrayUtil::fromHex(DIGEST_DATA));

    ASSERT_EQ(digestUpdateMultipleCmd.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));
}

TEST(DigestUpdateMultipleCmdBuildTest,
     digestUpdateMultipleCmd_cla80_encryptedSession)
{
    DigestUpdateMultipleCmdBuild digestUpdateMultipleCmd(
        SamRevision::C1,
        ENCRYPTED_SESSION_TRUE,
        ByteArrayUtil::fromHex(DIGEST_DATA));

    ASSERT_EQ(digestUpdateMultipleCmd.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_ENCRYPTED_SESSION));
}

TEST(DigestUpdateMultipleCmdBuildTest, digestUpdateMultipleCmd_digestDataNull)
{
    EXPECT_THROW(
        std::make_shared<DigestUpdateMultipleCmdBuild>(SamRevision::NO_REV,
                                               ENCRYPTED_SESSION_FALSE,
                                               std::vector<uint8_t>{}),
        IllegalArgumentException);
}

TEST(DigestUpdateMultipleCmdBuildTest, digestUpdateMultipleCmd_badDigestDataLength)
{
    /* Create digest data > 255 bytes */
    std::string digestData = "";
    while (digestData.size() < (255 * 2))
        digestData = digestData + DIGEST_DATA;

    EXPECT_THROW(
        std::make_shared<DigestUpdateMultipleCmdBuild>(
            SamRevision::NO_REV,
            ENCRYPTED_SESSION_FALSE,
            ByteArrayUtil::fromHex(digestData)),
        IllegalArgumentException);
}

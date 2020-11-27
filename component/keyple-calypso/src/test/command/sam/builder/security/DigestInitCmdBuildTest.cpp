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

#include "DigestInitCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "DigestInitRespPars.h"

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

static const uint8_t KIF_OK = 0x12;
static const uint8_t KVC_OK = 0x34;
static const uint8_t KEY_REC_NBR_OK = 0x56;
static const uint8_t KIF_ZERO = 0x00;
static const uint8_t KVC_ZERO = 0x00;
static const uint8_t KEY_REC_NBR_ZERO = 0x00;
static const uint8_t KIF_FF = 0xFF;

static const bool VERIFICATION_MODE_TRUE = true;
static const bool VERIFICATION_MODE_FALSE = false;
static const bool CONFIDENTIAL_SESSION_MODE_TRUE = true;
static const bool CONFIDENTIAL_SESSION_MODE_FALSE = false;

static const std::string DIGEST_DATA = "112233445566778899AA";
static const std::string APDU_CLA_80_KIF_OK =
    "808A00FF0C" +
    StringHelper::uint8ToHexString(KIF_OK) +
    StringHelper::uint8ToHexString(KVC_OK) +
    DIGEST_DATA;
static const std::string APDU_CLA_80_VERIF_MODE =
    "808A01FF0C" +
    StringHelper::uint8ToHexString(KIF_OK) +
    StringHelper::uint8ToHexString(KVC_OK) +
    DIGEST_DATA;
static const std::string APDU_CLA_80_CONFIDENTIAL_SESSION_MODE =
    "808A02FF0C" +
    StringHelper::uint8ToHexString(KIF_OK) +
    StringHelper::uint8ToHexString(KVC_OK) +
    DIGEST_DATA;
static const std::string APDU_CLA_80_KIF_FF =
    "808A00" +
    StringHelper::uint8ToHexString(KEY_REC_NBR_OK) +
    "0A" +
    DIGEST_DATA;
static const std::string APDU_CLA_94_KIF_OK =
    "948A00FF0C" +
    StringHelper::uint8ToHexString(KIF_OK) +
    StringHelper::uint8ToHexString(KVC_OK) +
    DIGEST_DATA;

TEST(DigestInitCmdBuildTest,
     digestInitCmdBuild_defaultRevision_nominal_createParser)
{
    DigestInitCmdBuild digestInitCmdBuild(SamRevision::NO_REV,
                                          VERIFICATION_MODE_FALSE,
                                          CONFIDENTIAL_SESSION_MODE_FALSE,
                                          KEY_REC_NBR_OK,
                                          KIF_OK,
                                          KVC_OK,
                                          ByteArrayUtil::fromHex(DIGEST_DATA));

    ASSERT_EQ(digestInitCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_KIF_OK));

    std::shared_ptr<DigestInitRespPars> digestInitRespPars =
        digestInitCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(
                ByteArrayUtil::fromHex(SW1SW2_OK), nullptr));

    const auto& instance = *digestInitRespPars.get();

    ASSERT_EQ(typeid(instance), typeid(DigestInitRespPars));
}

TEST(DigestInitCmdBuildTest, digestInitCmdBuild_kifFF)
{
    DigestInitCmdBuild digestInitCmdBuild(SamRevision::C1,
                                          VERIFICATION_MODE_FALSE,
                                          CONFIDENTIAL_SESSION_MODE_FALSE,
                                          KEY_REC_NBR_OK,
                                          KIF_FF,
                                          KVC_OK,
                                          ByteArrayUtil::fromHex(DIGEST_DATA));

    ASSERT_EQ(digestInitCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_KIF_FF));
}

TEST(DigestInitCmdBuildTest, digestInitCmdBuild_keyRec00_kif00)
{
    EXPECT_THROW(
        std::make_shared<DigestInitCmdBuild>(
            SamRevision::C1,
            VERIFICATION_MODE_FALSE,
            CONFIDENTIAL_SESSION_MODE_FALSE,
            KEY_REC_NBR_ZERO,
            KIF_ZERO,
            KVC_OK,
            ByteArrayUtil::fromHex(DIGEST_DATA)),
        IllegalArgumentException);
}

TEST(DigestInitCmdBuildTest, digestInitCmdBuild_keyRec00_kvc00)
{
    EXPECT_THROW(
        std::make_shared<DigestInitCmdBuild>(
            SamRevision::C1,
            VERIFICATION_MODE_FALSE,
            CONFIDENTIAL_SESSION_MODE_FALSE,
            KEY_REC_NBR_ZERO,
            KIF_OK,
            KVC_ZERO,
            ByteArrayUtil::fromHex(DIGEST_DATA)),
        IllegalArgumentException);
}

TEST(DigestInitCmdBuildTest, digestInitCmdBuild_digestDataNull)
{
    EXPECT_THROW(
        std::make_shared<DigestInitCmdBuild>(SamRevision::C1,
                                             VERIFICATION_MODE_FALSE,
                                             CONFIDENTIAL_SESSION_MODE_FALSE,
                                             KEY_REC_NBR_OK,
                                             KIF_OK,
                                             KVC_OK,
                                             std::vector<uint8_t>{}),
        IllegalArgumentException);
}

TEST(DigestInitCmdBuildTest, digestInitCmdBuild_cla94)
{
    DigestInitCmdBuild digestInitCmdBuild(SamRevision::S1D,
                                          VERIFICATION_MODE_FALSE,
                                          CONFIDENTIAL_SESSION_MODE_FALSE,
                                          KEY_REC_NBR_OK,
                                          KIF_OK,
                                          KVC_OK,
                                          ByteArrayUtil::fromHex(DIGEST_DATA));

    ASSERT_EQ(digestInitCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_94_KIF_OK));
}

TEST(DigestInitCmdBuildTest, digestInitCmdBuild_verificationMode)
{
    DigestInitCmdBuild digestInitCmdBuild(SamRevision::C1,
                                          VERIFICATION_MODE_TRUE,
                                          CONFIDENTIAL_SESSION_MODE_FALSE,
                                          KEY_REC_NBR_OK,
                                          KIF_OK,
                                          KVC_OK,
                                          ByteArrayUtil::fromHex(DIGEST_DATA));

    //std::shared_ptr<ApduRequest> ApduRequest = digestInitCmdBuild.getApduRequest();

    ASSERT_EQ(digestInitCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_VERIF_MODE));
}

TEST(DigestInitCmdBuildTest, digestInitCmdBuild_confidentialSessionMode)
{
    DigestInitCmdBuild digestInitCmdBuild(SamRevision::C1,
                                          VERIFICATION_MODE_FALSE,
                                          CONFIDENTIAL_SESSION_MODE_TRUE,
                                          KEY_REC_NBR_OK,
                                          KIF_OK,
                                          KVC_OK,
                                          ByteArrayUtil::fromHex(DIGEST_DATA));

    //std::shared_ptr<ApduRequest> ApduRequest = digestInitCmdBuild.getApduRequest();

    ASSERT_EQ(digestInitCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_CONFIDENTIAL_SESSION_MODE));
}

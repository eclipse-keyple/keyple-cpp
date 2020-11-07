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

#include "SamWriteKeyCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "SamWriteKeyRespPars.h"

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
static const uint8_t WRITING_MODE = 0x12;
static const uint8_t KEY_REF = 0x34;
static const std::string KEY_DATA15 = "00112233445566778899AABBCCDDEE";
static const std::string KEY_DATA16 = "00112233445566778899AABBCCDDEEFF";
static const std::string KEY_DATA17 = "00112233445566778899AABBCCDDEEFF00";
static const std::string KEY_DATA = KEY_DATA16 + KEY_DATA16 + KEY_DATA16;
static const std::string KEY_DATA_BAD_LENGTH1 = KEY_DATA16 +
                                                KEY_DATA16 +
                                                KEY_DATA15;
static const std::string KEY_DATA_BAD_LENGTH2 = KEY_DATA16 +
                                                KEY_DATA16 +
                                                KEY_DATA16 +
                                                KEY_DATA16 +
                                                KEY_DATA17;
static const std::string APDU_CLA_80 = "801A123430" + KEY_DATA;
static const std::string APDU_CLA_94 = "941A123430" + KEY_DATA;

TEST(SamWriteKeyCmdBuildTest, samWriteKeyCmdBuild_defaultRevision_createParser)
{
    SamWriteKeyCmdBuild samWriteKeyCmdBuild(SamRevision::NO_REV,
                                            WRITING_MODE,
                                            KEY_REF,
                                            ByteArrayUtil::fromHex(KEY_DATA));

    ASSERT_EQ(samWriteKeyCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));

    std::shared_ptr<SamWriteKeyRespPars> samWriteKeyRespPars =
        samWriteKeyCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_OK),
                                           nullptr));

    auto& instance = *samWriteKeyRespPars.get();

    ASSERT_EQ(typeid(instance), typeid(SamWriteKeyRespPars));
}

TEST(SamWriteKeyCmdBuildTest, samWriteKeyCmdBuild_cla94)
{
    SamWriteKeyCmdBuild samWriteKeyCmdBuild(SamRevision::S1D,
                                            WRITING_MODE,
                                            KEY_REF,
                                            ByteArrayUtil::fromHex(KEY_DATA));

    ASSERT_EQ(samWriteKeyCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_94));
}

TEST(SamWriteKeyCmdBuildTest, samWriteKeyCmdBuild_cla80)
{
    SamWriteKeyCmdBuild samWriteKeyCmdBuild(SamRevision::C1,
                                            WRITING_MODE,
                                            KEY_REF,
                                            ByteArrayUtil::fromHex(KEY_DATA));

    ASSERT_EQ(samWriteKeyCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));
}

TEST(SamWriteKeyCmdBuildTest, samWriteKeyCmdBuild_nullptrSignature)
{
    EXPECT_THROW(
        std::make_shared<SamWriteKeyCmdBuild>(SamRevision::NO_REV,
                                              WRITING_MODE,
                                              KEY_REF,
                                              std::vector<uint8_t>{}),
        IllegalArgumentException);
}

TEST(SamWriteKeyCmdBuildTest, samWriteKeyCmdBuild_badSignatureLength_1)
{
    EXPECT_THROW(
        std::make_shared<SamWriteKeyCmdBuild>(
            SamRevision::NO_REV,
            WRITING_MODE,
            KEY_REF,
            ByteArrayUtil::fromHex(KEY_DATA_BAD_LENGTH1)),
        IllegalArgumentException);
}

TEST(SamWriteKeyCmdBuildTest, samWriteKeyCmdBuild_badSignatureLength_2)
{
    EXPECT_THROW(
        std::make_shared<SamWriteKeyCmdBuild>(
            SamRevision::NO_REV,
            WRITING_MODE,
            KEY_REF,
            ByteArrayUtil::fromHex(KEY_DATA_BAD_LENGTH2)),
        IllegalArgumentException);
}

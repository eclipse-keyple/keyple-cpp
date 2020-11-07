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

#include "CardGenerateKeyCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "CardGenerateKeyRespPars.h"
#include "SamRevision.h"

/* Common */
#include "IllegalArgumentException.h"
#include "stringhelper.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::util;

static const uint8_t KIF_SRC = 0x12;
static const uint8_t KVC_SRC = 0x34;
static const uint8_t KIF_CIPH_KEY = 0x56;
static const uint8_t KVC_CIPH_KEY = 0x78;

static const std::shared_ptr<KeyReference> KEY_REFERENCE_SRC =
    std::make_shared<KeyReference>(KIF_SRC, KVC_SRC);
static const std::shared_ptr<KeyReference> KEY_REFERENCE_CIPH_KEY =
    std::make_shared<KeyReference>(KIF_CIPH_KEY, KVC_CIPH_KEY);

static const std::string SW1SW2_OK = "9000";
static const std::string APDU_CLA_80 =
    "8012FFFF05" +
    StringHelper::uint8ToHexString(KIF_CIPH_KEY) +
    StringHelper::uint8ToHexString(KVC_CIPH_KEY) +
    StringHelper::uint8ToHexString(KIF_SRC) +
    StringHelper::uint8ToHexString(KVC_SRC) +
    "90";
static const std::string APDU_CLA_94 =
    "9412FFFF05" +
    StringHelper::uint8ToHexString(KIF_CIPH_KEY) +
    StringHelper::uint8ToHexString(KVC_CIPH_KEY) +
    StringHelper::uint8ToHexString(KIF_SRC) +
    StringHelper::uint8ToHexString(KVC_SRC) +
    "90";
static const std::string APDU_CLA_80_CIPHERING_KEY_NULL =
    "8012FF0003" +
    StringHelper::uint8ToHexString(KIF_SRC) +
    StringHelper::uint8ToHexString(KVC_SRC) +
    "90";

TEST(CardGenerateKeyCmdBuildTest,
     cardGenerateKeyCmdBuild_defaultRevision_createParser)
{
    CardGenerateKeyCmdBuild cardGenerateKeyCmdBuild(SamRevision::NO_REV,
                                                    KEY_REFERENCE_CIPH_KEY,
                                                    KEY_REFERENCE_SRC);

    ASSERT_EQ(cardGenerateKeyCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));

    std::shared_ptr<CardGenerateKeyRespPars> cardGenerateKeyRespPars =
        cardGenerateKeyCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_OK),
                                           nullptr));

    auto& instance = *cardGenerateKeyRespPars.get();

    ASSERT_EQ(typeid(instance), typeid(CardGenerateKeyRespPars));
}

TEST(CardGenerateKeyCmdBuildTest, cardGenerateKeyCmdBuild_cla94)
{
    CardGenerateKeyCmdBuild cardGenerateKeyCmdBuild(SamRevision::S1D,
                                                    KEY_REFERENCE_CIPH_KEY,
                                                    KEY_REFERENCE_SRC);

    ASSERT_EQ(cardGenerateKeyCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_94));
}

TEST(CardGenerateKeyCmdBuildTest, cardGenerateKeyCmdBuild_cla80)
{
    CardGenerateKeyCmdBuild cardGenerateKeyCmdBuild(SamRevision::C1,
                                                    KEY_REFERENCE_CIPH_KEY,
                                                    KEY_REFERENCE_SRC);

    ASSERT_EQ(cardGenerateKeyCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));
}


TEST(CardGenerateKeyCmdBuildTest, cardGenerateKeyCmdBuild_cipheringKeyNull)
{
    CardGenerateKeyCmdBuild cardGenerateKeyCmdBuild(SamRevision::C1,
                                                    nullptr,
                                                    KEY_REFERENCE_SRC);

    ASSERT_EQ(cardGenerateKeyCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80_CIPHERING_KEY_NULL));
}

TEST(CardGenerateKeyCmdBuildTest, cardGenerateKeyCmdBuild_sourceKeyNull)
{
    EXPECT_THROW(
        std::make_shared<CardGenerateKeyCmdBuild>(SamRevision::C1,
                                                  KEY_REFERENCE_CIPH_KEY,
                                                  nullptr),
        IllegalArgumentException);
}

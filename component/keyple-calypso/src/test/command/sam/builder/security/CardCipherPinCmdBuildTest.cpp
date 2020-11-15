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

#include "CardCipherPinCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "CardCipherPinRespPars.h"
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

static const std::string SW1SW2_OK_STR = "9000";
static const std::string KIF_CIPH_KEY_STR = "56";
static const std::string KVC_CIPH_KEY_STR = "78";
static const std::string CURRENT_PIN_STR = "31323334";
static const std::string NEW_PIN_STR = "35363738";
static const std::string PIN_LENGTH_5_STR = "3132333435";

static const std::vector<uint8_t> SW1SW2_OK = ByteArrayUtil::fromHex(SW1SW2_OK_STR);
static const uint8_t KIF_CIPH_KEY = ByteArrayUtil::fromHex(KIF_CIPH_KEY_STR)[0];
static const uint8_t KVC_CIPH_KEY = ByteArrayUtil::fromHex(KVC_CIPH_KEY_STR)[0];
static const std::shared_ptr<KeyReference> KEY_REFERENCE_CIPH_KEY =
    std::make_shared<KeyReference>(KIF_CIPH_KEY, KVC_CIPH_KEY);
static const std::vector<uint8_t> CURRENT_PIN = ByteArrayUtil::fromHex(CURRENT_PIN_STR);
static const std::vector<uint8_t> NEW_PIN = ByteArrayUtil::fromHex(NEW_PIN_STR);
static const std::vector<uint8_t> PIN_LENGTH_5 = ByteArrayUtil::fromHex(PIN_LENGTH_5_STR);
static const std::vector<uint8_t> APDU_ISO_CHANGE_PIN = ByteArrayUtil::fromHex("80 12 40FF 0A" +
                                                                               KIF_CIPH_KEY_STR +
                                                                               KVC_CIPH_KEY_STR +
                                                                               CURRENT_PIN_STR +
                                                                               NEW_PIN_STR);
static const std::vector<uint8_t> APDU_ISO_VERIFY_PIN =
    ByteArrayUtil::fromHex("80 12 80FF 06" + KIF_CIPH_KEY_STR + KVC_CIPH_KEY_STR + CURRENT_PIN_STR);

TEST(CardCipherPinCmdBuildTest, cardCipherPinCmdBuild_update_PIN)
{
    CardCipherPinCmdBuild builder(SamRevision::C1, KEY_REFERENCE_CIPH_KEY, CURRENT_PIN, NEW_PIN);
    const std::vector<uint8_t> apduRequestBytes = builder.getApduRequest()->getBytes();

    ASSERT_EQ(apduRequestBytes, APDU_ISO_CHANGE_PIN);
}

TEST(CardCipherPinCmdBuildTest, cardCipherPinCmdBuild_verify_PIN)
{
    CardCipherPinCmdBuild builder(SamRevision::C1,
                                  KEY_REFERENCE_CIPH_KEY,
                                  CURRENT_PIN,
                                  std::vector<uint8_t>{});
    const std::vector<uint8_t> apduRequestBytes = builder.getApduRequest()->getBytes();

    ASSERT_EQ(apduRequestBytes, APDU_ISO_VERIFY_PIN);
}

TEST(CardCipherPinCmdBuildTest, cardCipherPinCmdBuild_update_PIN_bad_length_1)
{
    EXPECT_THROW(
        CardCipherPinCmdBuild(SamRevision::C1, KEY_REFERENCE_CIPH_KEY, PIN_LENGTH_5, NEW_PIN),
        IllegalArgumentException);
}

TEST(CardCipherPinCmdBuildTest, cardCipherPinCmdBuild_update_PIN_bad_length_2)
{
    EXPECT_THROW(
        CardCipherPinCmdBuild(SamRevision::C1, KEY_REFERENCE_CIPH_KEY, CURRENT_PIN, PIN_LENGTH_5),
        IllegalArgumentException);
}

TEST(CardCipherPinCmdBuildTest, cardCipherPinCmdBuild_parser)
{
    CardCipherPinCmdBuild builder(SamRevision::C1, KEY_REFERENCE_CIPH_KEY, CURRENT_PIN, NEW_PIN);
    auto apduResponse = std::make_shared<ApduResponse>(SW1SW2_OK, nullptr);

    auto parser = *(builder.createResponseParser(apduResponse)).get();

    ASSERT_EQ(typeid(parser), typeid(CardCipherPinRespPars));
}

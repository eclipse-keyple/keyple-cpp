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

#include "VerifyPinCmdBuild.h"

/* Calypso */
#include "VerifyPinRespPars.h"

/* Core */
#include "ByteArrayUtil.h"

/* Common */
#include "IllegalArgumentException.h"

using namespace testing;

using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::common::exception;
using namespace keyple::core::util;

using PinTransmissionMode = PoTransaction::PinTransmissionMode;

static const std::string SW1SW2_OK_STR = "9000";
static const std::string PIN_DATA_STR = "31323334";
static const std::string CIPHERED_PIN_DATA_STR = "0011223344556677";
static const std::string PIN_DATA_LENGTH_5_STR = "3132333435";

static const std::vector<uint8_t> SW1SW2_OK = ByteArrayUtil::fromHex(SW1SW2_OK_STR);
static const std::vector<uint8_t> PIN_DATA = ByteArrayUtil::fromHex(PIN_DATA_STR);
static const std::vector<uint8_t> CIPHERED_PIN_DATA = ByteArrayUtil::fromHex(CIPHERED_PIN_DATA_STR);
static const std::vector<uint8_t> PIN_DATA_LENGTH_5 = ByteArrayUtil::fromHex(PIN_DATA_LENGTH_5_STR);

static const std::vector<uint8_t> APDU_ISO_PLAIN =
    ByteArrayUtil::fromHex("00 20 0000 04" + PIN_DATA_STR);
static const std::vector<uint8_t> APDU_ISO_ENCRYPTED =
    ByteArrayUtil::fromHex("00 20 0000 08" + CIPHERED_PIN_DATA_STR);
static const std::vector<uint8_t> APDU_ISO_READ_COUNTER = ByteArrayUtil::fromHex("00 20 0000 00");

TEST(VerifyPinCmdBuild, verifyPin_plain)
{
    VerifyPinCmdBuild builder(PoClass::ISO, PinTransmissionMode::PLAIN, PIN_DATA);
    const std::vector<uint8_t>& apduRequestBytes = builder.getApduRequest()->getBytes();

    ASSERT_EQ(apduRequestBytes, APDU_ISO_PLAIN);
}

TEST(VerifyPinCmdBuild, verifyPin_pin_null)
{
    EXPECT_THROW(
        VerifyPinCmdBuild(PoClass::ISO, PinTransmissionMode::PLAIN, std::vector<uint8_t>{}),
        IllegalArgumentException);
}

TEST(VerifyPinCmdBuild, verifyPin_pin_bad_length)
{
    EXPECT_THROW(VerifyPinCmdBuild(PoClass::ISO, PinTransmissionMode::PLAIN, PIN_DATA_LENGTH_5),
                 IllegalArgumentException);
}

TEST(VerifyPinCmdBuild, verifyPin_encrypted)
{
    VerifyPinCmdBuild builder(PoClass::ISO, PinTransmissionMode::ENCRYPTED, CIPHERED_PIN_DATA);
    const std::vector<uint8_t> apduRequestBytes = builder.getApduRequest()->getBytes();

    ASSERT_EQ(apduRequestBytes, APDU_ISO_ENCRYPTED);
}

TEST(VerifyPinCmdBuild, verifyPin_encrypted_pin_null)
{
    EXPECT_THROW(
        VerifyPinCmdBuild(PoClass::ISO, PinTransmissionMode::ENCRYPTED, std::vector<uint8_t>{}),
        IllegalArgumentException);
}

TEST(VerifyPinCmdBuild, verifyPin_encrypted_pin_bad_length)
{
    EXPECT_THROW(
        VerifyPinCmdBuild(PoClass::ISO, PinTransmissionMode::ENCRYPTED, PIN_DATA_LENGTH_5),
        IllegalArgumentException);
}

TEST(VerifyPinCmdBuild, verifyPin_read_presentation_counter)
{
    VerifyPinCmdBuild builder(PoClass::ISO);
    const std::vector<uint8_t> apduRequestBytes = builder.getApduRequest()->getBytes();

    ASSERT_EQ(apduRequestBytes, APDU_ISO_READ_COUNTER);
}

TEST(VerifyPinCmdBuild, verifyPin_various_tests)
{
    VerifyPinCmdBuild builder(PoClass::ISO, PinTransmissionMode::PLAIN, PIN_DATA);
    auto apduResponse = std::make_shared<ApduResponse>(SW1SW2_OK, nullptr);

    auto resp = *(builder.createResponseParser(apduResponse)).get();

    ASSERT_EQ(typeid(resp), typeid(VerifyPinRespPars));
    ASSERT_FALSE(builder.isSessionBufferUsed());
}
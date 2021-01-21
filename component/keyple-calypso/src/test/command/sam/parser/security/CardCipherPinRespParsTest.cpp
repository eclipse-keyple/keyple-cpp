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

#include "CardCipherPinRespPars.h"

/* Calypso */
#include "CalypsoSamCommandException.h"

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleSeCommandException.h"

using namespace testing;

using namespace keyple::calypso::command::sam::exception;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::core::command::exception;
using namespace keyple::core::util;

static const std::string SW1SW2_KO_STR = "6A82";
static const std::string SW1SW2_OK_STR = "9000";
static const std::string CIPHERED_DATA_STR = "1122334455667788";

static const std::vector<uint8_t> SW1SW2_KO = ByteArrayUtil::fromHex(SW1SW2_KO_STR);
static const std::vector<uint8_t> CIPHERED_DATA = ByteArrayUtil::fromHex(CIPHERED_DATA_STR);
static const std::vector<uint8_t> APDU_RESPONSE_OK =
    ByteArrayUtil::fromHex(CIPHERED_DATA_STR + SW1SW2_OK_STR);

TEST(CardCipherPinRespParsTest, cardCipherPinRespPars_goodStatus)
{
    CardCipherPinRespPars parser(std::make_shared<ApduResponse>(APDU_RESPONSE_OK, nullptr),nullptr);
    parser.checkStatus();

    ASSERT_EQ(parser.getCipheredData(), CIPHERED_DATA);
}

TEST(CardCipherPinRespParsTest, cardCipherPinRespPars_badStatus)
{
    CardCipherPinRespPars parser(std::make_shared<ApduResponse>(SW1SW2_KO, nullptr), nullptr);

    EXPECT_THROW(parser.checkStatus(), KeypleSeCommandException);
}
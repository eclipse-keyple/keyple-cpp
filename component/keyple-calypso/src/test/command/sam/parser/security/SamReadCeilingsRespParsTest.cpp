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

#include "SamReadCeilingsRespPars.h"

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleSeCommandException.h"

using namespace testing;

using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::core::command::exception;
using namespace keyple::core::util;

static const std::string SW1SW2_KO = "6988";
static const std::string SW1SW2_OK = "9000";
static const std::string CEILING_DATA = "11223344";
static const std::string APDU_READ_CEILING = CEILING_DATA + SW1SW2_OK;

TEST(SamReadCeilingsRespParsTest, badStatus)
{
    SamReadCeilingsRespPars samReadCeilingsRespPars(
        std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_KO),
                                       nullptr),
        nullptr);

    EXPECT_THROW(samReadCeilingsRespPars.checkStatus(),
                 KeypleSeCommandException);
}

TEST(SamReadCeilingsRespParsTest, goodStatus_getSignature)
{
    SamReadCeilingsRespPars samReadCeilingsRespPars(
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(APDU_READ_CEILING), nullptr),
        nullptr);

    samReadCeilingsRespPars.checkStatus();

    ASSERT_EQ(samReadCeilingsRespPars.getCeilingsData(),
              ByteArrayUtil::fromHex(CEILING_DATA));
}

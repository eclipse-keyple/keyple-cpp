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

#include "DigestCloseRespPars.h"

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleSeCommandException.h"

using namespace testing;

using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::core::command::exception;
using namespace keyple::core::util;

static const std::string SW1SW2_KO = "6988";
static const std::string SW1SW2_OK = "9000";
static const std::string SAM_SIGNATURE = "11223344";
static const std::string APDU_DIGEST_CLOSE = SAM_SIGNATURE + SW1SW2_OK;

TEST(DigestCloseRespParsTest, badStatus)
{
    DigestCloseRespPars digestCloseRespPars(
        std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_KO),
                                       nullptr),
        nullptr);

    EXPECT_THROW(digestCloseRespPars.checkStatus(),
                 KeypleSeCommandException);
}

TEST(DigestCloseRespParsTest, getSignature)
{
    DigestCloseRespPars digestCloseRespPars(
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(APDU_DIGEST_CLOSE), nullptr),
        nullptr);

    digestCloseRespPars.checkStatus();

    ASSERT_EQ(digestCloseRespPars.getSignature(),
              ByteArrayUtil::fromHex(SAM_SIGNATURE));
}

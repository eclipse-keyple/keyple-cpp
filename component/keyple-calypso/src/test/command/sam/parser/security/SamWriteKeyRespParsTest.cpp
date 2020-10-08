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

#include "SamWriteKeyRespPars.h"

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleSeCommandException.h"

using namespace testing;

using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::core::command::exception;
using namespace keyple::core::util;

static const std::string SW1SW2_KO = "6988";
static const std::string SW1SW2_OK = "9000";

TEST(SamWriteKeyRespParsTest, badStatus)
{
    SamWriteKeyRespPars samWriteKeyRespPars(
        std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_KO),
                                       nullptr),
        nullptr);

    EXPECT_THROW(samWriteKeyRespPars.checkStatus(),
                 KeypleSeCommandException);
}

TEST(SamWriteKeyRespParsTest, goodStatus)
{
    SamWriteKeyRespPars samWriteKeyRespPars(
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(SW1SW2_OK), nullptr),
        nullptr);

    samWriteKeyRespPars.checkStatus();
}

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

#include "SamReadKeyParametersRespPars.h"

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleSeCommandException.h"

using namespace testing;

using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::core::command::exception;
using namespace keyple::core::util;

static const std::string SW1SW2_KO = "6A00";
static const std::string SW1SW2_OK = "9000";
static const std::string KEY_PARAMETERS = "11223344";
static const std::string APDU_READ_KEY_PARAMS = KEY_PARAMETERS + SW1SW2_OK;

TEST(SamReadKeyParametersRespParsTest, badStatus)
{
    SamReadKeyParametersRespPars samReadKeyParametersRespPars(
        std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_KO),
                                       nullptr),
        nullptr);

    EXPECT_THROW(samReadKeyParametersRespPars.checkStatus(),
                 KeypleSeCommandException);
}

TEST(SamReadKeyParametersRespParsTest, goodStatus_getSignature)
{
    SamReadKeyParametersRespPars samReadKeyParametersRespPars(
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(APDU_READ_KEY_PARAMS), nullptr),
        nullptr);

    samReadKeyParametersRespPars.checkStatus();

    ASSERT_EQ(samReadKeyParametersRespPars.getKeyParameters(),
              ByteArrayUtil::fromHex(KEY_PARAMETERS));
}

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

#include "SvCheckRespPars.h"

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

TEST(SvCheckRespParsTest, vCheckParse_sucessful)
{
    auto apduResponse = std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("9000"), nullptr);
    SvCheckRespPars svDebitRespPars(apduResponse, nullptr);
    svDebitRespPars.checkStatus();
}

TEST(SvCheckRespParsTest, svCheckParse_failed)
{
    auto apduResponse = std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("6988"), nullptr);
    SvCheckRespPars svDebitRespPars(apduResponse, nullptr);

    ASSERT_FALSE(svDebitRespPars.getApduResponse()->isSuccessful());
    EXPECT_THROW(svDebitRespPars.checkStatus(), KeypleSeCommandException);
}

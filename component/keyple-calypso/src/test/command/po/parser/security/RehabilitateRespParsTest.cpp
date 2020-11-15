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

#include "RehabilitateRespPars.h"

/* Calypso */
#include "CalypsoPoCommandException.h"

/* Common */
#include "IllegalArgumentException.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::po::exception;
using namespace keyple::calypso::command::po::parser::security;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

static const std::vector<uint8_t> SW1SW2_OK = ByteArrayUtil::fromHex("9000");
static const std::vector<uint8_t> SW1SW2_KO = ByteArrayUtil::fromHex("6982");

TEST(RehabilitateRespParsTest, rehabilitateRespPars_goodStatus)
{
    RehabilitateRespPars parser(std::make_shared<ApduResponse>(SW1SW2_OK, nullptr), nullptr);
    parser.checkStatus();
}

TEST(RehabilitateRespParsTest, rehabilitateRespPars_badStatus)
{
    RehabilitateRespPars parser(std::make_shared<ApduResponse>(SW1SW2_KO, nullptr), nullptr);

    EXPECT_THROW(parser.checkStatus(), KeypleSeCommandException);
}

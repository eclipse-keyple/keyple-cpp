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

#include "SelectFileRespPars.h"

#include <map>

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleSeCommandException.h"

/* Common */
#include "stringhelper.h"

using namespace testing;

using namespace keyple::calypso::command::po::parser;
using namespace keyple::common;
using namespace keyple::core::util;
using namespace keyple::core::seproxy::exception;

static const std::string SW1SW2_KO = "6A82";
static const std::string SW1SW2_OK = "9000";
static const std::string ACCESS_CONDITIONS_3F00 = "10100000";
static const std::string KEY_INDEXES_3F00 = "01030101";
static const std::string PROPRIETARY_INFORMATION = "0001000000" +
                                                   ACCESS_CONDITIONS_3F00 +
                                                   KEY_INDEXES_3F00 +
                                                   "00777879616770003F00";
static const std::string PO_SELECT_FILE_3F00_RSP = "8517" +
                                                   PROPRIETARY_INFORMATION +
                                                   SW1SW2_OK;

TEST(SelectFileRespParsTest, selectFileRespParsTest_badStatus)
{
    SelectFileRespPars selectFileRespPars(
        std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_KO),
                                       nullptr),
        nullptr);

    EXPECT_THROW(selectFileRespPars.checkStatus(), KeypleSeCommandException);
}

TEST(SelectFileRespParsTest, selectFileRespParsTest_goodStatus)
{
    SelectFileRespPars selectFileRespPars(
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(PO_SELECT_FILE_3F00_RSP), nullptr),
        nullptr);

    selectFileRespPars.checkStatus();
}

TEST(SelectFileRespParsTest, selectFileRespParsTest_getProprietaryInformation)
{
    SelectFileRespPars selectFileRespPars(
        std::make_shared<ApduResponse>(
            ByteArrayUtil::fromHex(PO_SELECT_FILE_3F00_RSP), nullptr),
        nullptr);

    ASSERT_EQ(selectFileRespPars.getProprietaryInformation(),
              ByteArrayUtil::fromHex(PROPRIETARY_INFORMATION));
}

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

#include "SelectDiversifierCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "SelectDiversifierRespPars.h"

/* Common */
#include "IllegalArgumentException.h"
#include "stringhelper.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::common;
using namespace keyple::core::util;

static const std::string SW1SW2_OK = "9000";
static const std::string PO_DIVERSIFIER = "12345678FEDCBA98";
static const std::string PO_DIVERSIFIER_BAD_LENGTH = "12345678FEDCBA";
static const std::string APDU_CLA_80 = "8014000008" + PO_DIVERSIFIER;
static const std::string APDU_CLA_94 = "9414000008" + PO_DIVERSIFIER;

TEST(SelectDiversifierCmdBuildTest,
     selectDiversifierCmdBuild_defaultRevision_createParser)
{
    SelectDiversifierCmdBuild selectDiversifierCmdBuild(
        SamRevision::NO_REV, ByteArrayUtil::fromHex(PO_DIVERSIFIER));

    ASSERT_EQ(selectDiversifierCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));

    std::shared_ptr<SelectDiversifierRespPars> selectDiversifierRespPars =
        selectDiversifierCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(SW1SW2_OK),
                                           nullptr));

    ASSERT_EQ(typeid(*selectDiversifierRespPars.get()),
              typeid(SelectDiversifierRespPars));
}

TEST(SelectDiversifierCmdBuildTest, selectDiversifierCmdBuild_cla94)
{
    SelectDiversifierCmdBuild selectDiversifierCmdBuild(
            SamRevision::S1D, ByteArrayUtil::fromHex(PO_DIVERSIFIER));

    ASSERT_EQ(selectDiversifierCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_94));
}

TEST(SelectDiversifierCmdBuildTest, selectDiversifierCmdBuild_cla80)
{
    SelectDiversifierCmdBuild selectDiversifierCmdBuild(
            SamRevision::C1, ByteArrayUtil::fromHex(PO_DIVERSIFIER));

    ASSERT_EQ(selectDiversifierCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));
}

TEST(SelectDiversifierCmdBuildTest,
     selectDiversifierCmdBuild_nullptrDiversifier)
{
    EXPECT_THROW(
        std::make_shared<SelectDiversifierCmdBuild>(SamRevision::NO_REV,
                                                    std::vector<uint8_t>{}),
        IllegalArgumentException);
}

TEST(SelectDiversifierCmdBuildTest,
     selectDiversifierCmdBuild_badDiversifierLength)
{
    EXPECT_THROW(
        std::make_shared<SelectDiversifierCmdBuild>(
            SamRevision::NO_REV,
            ByteArrayUtil::fromHex(PO_DIVERSIFIER_BAD_LENGTH)),
        IllegalArgumentException);
}

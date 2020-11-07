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

#include "SvCheckCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "SvCheckRespPars.h"
#include "SamRevision.h"

/* Common */
#include "IllegalArgumentException.h"
#include "stringhelper.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::util;

TEST(SvCheckCmdBuildTest, svCheckCmdBuild_mode_compat_base)
{
    SvCheckCmdBuild svCheckCmdBuild(SamRevision::C1, ByteArrayUtil::fromHex("112233"));
    const std::string apdu = ByteArrayUtil::toHex(svCheckCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "8058000003112233");
}

TEST(SvCheckCmdBuildTest, svCheckCmdBuild_mode_rev3_2_base)
{
    SvCheckCmdBuild svCheckCmdBuild(SamRevision::C1, ByteArrayUtil::fromHex("112233445566"));
    const std::string apdu = ByteArrayUtil::toHex(svCheckCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "8058000006112233445566");
}

TEST(SvCheckCmdBuildTest, svCheckCmdBuild_abort)
{
    SvCheckCmdBuild svCheckCmdBuild(SamRevision::C1, std::vector<uint8_t>{});
    const std::string apdu = ByteArrayUtil::toHex(svCheckCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "8058000000");
}

TEST(SvCheckCmdBuildTest, svCheckCmdBuild_bad_signature)
{
    EXPECT_THROW(SvCheckCmdBuild(SamRevision::C1, ByteArrayUtil::fromHex("1122")),
                 IllegalArgumentException);
}

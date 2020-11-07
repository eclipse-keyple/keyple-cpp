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

#include "SvGetCmdBuild.h"

/* Calypso */
#include "SvGetRespPars.h"

/* Core */
#include "ByteArrayUtil.h"

/* Common */
#include "IllegalArgumentException.h"

using namespace testing;

using namespace keyple::calypso::command::po::builder::storedvalue;
using namespace keyple::calypso::command::po::parser::storedvalue;
using namespace keyple::common::exception;
using namespace keyple::core::util;

using Operation = SvSettings::Operation;

TEST(SvGetCmdBuildTest, svGetCmdBuild_mode_compat_reload)
{
    SvGetCmdBuild svGetCmdBuild(PoClass::ISO, PoRevision::REV3_1, Operation::RELOAD);
    const std::string cmdBytes = ByteArrayUtil::toHex(svGetCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(cmdBytes, "007C000700");
}

TEST(SvGetCmdBuildTest, svGetCmdBuild_mode_compat_debit)
{
    SvGetCmdBuild svGetCmdBuild(PoClass::ISO, PoRevision::REV3_1, Operation::DEBIT);
    const std::string cmdBytes = ByteArrayUtil::toHex(svGetCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(cmdBytes, "007C000900");
}

TEST(SvGetCmdBuildTest, svGetCmdBuild_mode_compat_get_sv_operation)
{
    SvGetCmdBuild svGetCmdBuild1(PoClass::ISO, PoRevision::REV3_1, Operation::RELOAD);

    ASSERT_EQ(svGetCmdBuild1.getSvOperation(), Operation::RELOAD);

    SvGetCmdBuild svGetCmdBuild2(PoClass::ISO, PoRevision::REV3_1, Operation::DEBIT);

    ASSERT_EQ(svGetCmdBuild2.getSvOperation(), Operation::DEBIT);
}

TEST(SvGetCmdBuildTest, svGetCmdBuild_mode_rev32_reload)
{
    SvGetCmdBuild svGetCmdBuild(PoClass::ISO, PoRevision::REV3_2, Operation::RELOAD);
    const std::string cmdBytes = ByteArrayUtil::toHex(svGetCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(cmdBytes, "007C010700");
}

TEST(SvGetCmdBuildTest, svGetCmdBuild_mode_rev32_debit)
{
    SvGetCmdBuild svGetCmdBuild(PoClass::ISO, PoRevision::REV3_2, Operation::DEBIT);
    const std::string cmdBytes = ByteArrayUtil::toHex(svGetCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(cmdBytes, "007C010900");
}

TEST(SvGetCmdBuildTest, svGetCmdBuild_mode_rev32_get_sv_operation)
{
    SvGetCmdBuild svGetCmdBuild1(PoClass::ISO, PoRevision::REV3_2, Operation::RELOAD);

    ASSERT_EQ(svGetCmdBuild1.getSvOperation(), Operation::RELOAD);

    SvGetCmdBuild svGetCmdBuild2(PoClass::ISO, PoRevision::REV3_2, Operation::DEBIT);

    ASSERT_EQ(svGetCmdBuild2.getSvOperation(), Operation::DEBIT);
}

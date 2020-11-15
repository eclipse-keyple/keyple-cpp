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

#include "SvReloadCmdBuild.h"

/* Calypso */
#include "SvReloadRespPars.h"

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

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_compat_base)
{
    SvReloadCmdBuild svReloadCmdBuild(PoClass::ISO,
                                      PoRevision::REV3_1,
                                      1, /* Amount */
                                      0xAA, /* KVC */
                                      ByteArrayUtil::fromHex("1122"), /* Date */
                                      ByteArrayUtil::fromHex("3344"), /* Time */
                                      ByteArrayUtil::fromHex("F3EE")); /* Free */
    svReloadCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));

    ASSERT_EQ(svReloadCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex("00B8556617771122F3AAEE0000013344AABBCCDD1234561122334455"));
    ASSERT_EQ(svReloadCmdBuild.getSvReloadData(),
              ByteArrayUtil::fromHex("B8000017771122F3AAEE0000013344"));
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_compat_not_finalized)
{
    SvReloadCmdBuild svReloadCmdBuild(PoClass::ISO,
                                      PoRevision::REV3_1,
                                      1, /* Amount */
                                      0xAA, /* KVC */
                                      ByteArrayUtil::fromHex("1122"), /* Date */
                                      ByteArrayUtil::fromHex("3344"), /* Time */
                                      ByteArrayUtil::fromHex("F3EE")); /* Free */
    std::shared_ptr<ApduRequest> request = svReloadCmdBuild.getApduRequest();

    ASSERT_EQ(request, nullptr);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_compat_overlimit_negative_amount)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_1,
                                  -8388609, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("1122"), /* Date */
                                  ByteArrayUtil::fromHex("3344"), /* Time */
                                  ByteArrayUtil::fromHex("F3EE")), /* Free */
                 IllegalArgumentException);

    //svReloadCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_compat_overlimit_positive_amount)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_1,
                                  8388608, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("1122"), /* Date */
                                  ByteArrayUtil::fromHex("3344"), /* Time */
                                  ByteArrayUtil::fromHex("F3EE")), /* Free */
                 IllegalArgumentException);

    //svReloadCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_compat_bad_signature_length_1)
{
    SvReloadCmdBuild svReloadCmdBuild(PoClass::ISO,
                                      PoRevision::REV3_1,
                                      1, /* Amount */
                                      0xAA, /* KVC */
                                      ByteArrayUtil::fromHex("1122"), /* Date */
                                      ByteArrayUtil::fromHex("3344"), /* Time */
                                      ByteArrayUtil::fromHex("F3EE")); /* Free */

    EXPECT_THROW(
        svReloadCmdBuild.finalizeBuilder(
            ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900")),
        IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_compat_bad_signature_length_2)
{
    SvReloadCmdBuild svReloadCmdBuild(PoClass::ISO,
                                      PoRevision::REV3_1,
                                      1, /* Amount */
                                      0xAA, /* KVC */
                                      ByteArrayUtil::fromHex("1122"), /* Date */
                                      ByteArrayUtil::fromHex("3344"), /* Time */
                                      ByteArrayUtil::fromHex("F3EE")); /* Free */

    EXPECT_THROW(
        svReloadCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD556677123456112233")),
        IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_compat_illegal_date_1)
{
    /* Long date */
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_1,
                                  1, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("112233"), /* Date */
                                  ByteArrayUtil::fromHex("3344"), /* Time */
                                  ByteArrayUtil::fromHex("5566")), /* Free */
                 IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_compat_illegal_date_2)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_1,
                                  1, /* Amount */
                                  0xAA, /* KVC */
                                  std::vector<uint8_t>{}, /* Date */
                                  ByteArrayUtil::fromHex("3344"), /* Time */
                                  ByteArrayUtil::fromHex("5566")), /* Free */
                 IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_compat_illegal_time_1)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_1,
                                  1, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("1122"), /* Date */
                                  ByteArrayUtil::fromHex("334455"), /* Time */
                                  ByteArrayUtil::fromHex("5566")), /* Free */
                 IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_compat_illegal_time_2)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_1,
                                  1, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("1122"), /* Date */
                                  std::vector<uint8_t>{}, /* Time */
                                  ByteArrayUtil::fromHex("5566")), /* Free */
                 IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_compat_illegal_free_1)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_1,
                                  1, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("1122"), /* Date */
                                  ByteArrayUtil::fromHex("3344"), /* Time */
                                  ByteArrayUtil::fromHex("556677")), /* Free */
                 IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_compat_illegal_free_2)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_1,
                                  1, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("1122"), /* Date */
                                  ByteArrayUtil::fromHex("3344"), /* Time */
                                  std::vector<uint8_t>{}), /* Free */
                 IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_base)
{
    SvReloadCmdBuild svReloadCmdBuild(PoClass::ISO,
                                      PoRevision::REV3_2,
                                      1, /* Amount */
                                      0xAA, /* KVC */
                                      ByteArrayUtil::fromHex("1122"), /* Date */
                                      ByteArrayUtil::fromHex("3344"), /* Time */
                                      ByteArrayUtil::fromHex("F3EE")); /* Free */
    svReloadCmdBuild.finalizeBuilder(
        ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    const std::string apdu = ByteArrayUtil::toHex(svReloadCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00B855661C771122F3AAEE0000013344AABBCCDD12345611223344556677889900");
    ASSERT_EQ(svReloadCmdBuild.getSvReloadData(),
              ByteArrayUtil::fromHex("B800001C771122F3AAEE0000013344"));
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_not_finalized)
{
    SvReloadCmdBuild svReloadCmdBuild(PoClass::ISO,
                                      PoRevision::REV3_2,
                                      1, /* Amount */
                                      0xAA, /* KVC */
                                      ByteArrayUtil::fromHex("1122"), /* Date */
                                      ByteArrayUtil::fromHex("3344"), /* Time */
                                      ByteArrayUtil::fromHex("F3EE")); /* Free */
    std::shared_ptr<ApduRequest> request = svReloadCmdBuild.getApduRequest();

    ASSERT_EQ(request, nullptr);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_amout_256)
{
    SvReloadCmdBuild svReloadCmdBuild(PoClass::ISO,
                                      PoRevision::REV3_2,
                                      256, /* Amount */
                                      0xAA, /* KVC */
                                      ByteArrayUtil::fromHex("1122"), /* Date */
                                      ByteArrayUtil::fromHex("3344"), /* Time */
                                      ByteArrayUtil::fromHex("F3EE")); /* Free */
    svReloadCmdBuild.finalizeBuilder(
        ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    const std::string apdu = ByteArrayUtil::toHex(svReloadCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00B855661C771122F3AAEE0001003344AABBCCDD12345611223344556677889900");
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_amout_65536)
{
    SvReloadCmdBuild svReloadCmdBuild(PoClass::ISO,
                                      PoRevision::REV3_2,
                                      65536, /* Amount */
                                      0xAA, /* KVC */
                                      ByteArrayUtil::fromHex("1122"), /* Date */
                                      ByteArrayUtil::fromHex("3344"), /* Time */
                                      ByteArrayUtil::fromHex("F3EE")); /* Free */
    svReloadCmdBuild.finalizeBuilder(
        ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    const std::string apdu = ByteArrayUtil::toHex(svReloadCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00B855661C771122F3AAEE0100003344AABBCCDD12345611223344556677889900");
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_amout_m1)
{
    SvReloadCmdBuild svReloadCmdBuild(PoClass::ISO,
                                      PoRevision::REV3_2,
                                      -1, /* Amount */
                                      0xAA, /* KVC */
                                      ByteArrayUtil::fromHex("1122"), /* Date */
                                      ByteArrayUtil::fromHex("3344"), /* Time */
                                      ByteArrayUtil::fromHex("F3EE")); /* Free */
    svReloadCmdBuild.finalizeBuilder(
        ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    const std::string apdu = ByteArrayUtil::toHex(svReloadCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00B855661C771122F3AAEEFFFFFF3344AABBCCDD12345611223344556677889900");
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_amout_m256)
{
    SvReloadCmdBuild svReloadCmdBuild(PoClass::ISO,
                                      PoRevision::REV3_2,
                                      -256, /* Amount */
                                      0xAA, /* KVC */
                                      ByteArrayUtil::fromHex("1122"), /* Date */
                                      ByteArrayUtil::fromHex("3344"), /* Time */
                                      ByteArrayUtil::fromHex("F3EE")); /* Free */
    svReloadCmdBuild.finalizeBuilder(
        ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    const std::string apdu = ByteArrayUtil::toHex(svReloadCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00B855661C771122F3AAEEFFFF003344AABBCCDD12345611223344556677889900");
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_overlimit_negative_amount)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_2,
                                  -8388609, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("1122"), /* Date */
                                  ByteArrayUtil::fromHex("3344"), /* Time */
                                  ByteArrayUtil::fromHex("F3EE")), /* Free */
                 IllegalArgumentException);

    //svReloadCmdBuild.finalizeBuilder(
    //    ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_overlimit_positive_amount)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_2,
                                  8388608, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("1122"), /* Date */
                                  ByteArrayUtil::fromHex("3344"), /* Time */
                                  ByteArrayUtil::fromHex("F3EE")), /* Free */
                 IllegalArgumentException);

    //svReloadCmdBuild.finalizeBuilder(
    //    ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_bad_signature_length_1)
{
    SvReloadCmdBuild svReloadCmdBuild(PoClass::ISO,
                                      PoRevision::REV3_2,
                                      1, /* Amount */
                                      0xAA, /* KVC */
                                      ByteArrayUtil::fromHex("1122"), /* Date */
                                      ByteArrayUtil::fromHex("3344"), /* Time */
                                      ByteArrayUtil::fromHex("F3EE")); /* Free */

    EXPECT_THROW(
        svReloadCmdBuild.finalizeBuilder(
            ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455")),
        IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_bad_signature_length_2)
{
    SvReloadCmdBuild svReloadCmdBuild(PoClass::ISO,
                                      PoRevision::REV3_2,
                                      1, /* Amount */
                                      0xAA, /* KVC */
                                      ByteArrayUtil::fromHex("1122"), /* Date */
                                      ByteArrayUtil::fromHex("3344"), /* Time */
                                      ByteArrayUtil::fromHex("F3EE")); /* Free */
    EXPECT_THROW(
        svReloadCmdBuild.finalizeBuilder(
            ByteArrayUtil::fromHex("AABBCCDD5566771234561122")),
        IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_illegal_date_1)
{
    /* Long date */
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_2,
                                  1, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("112233"), /* Date */
                                  ByteArrayUtil::fromHex("3344"), /* Time */
                                  ByteArrayUtil::fromHex("5566")), /* Free */
                 IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_illegal_date_2)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_2,
                                  1, /* Amount */
                                  0xAA, /* KVC */
                                  std::vector<uint8_t>{}, /* Date */
                                  ByteArrayUtil::fromHex("3344"), /* Time */
                                  ByteArrayUtil::fromHex("5566")), /* Free */
                 IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_illegal_time_1)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_2,
                                  1, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("1122"), /* Date */
                                  ByteArrayUtil::fromHex("334455"), /* Time */
                                  ByteArrayUtil::fromHex("5566")), /* Free */
                 IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_illegal_time_2)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_2,
                                  1, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("1122"), /* Date */
                                  std::vector<uint8_t>{}, /* Time */
                                  ByteArrayUtil::fromHex("5566")), /* Free */
                 IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_illegal_free_1)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_2,
                                  1, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("1122"), /* Date */
                                  ByteArrayUtil::fromHex("3344"), /* Time */
                                  ByteArrayUtil::fromHex("556677")), /* Free */
                 IllegalArgumentException);
}

TEST(SvReloadCmdBuildTest, svReloadCmdBuild_mode_rev3_2_illegal_free_2)
{
    EXPECT_THROW(SvReloadCmdBuild(PoClass::ISO,
                                  PoRevision::REV3_2,
                                  1, /* Amount */
                                  0xAA, /* KVC */
                                  ByteArrayUtil::fromHex("1122"), /* Date */
                                  ByteArrayUtil::fromHex("3344"), /* Time */
                                  std::vector<uint8_t>{}), /* Free */
                 IllegalArgumentException);
}

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

#include "SvDebitCmdBuild.h"

/* Calypso */
#include "SvDebitRespPars.h"

/* Core */
#include "ByteArrayUtil.h"

/* Common */
#include "IllegalArgumentException.h"

using namespace testing;

using namespace keyple::calypso::command::po::builder::storedvalue;
using namespace keyple::calypso::command::po::parser::storedvalue;
using namespace keyple::common::exception;
using namespace keyple::core::util;

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_base)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_1,
                                    1, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */
    svDebitCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
    const std::string apdu = ByteArrayUtil::toHex(svDebitCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00BA55661477FFFF11223344AAAABBCCDD1234561122334455");
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_not_finalized)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_1,
                                    1, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */
    std::shared_ptr<ApduRequest> request = svDebitCmdBuild.getApduRequest();

    ASSERT_TRUE(request == nullptr);
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_4081)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_1,
                                    4081, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */
    svDebitCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
    const std::string apdu = ByteArrayUtil::toHex(svDebitCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00BA55661477F00F11223344AAAABBCCDD1234561122334455");
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_negative_amount)
{
    /** @see Calypso Layer ID 8.02 (200108) */
    EXPECT_THROW(SvDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_1,
                                 -1, /* Amount */
                                 0xAA, /* KVC */
                                 ByteArrayUtil::fromHex("1122"), /* Date */
                                 ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);

    //svDebitCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
    //EXPECT_THROW(svDebitCmdBuild.getApduRequest()->getBytes(), std::exception);
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_overlimit_amount)
{
    /** @see Calypso Layer ID 8.02 (200108) */
    EXPECT_THROW(SvDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_1,
                                 32768, /* Amount */
                                 0xAA, /* KVC */
                                 ByteArrayUtil::fromHex("1122"), /* Date */
                                 ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);

    //svDebitCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
    //svDebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_bad_signature_length_1)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_1,
                                    1, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */

    EXPECT_THROW(svDebitCmdBuild.finalizeBuilder(
                     ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900")),
                 IllegalArgumentException);

    //svDebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_bad_signature_length_2)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_1,
                                 1, /* Amount */
                                 0xAA, /* KVC */
                                 ByteArrayUtil::fromHex("1122"), /* Date */
                                 ByteArrayUtil::fromHex("3344"));/* Time */

    EXPECT_THROW(svDebitCmdBuild.finalizeBuilder(
                     ByteArrayUtil::fromHex("AABBCCDD556677123456112233")),
                 IllegalArgumentException);

    //EXPECT_THROW(svDebitCmdBuild.getApduRequest()->getBytes(), IllegalArgumentException);
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_illegal_date_1)
{
    /* Long date */
    EXPECT_THROW(SvDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_1,
                                 1, /* Amount */
                                 0xAA, /* KVC */
                                 ByteArrayUtil::fromHex("112233"), /* Date */
                                 ByteArrayUtil::fromHex("3344")),  /* Time */
                 IllegalArgumentException);
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_illegal_date_2)
{
    /* null date */
    EXPECT_THROW(SvDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_1,
                                 1, /* Amount */
                                 0xAA, /* KVC */
                                 std::vector<uint8_t>{}, /* Date */
                                 ByteArrayUtil::fromHex("3344")),  /* Time */
                 IllegalArgumentException);
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_illegal_time_1)
{
    EXPECT_THROW(SvDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_1,
                                 1, /* Amount */
                                 0xAA, /* KVC */
                                 ByteArrayUtil::fromHex("1122"), /* Date */
                                 ByteArrayUtil::fromHex("334455")),  /* Time */
                 IllegalArgumentException);
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_illegal_time_2)
{
    EXPECT_THROW(SvDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_1,
                                 1, /* Amount */
                                 0xAA, /* KVC */
                                 ByteArrayUtil::fromHex("1122"), /* Date */
                                 std::vector<uint8_t>{}),  /* Time */
                 IllegalArgumentException);
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_get_debit_data)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_1,
                                    1, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */
    const std::vector<uint8_t> svDebitData = svDebitCmdBuild.getSvDebitData();

    ASSERT_EQ(svDebitData, ByteArrayUtil::fromHex("BA00001400FFFF11223344AA"));
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_compat_response_parser)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_1,
                                    1, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */
    //const std::vector<uint8_t> svDebitData = svDebitCmdBuild.getSvDebitData();
    svDebitCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
    auto apdu = std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("1122339000"), nullptr);
    std::shared_ptr<SvDebitRespPars> svBuildCmdParser = svDebitCmdBuild.createResponseParser(apdu);

    ASSERT_EQ(svBuildCmdParser->getSignatureLo(), ByteArrayUtil::fromHex("112233"));
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_rev3_2_base)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_2,
                                    1, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */
    svDebitCmdBuild.finalizeBuilder(
        ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    const std::string apdu = ByteArrayUtil::toHex(svDebitCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00BA55661977FFFF11223344AAAABBCCDD12345611223344556677889900");
}


TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_rev3_2_not_finalized)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_2,
                                    1, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */
    std::shared_ptr<ApduRequest> request = svDebitCmdBuild.getApduRequest();

    ASSERT_TRUE(request == nullptr);
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_rev3_2_4081)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_2,
                                    4081, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */
    svDebitCmdBuild.finalizeBuilder(
        ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    const std::string apdu = ByteArrayUtil::toHex(svDebitCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00BA55661977F00F11223344AAAABBCCDD12345611223344556677889900");
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_rev3_2_negative_amount)
{
    /** @see Calypso Layer ID 8.02 (200108) */
    EXPECT_THROW(SvDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_2,
                                 -1, /* Amount */
                                 0xAA, /* KVC */
                                 ByteArrayUtil::fromHex("1122"), /* Date */
                                 ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);

    //svDebitCmdBuild.finalizeBuilder( ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"))
    //svDebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_rev3_2_overlimit_amount)
{
    /** @see Calypso Layer ID 8.02 (200108) */
    EXPECT_THROW(SvDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_2,
                                 32768, /* Amount */
                                 0xAA, /* KVC */
                                 ByteArrayUtil::fromHex("1122"), /* Date */
                                 ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);

    //svDebitCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
    //svDebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_rev3_2_bad_signature_length_1)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_2,
                                    1, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */

    EXPECT_THROW(svDebitCmdBuild.finalizeBuilder(
                     ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455")),
                 IllegalArgumentException);

    //svDebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_rev3_2_bad_signature_length_2)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_2,
                                    1, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */
    EXPECT_THROW(svDebitCmdBuild.finalizeBuilder(
                     ByteArrayUtil::fromHex("AABBCCDD556677123456112233")),
                 IllegalArgumentException);

    //svDebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvDebitCmdBuildTest,  svDebitCmdBuild_mode_rev3_2_illegal_date_1)
{
    EXPECT_THROW(SvDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_2,
                                 1, /* Amount */
                                 0xAA, /* KVC */
                                 ByteArrayUtil::fromHex("112233"), /* Date */
                                 ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_rev3_2_illegal_date_2)
{
    EXPECT_THROW(SvDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_2,
                                 1, /* Amount */
                                 0xAA, /* KVC */
                                 std::vector<uint8_t>{}, /* Date */
                                 ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_rev3_2_illegal_time_1)
{
    EXPECT_THROW(SvDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_2,
                                 1, /* Amount */
                                 0xAA, /* KVC */
                                 ByteArrayUtil::fromHex("1122"), /* Date */
                                 ByteArrayUtil::fromHex("334455")), /* Time */
                 IllegalArgumentException);
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_rev3_2_illegal_time_2)
{
    EXPECT_THROW(SvDebitCmdBuild(PoClass::ISO,
                                 PoRevision::REV3_2,
                                 1, /* Amount */
                                 0xAA, /* KVC */
                                 ByteArrayUtil::fromHex("1122"), /* Date */
                                 std::vector<uint8_t>{}), /* Time */
                 IllegalArgumentException);
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_rev3_2_get_debit_data)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_2,
                                    1, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */
    const std::vector<uint8_t> svDebitData = svDebitCmdBuild.getSvDebitData();

    ASSERT_EQ(svDebitData, ByteArrayUtil::fromHex("BA00001900FFFF11223344AA"));
}

TEST(SvDebitCmdBuildTest, svDebitCmdBuild_mode_rev3_2_response_parser)
{
    SvDebitCmdBuild svDebitCmdBuild(PoClass::ISO,
                                    PoRevision::REV3_2,
                                    1, /* Amount */
                                    0xAA, /* KVC */
                                    ByteArrayUtil::fromHex("1122"), /* Date */
                                    ByteArrayUtil::fromHex("3344")); /* Time */
    //const std::vector<uint8_t> svDebitData = svDebitCmdBuild.getSvDebitData();
    svDebitCmdBuild.finalizeBuilder(
        ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    auto apdu = std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("1122334455669000"), nullptr);
    std::shared_ptr<SvDebitRespPars> svBuildCmdParser = svDebitCmdBuild.createResponseParser(apdu);

    ASSERT_EQ(svBuildCmdParser->getSignatureLo(), ByteArrayUtil::fromHex("112233445566"));
}

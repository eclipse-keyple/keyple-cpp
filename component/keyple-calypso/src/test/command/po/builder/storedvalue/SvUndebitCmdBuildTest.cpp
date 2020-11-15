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

#include "SvUndebitCmdBuild.h"

/* Calypso */
#include "SvUndebitRespPars.h"

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

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_base)
{
    /* */
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_1,
                                        1, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */
    svUndebitCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
    const std::string apdu = ByteArrayUtil::toHex(svUndebitCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00BC55661477000111223344AAAABBCCDD1234561122334455");
}


TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_not_finalized)
{
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_1,
                                        1, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */
    std::shared_ptr<ApduRequest> request = svUndebitCmdBuild.getApduRequest();

    ASSERT_EQ(request, nullptr);
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_256)
{
    /* */
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_1,
                                        256, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */
    svUndebitCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
    const std::string apdu = ByteArrayUtil::toHex(svUndebitCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00BC55661477010011223344AAAABBCCDD1234561122334455");
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_negative_amount)
{
    /** @see Calypso Layer ID 8.02 (200108) */
    EXPECT_THROW(SvUndebitCmdBuild(PoClass::ISO,
                                   PoRevision::REV3_1,
                                   -1, /* Amount */
                                   0xAA, /* KVC */
                                   ByteArrayUtil::fromHex("1122"), /* Date */
                                   ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);

    //svUndebitCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
    //svUndebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_overlimit_amount)
{
    /** @see Calypso Layer ID 8.02 (200108) */
    EXPECT_THROW(SvUndebitCmdBuild(PoClass::ISO,
                                   PoRevision::REV3_1,
                                   32768, /* Amount */
                                   0xAA, /* KVC */
                                   ByteArrayUtil::fromHex("1122"), /* Date */
                                   ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);

    //svUndebitCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
    //svUndebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_bad_signature_length_1)
{
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_1,
                                        1, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */
    EXPECT_THROW(svUndebitCmdBuild.finalizeBuilder(
                     ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900")),
                 IllegalArgumentException);

    //svUndebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_bad_signature_length_2)
{
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_1,
                                        1, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */

    EXPECT_THROW(svUndebitCmdBuild.finalizeBuilder(
                     ByteArrayUtil::fromHex("AABBCCDD556677123456112233")),
                 IllegalArgumentException);

    //svUndebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_illegal_date_1)
{
    /* Long date */
    EXPECT_THROW(SvUndebitCmdBuild(PoClass::ISO,
                                   PoRevision::REV3_1,
                                   1, /* Amount */
                                   0xAA, /* KVC */
                                   ByteArrayUtil::fromHex("112233"), /* Date */
                                   ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_illegal_date_2)
{
    EXPECT_THROW(SvUndebitCmdBuild(PoClass::ISO,
                                   PoRevision::REV3_1,
                                   1, /* Amount */
                                   0xAA, /* KVC */
                                   std::vector<uint8_t>{}, /* Date */
                                   ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_illegal_time_1)
{
    EXPECT_THROW(SvUndebitCmdBuild(PoClass::ISO,
                                   PoRevision::REV3_1,
                                   1, /* Amount */
                                   0xAA, /* KVC */
                                   ByteArrayUtil::fromHex("1122"), /* Date */
                                   ByteArrayUtil::fromHex("334455")), /* Time */
                 IllegalArgumentException);
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_illegal_time_2)
{
    EXPECT_THROW(SvUndebitCmdBuild(PoClass::ISO,
                                   PoRevision::REV3_1,
                                   1, /* Amount */
                                   0xAA, /* KVC */
                                   ByteArrayUtil::fromHex("1122"), /* Date */
                                   std::vector<uint8_t>{}), /* Time */
                 IllegalArgumentException);
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_get_debit_data)
{
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_1,
                                        1, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */
    const std::vector<uint8_t> svUndebitData = svUndebitCmdBuild.getSvUndebitData();

    ASSERT_EQ(svUndebitData, ByteArrayUtil::fromHex("BC00001400000111223344AA"));
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_compat_response_parser)
{
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_1,
                                        1, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */
    const std::vector<uint8_t> svUndebitData = svUndebitCmdBuild.getSvUndebitData();
    svUndebitCmdBuild.finalizeBuilder(ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455"));
    auto apdu = std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("1122339000"), nullptr);
    std::shared_ptr<SvUndebitRespPars> svBuildCmdParser =
        svUndebitCmdBuild.createResponseParser(apdu);

    ASSERT_EQ(svBuildCmdParser->getSignatureLo(), ByteArrayUtil::fromHex("112233"));
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_rev3_2_base)
{
    /* */
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_2,
                                        1, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */
    svUndebitCmdBuild.finalizeBuilder(
        ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    const std::string apdu = ByteArrayUtil::toHex(svUndebitCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00BC55661977000111223344AAAABBCCDD12345611223344556677889900");
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_rev3_2_256)
{
    /* */
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_2,
                                        256, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */
    svUndebitCmdBuild.finalizeBuilder(
        ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    const std::string apdu = ByteArrayUtil::toHex(svUndebitCmdBuild.getApduRequest()->getBytes());

    ASSERT_EQ(apdu, "00BC55661977010011223344AAAABBCCDD12345611223344556677889900");
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_rev3_2_negative_amount)
{
    /** @see Calypso Layer ID 8.02 (200108) */
    EXPECT_THROW(SvUndebitCmdBuild(PoClass::ISO,
                                   PoRevision::REV3_2,
                                   -1, /* Amount */
                                   0xAA, /* KVC */
                                   ByteArrayUtil::fromHex("1122"), /* Date */
                                   ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);

    //svUndebitCmdBuild.finalizeBuilder(
    //    ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    //svUndebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_rev3_2_overlimit_amount)
{
    /** @see Calypso Layer ID 8.02 (200108) */
    EXPECT_THROW(SvUndebitCmdBuild(PoClass::ISO,
                                   PoRevision::REV3_2,
                                   32769, /* Amount */
                                   0xAA, /* KVC */
                                   ByteArrayUtil::fromHex("1122"), /* Date */
                                   ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);

    //svUndebitCmdBuild.finalizeBuilder(
    //    ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    //svUndebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_rev3_2_bad_signature_length_1)
{
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_2,
                                        1, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */
    EXPECT_THROW(svUndebitCmdBuild.finalizeBuilder(
                     ByteArrayUtil::fromHex("AABBCCDD556677123456112233445566778899001122")),
                 IllegalArgumentException);

    //svUndebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_rev3_2_bad_signature_length_2)
{
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_2,
                                        1, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */
    EXPECT_THROW(svUndebitCmdBuild.finalizeBuilder(
                     ByteArrayUtil::fromHex("AABBCCDD5566771234561122334455667788")),
                 IllegalArgumentException);

    //svUndebitCmdBuild.getApduRequest()->getBytes();
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_rev3_2_illegal_date_1)
{
    /* Long date */
    EXPECT_THROW(SvUndebitCmdBuild(PoClass::ISO,
                                   PoRevision::REV3_2,
                                   1, /* Amount */
                                   0xAA, /* KVC */
                                   ByteArrayUtil::fromHex("112233"), /* Date */
                                   ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_rev3_2_illegal_date_2)
{
    EXPECT_THROW(SvUndebitCmdBuild(PoClass::ISO,
                                   PoRevision::REV3_2,
                                   1, /* Amount */
                                   0xAA, /* KVC */
                                   std::vector<uint8_t>{}, /* Date */
                                   ByteArrayUtil::fromHex("3344")), /* Time */
                 IllegalArgumentException);
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_rev3_2_illegal_time_1)
{
    EXPECT_THROW(SvUndebitCmdBuild(PoClass::ISO,
                                   PoRevision::REV3_2,
                                   1, /* Amount */
                                   0xAA, /* KVC */
                                   ByteArrayUtil::fromHex("1122"), /* Date */
                                   ByteArrayUtil::fromHex("334455")), /* Time */
                 IllegalArgumentException);
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_rev3_2_illegal_time_2)
{
    EXPECT_THROW(SvUndebitCmdBuild(PoClass::ISO,
                                   PoRevision::REV3_2,
                                   1, /* Amount */
                                   0xAA, /* KVC */
                                   ByteArrayUtil::fromHex("1122"), /* Date */
                                   std::vector<uint8_t>{}), /* Time */
                 IllegalArgumentException);
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_rev3_2_get_debit_data)
{
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_2,
                                        1, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */
    const std::vector<uint8_t> svUndebitData = svUndebitCmdBuild.getSvUndebitData();

    ASSERT_EQ(svUndebitData, ByteArrayUtil::fromHex("BC00001900000111223344AA"));
}

TEST(SvUndebitCmdBuildTest, svUndebitCmdBuild_mode_rev3_2_response_parser)
{
    SvUndebitCmdBuild svUndebitCmdBuild(PoClass::ISO,
                                        PoRevision::REV3_2,
                                        1, /* Amount */
                                        0xAA, /* KVC */
                                        ByteArrayUtil::fromHex("1122"), /* Date */
                                        ByteArrayUtil::fromHex("3344")); /* Time */
    const std::vector<uint8_t> svUndebitData = svUndebitCmdBuild.getSvUndebitData();
    svUndebitCmdBuild.finalizeBuilder(
        ByteArrayUtil::fromHex("AABBCCDD55667712345611223344556677889900"));
    auto apdu = std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("1122334455669000"), nullptr);
    std::shared_ptr<SvUndebitRespPars> svBuildCmdParser =
        svUndebitCmdBuild.createResponseParser(apdu);

    ASSERT_EQ(svBuildCmdParser->getSignatureLo(), ByteArrayUtil::fromHex("112233445566"));
}

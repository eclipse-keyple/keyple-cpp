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

#include "ByteArrayUtil.h"

#include "IllegalArgumentException.h"

using namespace testing;

using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::util;

static const std::string HEXSTRING_ODD = "0102030";
static const std::string HEXSTRING_BAD = "010203ABGH80";
static const std::string HEXSTRING_GOOD = "1234567890ABCDEFFEDCBA0987654321";
static const std::vector<uint8_t> BYTEARRAY_GOOD = {
    0x12, 0x34, 0x56, 0x78, 0x90,
    0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA,
    0x09, 0x87, 0x65, 0x43, 0x21};
static const std::vector<uint8_t> BYTEARRAY_LEN_2 = {0x12, 0x34};
static const std::vector<uint8_t> BYTEARRAY_LEN_3 = {0x12, 0x34, 0x56};


TEST(ByteArrayUtilTest, fromHex_empty)
{
    std::vector<uint8_t> bytes = ByteArrayUtil::fromHex("");

    ASSERT_EQ(static_cast<int>(bytes.size()), 0);
}

TEST(ByteArrayUtilTest, fromHex_odd_length)
{
    EXPECT_THROW(ByteArrayUtil::fromHex(HEXSTRING_ODD),
                 IllegalArgumentException);
}

TEST(ByteArrayUtilTest, fromHex_bad_hex)
{
    /* No verification is being carried out at the moment */
    ByteArrayUtil::fromHex(HEXSTRING_BAD);
}

TEST(ByteArrayUtilTest, fromHex_good_hex)
{
    std::vector<uint8_t> bytes = ByteArrayUtil::fromHex(HEXSTRING_GOOD);

    ASSERT_EQ(bytes, BYTEARRAY_GOOD);
}

TEST(ByteArrayUtilTest, toHex_empty)
{
    std::vector<uint8_t> bytes;

    std::string hex = ByteArrayUtil::toHex(bytes);

    ASSERT_EQ(static_cast<int>(hex.size()), 0);
}

TEST(ByteArrayUtilTest, toHex_bytearray_good)
{
    std::string hex = ByteArrayUtil::toHex(BYTEARRAY_GOOD);

    ASSERT_EQ(hex, HEXSTRING_GOOD);
}

TEST(ByteArrayUtilTest, threeBytesToInt_negative_offset)
{
    EXPECT_THROW(ByteArrayUtil::threeBytesToInt(BYTEARRAY_GOOD, -1),
                IllegalArgumentException);
}

TEST(ByteArrayUtilTest, threeBytesToInt_too_short_buffer_1)
{
    EXPECT_THROW(ByteArrayUtil::threeBytesToInt(BYTEARRAY_LEN_2, 0),
                 IllegalArgumentException);
}

TEST(ByteArrayUtilTest, threeBytesToInt_too_short_buffer_2)
{
    EXPECT_THROW(ByteArrayUtil::threeBytesToInt(BYTEARRAY_LEN_3, 1),
                 IllegalArgumentException);
}

TEST(ByteArrayUtilTest, threeBytesToInt_buffer_ok_1)
{
    int value = ByteArrayUtil::threeBytesToInt(BYTEARRAY_LEN_3, 0);

    ASSERT_EQ(value, 0x123456);
}

TEST(ByteArrayUtilTest, threeBytesToInt_buffer_ok_2)
{
    int value = ByteArrayUtil::threeBytesToInt(BYTEARRAY_GOOD, 0);

    ASSERT_EQ(value, 0x123456);
}

TEST(ByteArrayUtilTest, threeBytesToInt_buffer_ok_3)
{
    int value = ByteArrayUtil::threeBytesToInt(BYTEARRAY_GOOD, 1);

    ASSERT_EQ(value, 0x345678);
}

TEST(ByteArrayUtilTest, threeBytesToInt_buffer_ok_4)
{
    int value = ByteArrayUtil::threeBytesToInt(BYTEARRAY_GOOD, 4);

    ASSERT_EQ(value, 0x90ABCD);
}

TEST(ByteArrayUtilTest, threeBytesToInt_buffer_ok_5)
{
    int value = ByteArrayUtil::threeBytesToInt(BYTEARRAY_GOOD, 13);

    ASSERT_EQ(value, 0x654321);
}

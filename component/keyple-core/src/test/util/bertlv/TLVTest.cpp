
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

#include "TLV.h"

#include "Tag.h"

using namespace keyple::core::util::bertlv;

using namespace testing;

TEST(TLVTest, TLV)
{
    const std::vector<uint8_t> binary;

    TLV tlv(binary);
}

TEST(TLVTest, parse)
{
    std::shared_ptr<Tag> tag1 =
        std::make_shared<Tag>(0x04, Tag::TagClass::CONTEXT,
                              Tag::TagType::PRIMITIVE, 1);
    std::shared_ptr<Tag> tag2 =
        std::make_shared<Tag>(0x04, Tag::TagClass::CONTEXT,
                              Tag::TagType::CONSTRUCTED, 1);

    TLV tlv({0x84, 0x05, 0x00, 0x11, 0x22, 0x33, 0x44});

    /* 1st parsing */
    ASSERT_TRUE(tlv.parse(tag1, 0));
    /* 2nd same parsing */
    ASSERT_TRUE(tlv.parse(tag1, 0));
    /* Search another tag */
    ASSERT_FALSE(tlv.parse(tag2, 0));
}

TEST(TLVTest, parse_TagNull)
{
    TLV tlv({ 0x84, 0x05, 0x00, 0x11, 0x22, 0x33, 0x44 });

    EXPECT_THROW(tlv.parse(nullptr, 0), std::invalid_argument);
}

TEST(TLVTest, parse_TooLargeIndex)
{
    std::shared_ptr<Tag> tag1 =
        std::make_shared<Tag>(0x04, Tag::TagClass::CONTEXT,
                              Tag::TagType::PRIMITIVE, 1);

    TLV tlv({ 0x84, 0x05, 0x00, 0x11, 0x22, 0x33, 0x44 });

    EXPECT_THROW(tlv.parse(tag1, 20), std::invalid_argument);
}

TEST(TLVTest, getValue)
{
    const std::vector<uint8_t> value = {0x00, 0x11, 0x22, 0x33, 0x44};
    std::shared_ptr<Tag> tag1 =
        std::make_shared<Tag>(0x04, Tag::TagClass::CONTEXT,
                              Tag::TagType::PRIMITIVE, 1);

    TLV tlv1({0x84, 0x05, 0x00, 0x11, 0x22, 0x33, 0x44});

    ASSERT_TRUE(tlv1.parse(tag1, 0));
    ASSERT_EQ(tlv1.getValue(), value);

    /* Length octets variant */
    TLV tlv2({0x84, 0x81, 0x05, 0x00, 0x11, 0x22, 0x33, 0x44});

    ASSERT_TRUE(tlv2.parse(tag1, 0));
    ASSERT_EQ(tlv2.getValue(), value);
}

TEST(TLVTest, getPosition)
{
    const std::vector<uint8_t> value1 = {0x00, 0x11, 0x22, 0x33, 0x44};
    const std::vector<uint8_t> value2 = {0x55, 0x66, 0x77, 0x88, 0x99};
    std::shared_ptr<Tag> tag1 =
        std::make_shared<Tag>(0x04, Tag::TagClass::CONTEXT,
                              Tag::TagType::PRIMITIVE, 1);

    /* Two TLV */
    TLV tlv({0x84, 0x05, 0x00, 0x11, 0x22, 0x33, 0x44,
             0x84, 0x05, 0x55, 0x66, 0x77, 0x88, 0x99});

    ASSERT_TRUE(tlv.parse(tag1, 0));

    /* Test position before getValue() */
    ASSERT_EQ(tlv.getPosition(), 2);
    ASSERT_EQ(tlv.getValue(), value1);

    /* Test position after getValue() */
    ASSERT_EQ(tlv.getPosition(), 7);
    ASSERT_TRUE(tlv.parse(tag1, tlv.getPosition()));

    /* Test position before getValue() */
    ASSERT_EQ(tlv.getPosition(), 9);
    ASSERT_EQ(tlv.getValue(), value2);

    /* Test position after getValue() */
    ASSERT_EQ(tlv.getPosition(), 14);
}

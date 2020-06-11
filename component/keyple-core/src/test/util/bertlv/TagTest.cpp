/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
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

#include "Tag.h"

using namespace testing;

using namespace keyple::core::util::bertlv;

TEST(TagTest, Tag)
{
    Tag tag1(0x55, Tag::TagClass::APPLICATION, Tag::TagType::CONSTRUCTED);
    
    Tag tag2({0x55, 0x05, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55}, 0);
}

TEST(TagTest, getTagNumber)
{
    Tag tag1(0x55, Tag::TagClass::APPLICATION, Tag::TagType::PRIMITIVE);
    
    ASSERT_EQ(tag1.getTagNumber(), 0x55);

    Tag tag2({0xBF, 0x55, 0x01, 0x00}, 0);

    ASSERT_EQ(tag2.getTagNumber(), 0x55);
}

TEST(Tag, getTagClass)
{
    Tag tag1(0x55, Tag::TagClass::UNIVERSAL, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag1.getTagClass(), Tag::TagClass::UNIVERSAL);

    Tag tag2(0x55, Tag::TagClass::APPLICATION, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag2.getTagClass(), Tag::TagClass::APPLICATION);

    Tag tag3(0x55, Tag::TagClass::CONTEXT, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag3.getTagClass(), Tag::TagClass::CONTEXT);

    Tag tag4(0x55, Tag::TagClass::PRIVATE, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag4.getTagClass(), Tag::TagClass::PRIVATE);

    Tag tag5({0x1F, 0x55, 0x01, 0x00}, 0);
    ASSERT_EQ(tag5.getTagClass(), Tag::TagClass::UNIVERSAL);

    Tag tag6({0x5F, 0x55, 0x01, 0x00}, 0);
    ASSERT_EQ(tag6.getTagClass(), Tag::TagClass::APPLICATION);

    Tag tag7({0xBF, 0x55, 0x01, 0x00}, 0);
    ASSERT_EQ(tag7.getTagClass(), Tag::TagClass::CONTEXT);

    Tag tag8({0xDF, 0x55, 0x01, 0x00}, 0);
    ASSERT_EQ(tag8.getTagClass(), Tag::TagClass::PRIVATE);
}

TEST(Tag, getTagClass_Bad1)
{
    EXPECT_THROW(Tag(0x55, static_cast<Tag::TagClass>(10),
                     Tag::TagType::PRIMITIVE),
                 std::invalid_argument);
}

TEST(Tag, getTagClass_Bad2)
{
    EXPECT_THROW(Tag(0x55, static_cast<Tag::TagClass>(-10),
                     Tag::TagType::PRIMITIVE),
                 std::invalid_argument);
}

TEST(Tag, getTagType)
{
    Tag tag1(0x55, Tag::TagClass::UNIVERSAL, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag1.getTagType(), Tag::TagType::PRIMITIVE);

    Tag tag2(0x55, Tag::TagClass::APPLICATION, Tag::TagType::CONSTRUCTED);
    ASSERT_EQ(tag2.getTagType(), Tag::TagType::CONSTRUCTED);

    Tag tag3({0x1F, 0x55, 0x01, 0x00}, 0);
    ASSERT_EQ(tag3.getTagType(), Tag::TagType::PRIMITIVE);

    Tag tag4({0x3F, 0x55, 0x01, 0x00}, 0);
    ASSERT_EQ(tag4.getTagType(), Tag::TagType::CONSTRUCTED);
}

TEST(Tag, getSize)
{
    Tag tag1(0x15, Tag::TagClass::UNIVERSAL, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag1.getSize(), 1);

    Tag tag2(0x1E, Tag::TagClass::UNIVERSAL, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag2.getSize(), 1);

    Tag tag3(0x1F, Tag::TagClass::UNIVERSAL, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag3.getSize(), 2);

    Tag tag4(0x7F, Tag::TagClass::UNIVERSAL, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag4.getSize(), 2);

    Tag tag5(0x80, Tag::TagClass::UNIVERSAL, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag5.getSize(), 3);

    Tag tag6(0x3FFF, Tag::TagClass::UNIVERSAL, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag6.getSize(), 3);

    Tag tag7(0x4000, Tag::TagClass::UNIVERSAL, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag7.getSize(), 4);

    Tag tag8(0x1FFFFF, Tag::TagClass::UNIVERSAL, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag8.getSize(), 4);

    Tag tag9(0x200000, Tag::TagClass::UNIVERSAL, Tag::TagType::PRIMITIVE);
    ASSERT_EQ(tag9.getSize(), 5);
}

TEST(Tag, equals)
{
    Tag tag1(0x55, Tag::TagClass::UNIVERSAL, Tag::TagType::PRIMITIVE);
    Tag tag2({0x1F, 0x55, 0x01, 0x00}, 0);
    Tag tag3({0x6F, 0x55, 0x01, 0x00}, 0);
    ASSERT_EQ(tag1, tag2);
    ASSERT_NE(tag1, tag3);
    ASSERT_NE(tag2, tag3);
}

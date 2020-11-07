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

#include "FileData.h"

/* Common */
#include "IllegalArgumentException.h"
#include "IndexOutOfBoundsException.h"
#include "NoSuchElementException.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::transaction;
using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::util;

static std::vector<uint8_t> data1 = ByteArrayUtil::fromHex("11");
static std::vector<uint8_t> data2 = ByteArrayUtil::fromHex("2222");
static std::vector<uint8_t> data3 = ByteArrayUtil::fromHex("333333");
static std::vector<uint8_t> data4 = ByteArrayUtil::fromHex("44444444");

TEST(FileDataTest, etAllRecordsContent_whenNoContent_shouldReturnNotNullObject)
{
    FileData file;

    /*
     * C++ vs. Java: The returned value is a map, therefore cannot be null.
     *               Let's just check the size is 0 as should be on a new
     *               object.
     */
    ASSERT_EQ(file.getAllRecordsContent().size(), 0);
}

TEST(FileDataTest, getAllRecordsContent_shouldReturnAReference)
{
    FileData file;
    file.setContent(1, data1);

    const std::map<int, std::vector<uint8_t>>& copy1 =
        file.getAllRecordsContent();
    const std::map<int, std::vector<uint8_t>>& copy2 =
        file.getAllRecordsContent();

    ASSERT_EQ(copy1, copy2);
    ASSERT_EQ(copy1.find(1)->second, copy2.find(1)->second);
}

TEST(FileDataTest, getContent_whenRecord1IsNotSet_shouldThrowNSEE)
{
    FileData file;

    EXPECT_THROW(file.getContent(), NoSuchElementException);
}

TEST(FileDataTest,  getContent_shouldReturnAReference)
{
    FileData file;
    file.setContent(1, data1);

    const std::vector<uint8_t> copy = file.getContent();

    ASSERT_EQ(copy, data1);
}

TEST(FileDataTest, getContent_shouldReturnRecord1)
{
    FileData file;
    file.setContent(1, data1);

    const std::vector<uint8_t> copy = file.getContent();

    ASSERT_EQ(copy, data1);
}

TEST(FileDataTest, getContentP1_whenRecordIsNotSet_shouldThrowNSEE)
{
    FileData file;

    EXPECT_THROW(file.getContent(1), NoSuchElementException);
}

TEST(FileDataTest, getContentP1_shouldReturnAReference)
{
    FileData file;
    file.setContent(1, data1);

    const std::vector<uint8_t> copy = file.getContent(1);

    ASSERT_EQ(copy, data1);
}

TEST(FileDataTest, getContentP1_shouldReturnRecord)
{
    FileData file;
    file.setContent(1, data1);

    const std::vector<uint8_t> copy = file.getContent(1);

    ASSERT_EQ(copy, data1);
}

TEST(FileDataTest, getContentP3_whenOffsetLt0_shouldThrowIAE)
{
    FileData file;

    EXPECT_THROW(file.getContent(1, -1, 1), IllegalArgumentException);
}

TEST(FileDataTest, getContentP3_whenLengthLt1_shouldThrowIAE)
{
    FileData file;

    EXPECT_THROW(file.getContent(1, 0, 0), IllegalArgumentException);
}

TEST(FileDataTest, getContentP3_whenRecordIsNotSet_shouldThrowNSEE)
{
    FileData file;

    EXPECT_THROW(file.getContent(1, 0, 1), NoSuchElementException);
}

TEST(FileDataTest, getContentP3_whenOffsetGeSize_shouldThrowIOOBE)
{
    FileData file;
    file.setContent(1, data1);

    EXPECT_THROW(file.getContent(1, 1, 1), IndexOutOfBoundsException);
}

TEST(FileDataTest, getContentP3_whenOffsetLengthGtSize_shouldThrowIOOBE)
{
    FileData file;
    file.setContent(2, data2);

    EXPECT_THROW(file.getContent(2, 1, 2), IndexOutOfBoundsException);
}

TEST(FileDataTest, getContentP3_shouldReturnACopy)
{
    FileData file;
    file.setContent(1, data1);

    const std::vector<uint8_t> copy = file.getContent(1, 0, 1);

    ASSERT_NE(&copy, &data1);
}

TEST(FileDataTest, getContentP3_shouldReturnASubset)
{
    FileData file;
    file.setContent(2, data2);

    const std::vector<uint8_t> copy = file.getContent(2, 1, 1);

    ASSERT_EQ(copy, ByteArrayUtil::fromHex("22"));
}

TEST(FileDataTest, getContentAsCounterValue_whenNumRecordLt1_shouldThrowIAE)
{
    FileData file;

    EXPECT_THROW(file.getContentAsCounterValue(0), IllegalArgumentException);
}

TEST(FileDataTest, getContentAsCounterValue_whenRecordIsNotSet_shouldThrowNSEE)
{
    FileData file;

    EXPECT_THROW(file.getContentAsCounterValue(1), NoSuchElementException);
}

TEST(FileDataTest, getContentAsCounterValue_whenCounterIsNotSet_shouldThrowNSEE)
{
    FileData file;
    file.setContent(1, data3);

    EXPECT_THROW(file.getContentAsCounterValue(2), NoSuchElementException);
}

TEST(FileDataTest,
     getContentAsCounterValue_whenCounterIsTruncated_shouldThrowIOOBE)
{
    FileData file;
    file.setContent(1, data4);

    EXPECT_THROW(file.getContentAsCounterValue(2), IndexOutOfBoundsException);
}

TEST(FileDataTest, getContentAsCounterValue_shouldReturnCounterValue)
{
    FileData file;
    file.setContent(1, data3);

    int val = file.getContentAsCounterValue(1);

    ASSERT_EQ(val, 0x333333);
}

TEST(FileDataTest, getAllCountersValue_whenRecordIsNotSet_shouldThrowNSEE)
{
    FileData file;

    EXPECT_THROW(file.getAllCountersValue(), NoSuchElementException);
}

TEST(FileDataTest, getAllCountersValue_shouldReturnAllNonTruncatedCounters)
{
    FileData file;
    file.setContent(1, data4);

    const std::map<int, int> counters = file.getAllCountersValue();

    ASSERT_EQ(counters.size(), 1);
    ASSERT_EQ(counters.find(1)->second, 0x444444);
}

TEST(FileDataTest, setContentP2_shouldPutAReference)
{
    FileData file;
    file.setContent(1, data1);

    const std::vector<uint8_t> copy = file.getContent(1);

    ASSERT_EQ(copy, data1);
}

TEST(FileDataTest, setContentP2_shouldBeSuccess)
{
    FileData file;
    file.setContent(1, data1);

    const std::vector<uint8_t> val = file.getContent(1);

    ASSERT_EQ(val, data1);
}

TEST(FileDataTest, setContentP2_shouldReplaceExistingContent)
{
    FileData file;
    file.setContent(1, data1);
    file.setContent(1, data2);

    const std::vector<uint8_t> val = file.getContent(1);

    ASSERT_EQ(val, data2);
}

TEST(FileDataTest, setCounter_whenRecord1IsNotSet_shouldCreateRecord1)
{
    FileData file;
    file.setCounter(1, data3);

    const std::vector<uint8_t> val = file.getContent(1);

    /*
     * C++ vs. Java: getContent returns a reference therefore cannot be null.
     *               Let's just check the size is exactly 3 here (data3 is 3
     *               bytes long)
     */
    ASSERT_EQ(val.size(), 3);
}

TEST(FileDataTest, setCounter_shouldPutACopy)
{
    FileData file;
    file.setCounter(1, data3);

    const std::vector<uint8_t> copy = file.getContent(1);

    ASSERT_NE(&copy, &data3);
}

TEST(FileDataTest, setCounter_shouldSetOrReplaceCounterValue)
{
    FileData file;
    file.setContent(1, data4);
    file.setCounter(2, data3);

    const std::vector<uint8_t> val = file.getContent(1);

    ASSERT_EQ(val, ByteArrayUtil::fromHex("444444333333"));
}

TEST(FileDataTest, setContentP3_shouldPutACopy)
{
    FileData file;
    file.setContent(1, data1, 0);

    const std::vector<uint8_t> copy = file.getContent(1);

    ASSERT_NE(&copy, &data1);
}

TEST(FileDataTest, setContentP3_whenRecordIsNotSet_shouldPadWith0)
{
    FileData file;
    file.setContent(1, data1, 1);

    const std::vector<uint8_t> val = file.getContent(1);

    ASSERT_EQ(val, ByteArrayUtil::fromHex("0011"));
}

TEST(FileDataTest, setContentP3_whenOffsetGeSize_shouldPadWith0)
{
    FileData file;
    file.setContent(1, data1);
    file.setContent(1, data2, 2);

    const std::vector<uint8_t> val = file.getContent(1);

    ASSERT_EQ(val, ByteArrayUtil::fromHex("11002222"));
}

TEST(FileDataTest, setContentP3_shouldReplaceInRange)
{
    FileData file;
    file.setContent(1, data4);
    file.setContent(1, data2, 1);

    const std::vector<uint8_t> val = file.getContent(1);

    ASSERT_EQ(val, ByteArrayUtil::fromHex("44222244"));
}

TEST(FileDataTest, fillContent_whenRecordIsNotSet_shouldPutContent)
{
    FileData file;
    file.fillContent(1, data1);

    const std::vector<uint8_t>content = file.getContent(1);

    ASSERT_EQ(content, data1);
}

TEST(FileDataTest,
     fillContent_whenLengthGtActualSize_shouldApplyBinaryOperationAndRightPadWithContent)
{
    FileData file;
    file.setContent(1, data1);
    file.fillContent(1, data2);

    const std::vector<uint8_t>content = file.getContent(1);

    ASSERT_EQ(content, ByteArrayUtil::fromHex("3322"));
}

TEST(FileDataTest,
     fillContent_whenLengthLeActualSize_shouldApplyBinaryOperation)
{
    FileData file;
    file.setContent(1, data2);
    file.fillContent(1, data1);

    const std::vector<uint8_t>content = file.getContent(1);

    ASSERT_EQ(content, ByteArrayUtil::fromHex("3322"));
}

TEST(FileDataTest, addCyclicContent_whenNoContent_shouldSetContentToRecord1)
{
    FileData file;
    file.addCyclicContent(data1);

    const std::vector<uint8_t> val = file.getContent(1);

    ASSERT_EQ(val, data1);
}

TEST(FileDataTest,
     addCyclicContent_shouldShiftAllRecordsAndSetContentToRecord1)
{
    /* Reset data2 as it was previously altered */
    data2[0] = 0x22;

    FileData file;
    file.setContent(1, data1);
    file.setContent(2, data2);
    file.addCyclicContent(data3);

    ASSERT_EQ(file.getAllRecordsContent().size(), 3);
    ASSERT_EQ(file.getAllRecordsContent().find(1)->second,
              ByteArrayUtil::fromHex("333333"));
    ASSERT_EQ(file.getAllRecordsContent().find(2)->second,
              ByteArrayUtil::fromHex("11"));
    ASSERT_EQ(file.getAllRecordsContent().find(3)->second,
              ByteArrayUtil::fromHex("2222"));
}

TEST(FileDataTest, clone_shouldReturnACopy)
{
    FileData file;
    file.setContent(1, data1);

    FileData clone(file);

    ASSERT_NE(&clone, &file);
    ASSERT_NE(&clone.getContent(1), &file.getContent(1));
}

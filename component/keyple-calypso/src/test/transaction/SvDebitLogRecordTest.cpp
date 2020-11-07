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

#include "SvDebitLogRecord.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::transaction;
using namespace keyple::core::util;

static const std::string HEADER = "79007013DE31A75F00001A";
static const std::string AMOUNT_STR = "FFFE";
static const std::string DATE_STR = "1234";
static const std::string TIME_STR = "5678";
static const std::string KVC_STR = "90";
static const std::string SAMID_STR = "AABBCCDD";
static const std::string SAM_TNUM_STR = "112233";
static const std::string BALANCE_STR = "445566";
static const std::string SV_TNUM_STR = "7788";

static const int AMOUNT = -2;
static const int DATE = 0x1234;
static const std::vector<uint8_t> DATE_BYTES = ByteArrayUtil::fromHex(DATE_STR);
static const int TIME = 0x5678;
static const std::vector<uint8_t> TIME_BYTES = ByteArrayUtil::fromHex(TIME_STR);
static const uint8_t KVC = 0x90;
static const int SAMID = 0xAABBCCDD;
static const std::vector<uint8_t> SAMID_BYTES = ByteArrayUtil::fromHex(SAMID_STR);
static const int SAM_TNUM = 0x112233;
static const std::vector<uint8_t> SAM_TNUM_BYTES = ByteArrayUtil::fromHex(SAM_TNUM_STR);
static const int BALANCE = 0x445566;
static const int SV_TNUM = 0x7788;
static const std::vector<uint8_t> SV_TNUM_BYTES = ByteArrayUtil::fromHex(SV_TNUM_STR);

TEST(SvDebitLogRecordTest, svDebitLogRecord)
{
    const std::vector<uint8_t> svGetDebitData =
        ByteArrayUtil::fromHex(HEADER + AMOUNT_STR + DATE_STR + TIME_STR + KVC_STR +
                               SAMID_STR + SAM_TNUM_STR + BALANCE_STR + SV_TNUM_STR);

    SvDebitLogRecord svDebitLogRecord(svGetDebitData, HEADER.size() / 2);

    ASSERT_EQ(svDebitLogRecord.getAmount(), AMOUNT);
    ASSERT_EQ(svDebitLogRecord.getBalance(), BALANCE);
    ASSERT_EQ(svDebitLogRecord.getDebitDate(), DATE);
    ASSERT_EQ(svDebitLogRecord.getDebitDateBytes(), DATE_BYTES);
    ASSERT_EQ(svDebitLogRecord.getDebitTime(), TIME);
    ASSERT_EQ(svDebitLogRecord.getDebitTimeBytes(), TIME_BYTES);
    ASSERT_EQ(svDebitLogRecord.getKvc(), KVC);
    ASSERT_EQ(svDebitLogRecord.getSamId(), SAMID);
    ASSERT_EQ(svDebitLogRecord.getSamIdBytes(), SAMID_BYTES);
    ASSERT_EQ(svDebitLogRecord.getSamTNum(), SAM_TNUM);
    ASSERT_EQ(svDebitLogRecord.getSamTNumBytes(), SAM_TNUM_BYTES);
    ASSERT_EQ(svDebitLogRecord.getSvTNum(), SV_TNUM);
    ASSERT_EQ(svDebitLogRecord.getSvTNumBytes(), SV_TNUM_BYTES);
    EXPECT_THAT(svDebitLogRecord.toString(), HasSubstr(SAMID_STR));
}

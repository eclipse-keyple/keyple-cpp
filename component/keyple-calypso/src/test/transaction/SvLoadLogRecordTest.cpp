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

#include "SvLoadLogRecord.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::transaction;
using namespace keyple::core::util;

static const std::string HEADER = "79007013DE31A75F00001A";
static const std::string AMOUNT_STR = "FFFFFE";
static const std::string DATE_STR = "1234";
static const std::string TIME_STR = "5678";
static const std::string FREE1_STR = "41";
static const std::string FREE2_STR = "42";
static const std::string FREE = "AB";
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
static const std::vector<uint8_t>  FREE_BYTES = ByteArrayUtil::fromHex(FREE1_STR + FREE2_STR);
static const uint8_t KVC = 0x90;
static const uint32_t SAMID = 0xAABBCCDD;
static const std::vector<uint8_t> SAMID_BYTES = ByteArrayUtil::fromHex(SAMID_STR);
static const int SAM_TNUM = 0x112233;
static const std::vector<uint8_t> SAM_TNUM_BYTES = ByteArrayUtil::fromHex(SAM_TNUM_STR);
static const int BALANCE = 0x445566;
static const int SV_TNUM = 0x7788;
static const std::vector<uint8_t> SV_TNUM_BYTES = ByteArrayUtil::fromHex(SV_TNUM_STR);

TEST(SvLoadLogRecordTest, svLoadLogRecord)
{
    const std::vector<uint8_t> svGetLoadData =
        ByteArrayUtil::fromHex(HEADER + DATE_STR + FREE1_STR + KVC_STR + FREE2_STR + BALANCE_STR +
                               AMOUNT_STR + TIME_STR + SAMID_STR + SAM_TNUM_STR + SV_TNUM_STR);

    SvLoadLogRecord svLoadLogRecord(svGetLoadData, HEADER.size() / 2);

    ASSERT_EQ(svLoadLogRecord.getAmount(), AMOUNT);
    ASSERT_EQ(svLoadLogRecord.getBalance(), BALANCE);
    ASSERT_EQ(svLoadLogRecord.getLoadDate(), DATE);
    ASSERT_EQ(svLoadLogRecord.getLoadDateBytes(), DATE_BYTES);
    ASSERT_EQ(svLoadLogRecord.getLoadTime(), TIME);
    ASSERT_EQ(svLoadLogRecord.getLoadTimeBytes(), TIME_BYTES);
    ASSERT_EQ(svLoadLogRecord.getLoadTime(), TIME);
    ASSERT_EQ(svLoadLogRecord.getFreeByte(), FREE);
    ASSERT_EQ(svLoadLogRecord.getFreeByteBytes(), FREE_BYTES);
    ASSERT_EQ(svLoadLogRecord.getKvc(), KVC);
    ASSERT_EQ(svLoadLogRecord.getSamId(), SAMID);
    ASSERT_EQ(svLoadLogRecord.getSamIdBytes(), SAMID_BYTES);
    ASSERT_EQ(svLoadLogRecord.getSamTNum(), SAM_TNUM);
    ASSERT_EQ(svLoadLogRecord.getSamTNumBytes(), SAM_TNUM_BYTES);
    ASSERT_EQ(svLoadLogRecord.getSvTNum(), SV_TNUM);
    ASSERT_EQ(svLoadLogRecord.getSvTNumBytes(), SV_TNUM_BYTES);
    EXPECT_THAT(svLoadLogRecord.toString(), HasSubstr(SAMID_STR));
}

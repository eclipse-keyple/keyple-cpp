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

#include "ReadRecordsRespPars.h"

#include <map>

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleSeCommandException.h"

/* Common */
#include "stringhelper.h"

using namespace testing;

using namespace keyple::calypso::command::po::parser;
using namespace keyple::common;
using namespace keyple::core::util;
using namespace keyple::core::seproxy::exception;

using ReadMode = ReadRecordsCmdBuild::ReadMode;

static const std::string SW1SW2_KO = "6A82";
static const std::string SW1SW2_OK = "9000";
static const std::string REC1 = "112233445566778899AABBCCDDEEFF00";
static const std::string REC2 = "00FFEEDDCCBBAA998877665544332211";
static const std::string APDU_BAD_SW1SW2 = SW1SW2_KO;
static const std::string APDU_ONE_RECORD = REC1 + SW1SW2_OK;
static const std::string APDU_TWO_RECORDS =
    "01" +
    StringHelper::uint8ToHexString(REC1.length() / 2) +
    REC1 +
    "02" +
    StringHelper::uint8ToHexString(REC2.length() / 2) +
    REC2 +
    SW1SW2_OK;
static const int SFI1 = 1;
static const int FIRST_REC1 = 1;
static const int EXPECTED_LENGTH1 = REC1.length();
static const int EXPECTED_LENGTH2 = (REC1.length() + REC2.length()) / 2;

TEST(ReadRecordsRespParsTest, readRecordRespPars_badStatus)
{
    ReadRecordsRespPars readRecordRespPars(
        std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(APDU_BAD_SW1SW2),
                                       nullptr),
        nullptr);

    EXPECT_THROW(readRecordRespPars.checkStatus(), KeypleSeCommandException);
}

TEST(ReadRecordsRespParsTest, readRecordRespPars_goodStatus)
{
    ReadRecordsRespPars readRecordRespPars(
        std::make_shared<ApduResponse>(ByteArrayUtil::fromHex(APDU_ONE_RECORD),
                                       nullptr),
        nullptr);

    readRecordRespPars.checkStatus();
}

TEST(ReadRecordsRespParsTest, readRecordRespPars_getRecords_singleRecord)
{
    ReadRecordsCmdBuild readRecordsCmdBuild(PoClass::ISO,
                                            SFI1,
                                            FIRST_REC1,
                                            ReadMode::ONE_RECORD,
                                            EXPECTED_LENGTH1);

    std::shared_ptr<ReadRecordsRespPars> readRecordRespPars =
        readRecordsCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(
                ByteArrayUtil::fromHex(APDU_ONE_RECORD), nullptr));

    readRecordRespPars->checkStatus();

    std::map<int, std::vector<uint8_t>> records =
        readRecordRespPars->getRecords();

    ASSERT_EQ(static_cast<int>(records.size()), 1);
    ASSERT_EQ(records[FIRST_REC1], ByteArrayUtil::fromHex(REC1));
}

TEST(ReadRecordsRespParsTest, readRecordRespPars_getRecords_twoRecords)
{
    ReadRecordsCmdBuild readRecordsCmdBuild(PoClass::ISO,
                                            SFI1,
                                            FIRST_REC1,
                                            ReadMode::MULTIPLE_RECORD,
                                            EXPECTED_LENGTH2);

    std::shared_ptr<ReadRecordsRespPars>  readRecordRespPars =
        readRecordsCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(
                ByteArrayUtil::fromHex(APDU_TWO_RECORDS), nullptr));

    readRecordRespPars->checkStatus();

    std::map<int, std::vector<uint8_t>> records =
        readRecordRespPars->getRecords();

    ASSERT_EQ(static_cast<int>(records.size()), 2);
    ASSERT_EQ(records[FIRST_REC1], ByteArrayUtil::fromHex(REC1));
    ASSERT_EQ(records[FIRST_REC1 + 1], ByteArrayUtil::fromHex(REC2));
}

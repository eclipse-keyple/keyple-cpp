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

#include "ReadRecordsCmdBuild.h"

/* Calypso */
#include "PoClass.h"

using namespace testing;

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po::builder;

using ReadMode = ReadRecordsCmdBuild::ReadMode;

static const uint8_t record_number = 0x01;
static const uint8_t expectedLength = 0x00;

TEST(ReadRecordsCmdBuildTest, readRecords_rev2_4)
{
    const uint8_t cla = 0x94;
    const uint8_t cmd = 0xB2;
    const uint8_t sfi = 0x08;
    const uint8_t p2 = (sfi * 8) + 4; /* One record */

    /* Revision 2.4 */
    const std::vector<uint8_t> request2_4 = {cla, cmd, record_number, p2, 0x00};
    ReadRecordsCmdBuild readRecordsCmdBuilder(PoClass::LEGACY,
                                              sfi,
                                              record_number,
                                              ReadMode::ONE_RECORD,
                                              expectedLength);

    std::shared_ptr<ApduRequest> apduRequest =
        readRecordsCmdBuilder.getApduRequest();

    ASSERT_EQ(apduRequest->getBytes(), request2_4);
    ASSERT_EQ(readRecordsCmdBuilder.getReadMode(), ReadMode::ONE_RECORD);
}

TEST(ReadRecordsCmdBuildTest, readRecords_rev2_4_2)
{
    const uint8_t cla = 0x94;
    const uint8_t cmd = 0xB2;
    const uint8_t sfi = 0x08;
    const uint8_t p2 = (sfi * 8 + 5); /* All records */

    // revision 2.4
    const std::vector<uint8_t> request2_4 = {cla, cmd, record_number, p2, 0x00};
    ReadRecordsCmdBuild readRecordsCmdBuilder(PoClass::LEGACY,
                                              sfi,
                                              record_number,
                                              ReadMode::MULTIPLE_RECORD,
                                              expectedLength);

    std::shared_ptr<ApduRequest> apduRequest =
        readRecordsCmdBuilder.getApduRequest();

    ASSERT_EQ(apduRequest->getBytes(), request2_4);
    ASSERT_EQ(readRecordsCmdBuilder.getReadMode(), ReadMode::MULTIPLE_RECORD);
}

TEST(ReadRecordsCmdBuildTest, readRecords_rev3_1)
{
    const uint8_t cla = 0x00;
    const uint8_t cmd = 0xB2;
    const uint8_t sfi = 0x08;
    const uint8_t p2 =  sfi * 8 + 5; /* All records */

    /* Revision 3.1 */
    const std::vector<uint8_t> request3_1 = {cla, cmd, record_number, p2, 0x00};
    ReadRecordsCmdBuild readRecordsCmdBuilder(PoClass::ISO,
                                              sfi,
                                              record_number,
                                              ReadMode::MULTIPLE_RECORD,
                                              expectedLength);

    std::shared_ptr<ApduRequest> apduRequest =
        readRecordsCmdBuilder.getApduRequest();

    ASSERT_EQ(apduRequest->getBytes(), request3_1);
    ASSERT_EQ(readRecordsCmdBuilder.getReadMode(), ReadMode::MULTIPLE_RECORD);
}

TEST(ReadRecordsCmdBuildTest, readRecords_rev3_2)
{
    const uint8_t cla = 0x00;
    const uint8_t cmd = 0xB2;
    const uint8_t sfi = 0x08;
    const uint8_t p2 = sfi * 8 + 5; /* All records */

    /* Revision 3.2 */
    const std::vector<uint8_t> request3_2 = {cla, cmd, record_number, p2, 0x00};
    ReadRecordsCmdBuild readRecordsCmdBuilder(PoClass::ISO,
                                              sfi,
                                              record_number,
                                              ReadMode::MULTIPLE_RECORD,
                                              expectedLength);
    std::shared_ptr<ApduRequest> apduRequest =
        readRecordsCmdBuilder.getApduRequest();

    ASSERT_EQ(apduRequest->getBytes(), request3_2);
    ASSERT_EQ(readRecordsCmdBuilder.getReadMode(), ReadMode::MULTIPLE_RECORD);
}

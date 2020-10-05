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

#include "UpdateRecordCmdBuild.h"

using namespace testing;

using namespace keyple::calypso::command::po::builder;

static const uint8_t record_number = 0x01;
static const std::vector<uint8_t> newRecordData = {0x00,0x01,0x02,0x03,0x04};

TEST(UpdateRecordCmdBuildTest, updateRecordCmdBuild_rev2_4)
{
    /* Revision 2.4 */
    const std::vector<uint8_t> request2_4 = {
        0x94, 0xDC, 0x01,0x44, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04};

    UpdateRecordCmdBuild apduCommandBuilder(PoClass::LEGACY,
                                            0x08,
                                            record_number,
                                            newRecordData);

    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder.getApduRequest();

    ASSERT_EQ(apduRequest->getBytes(), request2_4);
}

TEST(UpdateRecordCmdBuildTest, updateRecordCmdBuild_rev3_1)
{
    /* Revision 3.1 */
    const std::vector<uint8_t> request3_1 = {
        0x00, 0xDC, 0x01,0x44, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04};

    UpdateRecordCmdBuild apduCommandBuilder(PoClass::ISO,
                                            0x08,
                                            record_number,
                                            newRecordData);

    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder.getApduRequest();

    ASSERT_EQ(apduRequest->getBytes(), request3_1);
}

TEST(UpdateRecordCmdBuildTest, updateRecordCmdBuild_rev3_2)
{
    /* Revision 3.2 */
    const std::vector<uint8_t> request3_2 = {
        0x00, 0xDC, 0x01,0x44, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04};

    UpdateRecordCmdBuild apduCommandBuilder(PoClass::ISO,
                                            0x08,
                                            record_number,
                                            newRecordData);

    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder.getApduRequest();

    ASSERT_EQ(apduRequest->getBytes(), request3_2);
}

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

#include "UpdateRecordCmdBuildTest.h"

using namespace keyple::calypso::command::po::builder;

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
using PoClass = keyple::calypso::command::PoClass;
using AbstractApduCommandBuilder =
    keyple::core::command::AbstractApduCommandBuilder;
using ApduRequest = keyple::core::seproxy::message::ApduRequest;

void UpdateRecordCmdBuildTest::updateRecordCmdBuild_rev2_4()
{

    // revision 2.4
    std::vector<uint8_t> request2_4 = {
        0x94, 0xDC, 0x01,0x44, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04};

    apduCommandBuilder  = std::make_shared<UpdateRecordCmdBuild>(
        keyple::calypso::command::PoClass::LEGACY, 0x08, record_number,
        newRecordData, "TestRev2_4");

    apduRequest = apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request2_4, apduRequest->getBytes());
}

void UpdateRecordCmdBuildTest::updateRecordCmdBuild_rev3_1()
{
    // revision 3.1
    std::vector<uint8_t> request3_1 = {
        0x00, 0xDC, 0x01,0x44, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04};

    apduCommandBuilder = std::make_shared<UpdateRecordCmdBuild>(
        keyple::calypso::command::PoClass::ISO, 0x08, record_number,
        newRecordData, "TestRev3_1");

    apduRequest = apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request3_1, apduRequest->getBytes());
}

void UpdateRecordCmdBuildTest::updateRecordCmdBuild_rev3_2()
{
    // revision 3.2
    std::vector<uint8_t> request3_2 = {
        0x00, 0xDC, 0x01,0x44, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04};

    apduCommandBuilder = std::make_shared<UpdateRecordCmdBuild>(
        keyple::calypso::command::PoClass::ISO, 0x08, record_number,
        newRecordData, "TestRev3_2");

    apduRequest = apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request3_2, apduRequest->getBytes());
}
}
}
}
}
}

TEST(UpdateRecordCmdBuildTest, testA)
{
    std::shared_ptr<UpdateRecordCmdBuildTest> LocalTest =
        std::make_shared<UpdateRecordCmdBuildTest>();
    LocalTest->updateRecordCmdBuild_rev2_4();
}

TEST(UpdateRecordCmdBuildTest, testB)
{
    std::shared_ptr<UpdateRecordCmdBuildTest> LocalTest =
        std::make_shared<UpdateRecordCmdBuildTest>();
    LocalTest->updateRecordCmdBuild_rev3_1();
}

TEST(UpdateRecordCmdBuildTest, testC)
{
    std::shared_ptr<UpdateRecordCmdBuildTest> LocalTest =
        std::make_shared<UpdateRecordCmdBuildTest>();
    LocalTest->updateRecordCmdBuild_rev3_2();
}

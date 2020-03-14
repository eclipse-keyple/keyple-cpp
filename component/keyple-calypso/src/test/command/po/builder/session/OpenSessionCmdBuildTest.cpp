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

#include "OpenSessionCmdBuildTest.h"

using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::calypso::command::po::parser::security;

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using PoRevision = keyple::calypso::command::po::PoRevision;
using AbstractApduCommandBuilder =
    keyple::core::command::AbstractApduCommandBuilder;
using ApduRequest   = keyple::core::seproxy::message::ApduRequest;
using ByteArrayUtil = keyple::core::util::ByteArrayUtil;

void OpenSessionCmdBuildTest::openSessionCmdBuild_rev_2_4_exception()
{
    uint8_t keyIndex           = 0x00;
    uint8_t recordNumberToRead = 0x01;
    uint8_t sfiToSelect        = 0x08;

    try {
        apduCommandBuilder =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                PoRevision::REV2_4, keyIndex, samChallenge, sfiToSelect,
                recordNumberToRead, "");
    } catch (...) {
    }
}

void OpenSessionCmdBuildTest::openSessionCmdBuild_rev_2_4()
{
    uint8_t keyIndex            = 0x03;
    uint8_t recordNumberToRead  = 0x01;
    uint8_t sfiToSelect         = 0x08;
    uint8_t cla                 = 0x94;
    uint8_t p1                  = 0x80 + (recordNumberToRead * 8) + keyIndex;
    uint8_t p2                  = sfiToSelect * 8;
    uint8_t cmd                 = 0x8A;
    std::vector<uint8_t> dataIn = samChallenge;
    // revision 2.4
    uint8_t le                  = 0; // case 4

    std::vector<uint8_t> request2_4 = {
        cla, cmd, p1, p2, static_cast<uint8_t>(dataIn.size()), 0xA8, 0x31, 0xC3,
        0x3E, le};

    apduCommandBuilder =
        AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
            PoRevision::REV2_4, keyIndex, dataIn, sfiToSelect,
            recordNumberToRead, "");

    apduRequest = apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request2_4, apduRequest->getBytes());
}

void OpenSessionCmdBuildTest::openSessionCmdBuild_rev_3_1()
{
    uint8_t keyIndex            = 0x03;
    uint8_t recordNumberToRead  = 0x01;
    uint8_t sfiToSelect         = 0x08;
    uint8_t cla                 = 0x00;
    uint8_t p1                  = (recordNumberToRead * 8) + keyIndex;
    uint8_t p2                  = (sfiToSelect * 8) + 1;
    uint8_t cmd                 = 0x8A;
    std::vector<uint8_t> dataIn = samChallenge;
    uint8_t le                  = 0; // case 4

    // revision 3.1
    std::vector<uint8_t> request3_1 = {
        cla, cmd, p1, p2, static_cast<uint8_t>(dataIn.size()), 0xA8, 0x31, 0xC3,
        0x3E, le};

    apduCommandBuilder =
        AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
            PoRevision::REV3_1, keyIndex, dataIn, sfiToSelect,
            recordNumberToRead, "");

    apduRequest = apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request3_1, apduRequest->getBytes());
}

void OpenSessionCmdBuildTest::openSessionCmdBuild_rev_3_2()
{
    uint8_t keyIndex           = 0x03;
    uint8_t recordNumberToRead = 0x01;
    uint8_t sfiToSelect        = 0x08;
    uint8_t cla                = 0x00;
    uint8_t p1                 = (recordNumberToRead * 8) + keyIndex;
    uint8_t p2                 = (sfiToSelect * 8) + 2;
    uint8_t cmd                = 0x8A;
    std::vector<uint8_t> dataIn(samChallenge.size() + 1);
    System::arraycopy(samChallenge, 0, dataIn, 1, samChallenge.size());
    uint8_t le = 0; // case 4
    // revision 3.2
    std::vector<uint8_t> request3_2 = {
        cla, cmd, p1, p2, static_cast<uint8_t>((samChallenge.size()) + 1),
        0x00, 0xA8, 0x31, 0xC3, 0x3E, le};

    apduCommandBuilder =
        AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
            PoRevision::REV3_2, keyIndex, samChallenge, sfiToSelect,
            recordNumberToRead, "");

    apduRequest = apduCommandBuilder->getApduRequest();

    ASSERT_EQ(ByteArrayUtil::toHex(request3_2),
              ByteArrayUtil::toHex(apduRequest->getBytes()));
}
}
}
}
}
}
}

TEST(OpenSessionCmdBuildTest, testA)
{
    std::shared_ptr<OpenSessionCmdBuildTest> LocalTest =
        std::make_shared<OpenSessionCmdBuildTest>();
    LocalTest->openSessionCmdBuild_rev_2_4_exception();
}

TEST(OpenSessionCmdBuildTest, testB)
{
    std::shared_ptr<OpenSessionCmdBuildTest> LocalTest =
        std::make_shared<OpenSessionCmdBuildTest>();
    LocalTest->openSessionCmdBuild_rev_2_4();
}

TEST(OpenSessionCmdBuildTest, testC)
{
    std::shared_ptr<OpenSessionCmdBuildTest> LocalTest =
        std::make_shared<OpenSessionCmdBuildTest>();
    LocalTest->openSessionCmdBuild_rev_3_1();
}

TEST(OpenSessionCmdBuildTest, testD)
{
    std::shared_ptr<OpenSessionCmdBuildTest> LocalTest =
        std::make_shared<OpenSessionCmdBuildTest>();
    LocalTest->openSessionCmdBuild_rev_3_2();
}

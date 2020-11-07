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

/* Calypso */
#include "AbstractOpenSessionCmdBuild.h"
#include "AbstractOpenSessionRespPars.h"
#include "OpenSession10CmdBuild.h"
#include "OpenSession24CmdBuild.h"
#include "OpenSession31CmdBuild.h"
#include "OpenSession32CmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"
#include "System.h"

/* Core */
#include "AbstractApduCommandBuilder.h"
#include "AbstractApduResponseParser.h"

using namespace testing;

using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::calypso::command::po::parser::security;
using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::command;

static const std::vector<uint8_t> samChallenge = {0xA8, 0x31, 0xC3, 0x3E};

TEST(OpenSessionCmdBuildTest, openSessionCmdBuild_rev_2_4_exception)
{
    const uint8_t keyIndex           = 0x00;
    const uint8_t recordNumberToRead = 0x01;
    const uint8_t sfiToSelect        = 0x08;

    EXPECT_THROW(AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>
                     ::create(PoRevision::REV2_4,
                              keyIndex,
                              samChallenge,
                              sfiToSelect,
                              recordNumberToRead),
                 IllegalArgumentException);
}

TEST(OpenSessionCmdBuildTest, openSessionCmdBuild_rev_2_4)
{
    const uint8_t keyIndex            = 0x03;
    const uint8_t recordNumberToRead  = 0x01;
    const uint8_t sfiToSelect         = 0x08;
    const uint8_t cla                 = 0x94;
    const uint8_t p1                  = 0x80 + (recordNumberToRead * 8) + keyIndex;
    const uint8_t p2                  = sfiToSelect * 8;
    const uint8_t cmd                 = 0x8A;
    const std::vector<uint8_t>& dataIn = samChallenge;
    // revision 2.4
    const uint8_t le                  = 0; // case 4

    std::vector<uint8_t> request2_4 = {
        cla, cmd, p1, p2, static_cast<uint8_t>(dataIn.size()), 0xA8, 0x31, 0xC3,
        0x3E, le};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
            PoRevision::REV2_4, keyIndex, dataIn, sfiToSelect,
            recordNumberToRead);

    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request2_4, apduRequest->getBytes());
}

TEST(OpenSessionCmdBuildTest, openSessionCmdBuild_rev_3_1)
{
    const uint8_t keyIndex            = 0x03;
    const uint8_t recordNumberToRead  = 0x01;
    const uint8_t sfiToSelect         = 0x08;
    const uint8_t cla                 = 0x00;
    const uint8_t p1                  = (recordNumberToRead * 8) + keyIndex;
    const uint8_t p2                  = (sfiToSelect * 8) + 1;
    const uint8_t cmd                 = 0x8A;
    const std::vector<uint8_t>& dataIn = samChallenge;
    const uint8_t le                  = 0; // case 4

    // revision 3.1
    std::vector<uint8_t> request3_1 = {
        cla, cmd, p1, p2, static_cast<uint8_t>(dataIn.size()), 0xA8, 0x31, 0xC3,
        0x3E, le};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
            PoRevision::REV3_1, keyIndex, dataIn, sfiToSelect,
            recordNumberToRead);

    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request3_1, apduRequest->getBytes());
}

TEST(OpenSessionCmdBuildTest, openSessionCmdBuild_rev_3_2)
{
    const uint8_t keyIndex           = 0x03;
    const uint8_t recordNumberToRead = 0x01;
    const uint8_t sfiToSelect        = 0x08;
    const uint8_t cla                = 0x00;
    const uint8_t p1                 = (recordNumberToRead * 8) + keyIndex;
    const uint8_t p2                 = (sfiToSelect * 8) + 2;
    const uint8_t cmd                = 0x8A;
    std::vector<uint8_t> dataIn(samChallenge.size() + 1);
    System::arraycopy(samChallenge, 0, dataIn, 1, samChallenge.size());
    const uint8_t le = 0; // case 4
    // revision 3.2
    std::vector<uint8_t> request3_2 = {
        cla, cmd, p1, p2, static_cast<uint8_t>((samChallenge.size()) + 1),
        0x00, 0xA8, 0x31, 0xC3, 0x3E, le};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
            PoRevision::REV3_2, keyIndex, samChallenge, sfiToSelect,
            recordNumberToRead);

    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request3_2, apduRequest->getBytes());
}

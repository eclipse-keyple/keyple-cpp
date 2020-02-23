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

#include "PoGetChallengeCmdBuildTest.h"

using namespace keyple::calypso::command::po::builder::security;

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {
using PoClass = keyple::calypso::command::PoClass;
using AbstractApduCommandBuilder =
    keyple::core::command::AbstractApduCommandBuilder;
using ApduRequest = keyple::core::seproxy::message::ApduRequest;

void PoGetChallengeCmdBuildTest::POGetChallenge_Rev2_4()
{

    std::vector<char> request = {static_cast<char>(0x94),
                                 static_cast<char>(0x84), 0x01, 0x10, 0x08};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<PoGetChallengeCmdBuild>(PoClass::LEGACY);
    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request, apduRequest->getBytes());
}

void PoGetChallengeCmdBuildTest::POGetChallenge_Rev3_1()
{

    std::vector<char> request = {static_cast<char>(0x00),
                                 static_cast<char>(0x84), 0x01, 0x10, 0x08};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<PoGetChallengeCmdBuild>(PoClass::ISO);
    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request, apduRequest->getBytes());
}

void PoGetChallengeCmdBuildTest::POGetChallenge_Rev3_2()
{

    std::vector<char> request = {static_cast<char>(0x00),
                                 static_cast<char>(0x84), 0x01, 0x10, 0x08};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<PoGetChallengeCmdBuild>(PoClass::ISO);
    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request, apduRequest->getBytes());
}
}
}
}
}
}
}

TEST(PoGetChallengeCmdBuildTest, testA)
{
    std::shared_ptr<PoGetChallengeCmdBuildTest> LocalTest =
        std::make_shared<PoGetChallengeCmdBuildTest>();
    LocalTest->POGetChallenge_Rev2_4();
}

TEST(PoGetChallengeCmdBuildTest, testB)
{
    std::shared_ptr<PoGetChallengeCmdBuildTest> LocalTest =
        std::make_shared<PoGetChallengeCmdBuildTest>();
    LocalTest->POGetChallenge_Rev3_1();
}

TEST(PoGetChallengeCmdBuildTest, testC)
{
    std::shared_ptr<PoGetChallengeCmdBuildTest> LocalTest =
        std::make_shared<PoGetChallengeCmdBuildTest>();
    LocalTest->POGetChallenge_Rev3_2();
}

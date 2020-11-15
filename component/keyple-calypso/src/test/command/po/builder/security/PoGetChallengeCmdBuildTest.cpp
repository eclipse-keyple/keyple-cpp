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

#include "PoGetChallengeCmdBuild.h"

/* Core */
#include "AbstractApduCommandBuilder.h"

using namespace testing;

using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::core::command;

TEST(PoGetChallengeCmdBuildTest, POGetChallenge_Rev2_4)
{
    const std::vector<uint8_t> request = {0x94, 0x84, 0x00, 0x00, 0x08};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<PoGetChallengeCmdBuild>(PoClass::LEGACY);
    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request, apduRequest->getBytes());
}

TEST(PoGetChallengeCmdBuildTest, POGetChallenge_Rev3_1)
{
    const std::vector<uint8_t> request = {0x00, 0x84, 0x00, 0x00, 0x08};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<PoGetChallengeCmdBuild>(PoClass::ISO);
    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request, apduRequest->getBytes());
}

TEST(PoGetChallengeCmdBuildTest, POGetChallenge_Rev3_2)
{
    const std::vector<uint8_t> request = {0x00, 0x84, 0x00, 0x00, 0x08};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<PoGetChallengeCmdBuild>(PoClass::ISO);
    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request, apduRequest->getBytes());
}

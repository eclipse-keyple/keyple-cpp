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

#include "GetDataFciCmdBuild.h"

using namespace testing;

using namespace keyple::calypso::command::po::builder;

TEST(GetDataFciCmdBuildTest, getDataFCICmdBuild)
{
    const std::vector<uint8_t> request = {0x94, 0xCA, 0x00, 0x6F, 0x00};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<GetDataFciCmdBuild>(PoClass::LEGACY);
    std::shared_ptr<ApduRequest> apduReq = apduCommandBuilder->getApduRequest();

    ASSERT_EQ(apduReq->getBytes(), request);
}

TEST(GetDataFciCmdBuildTest, getDataFCICmdBuild2)
{
    const std::vector<uint8_t> request2 = {0x00, 0xCA, 0x00, 0x6F, 0x00};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<GetDataFciCmdBuild>(PoClass::ISO);
    std::shared_ptr<ApduRequest> apduReq = apduCommandBuilder->getApduRequest();

    ASSERT_EQ(apduReq->getBytes(), request2);
}

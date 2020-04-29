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

#include "CloseSessionCmdBuildTest.h"

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

void CloseSessionCmdBuildTest::closeSessionCmdBuild()
{
    std::vector<uint8_t> request2_4 = {
        0x94, 0x8E, 0x00, 0x00, 0x04, 0xA8, 0x31, 0xC3, 0x3E, 0x00};

    std::vector<uint8_t> request3_1 = {
        0x00, 0x8E, 0x80, 0x00, 0x04, 0xA8, 0x31, 0xC3, 0x3E, 0x00};

    std::vector<uint8_t> terminalSessionSignature = {
        0xA8, 0x31, 0xC3, 0x3E};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<CloseSessionCmdBuild>(PoClass::LEGACY, false,
                                               terminalSessionSignature);

    std::shared_ptr<ApduRequest> reqApdu = apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request2_4, reqApdu->getBytes());

    apduCommandBuilder = std::make_shared<CloseSessionCmdBuild>(
        PoClass::ISO, true, terminalSessionSignature);

    reqApdu = apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request3_1, reqApdu->getBytes());
}
}
}
}
}
}
}

TEST(CloseSessionCmdBuildTest, testA)
{
    std::shared_ptr<CloseSessionCmdBuildTest> LocalTest =
        std::make_shared<CloseSessionCmdBuildTest>();
    LocalTest->closeSessionCmdBuild();
}

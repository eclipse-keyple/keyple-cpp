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

#include "DigestUpdateCmdBuildTest.h"
//#include "SamRevision.h"
#include "DigestUpdateCmdBuild.h"
#include "AbstractApduCommandBuilder.h"
#include "ApduRequest.h"

using namespace keyple::calypso::command::sam::builder::security;

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using SamRevision = keyple::calypso::command::sam::SamRevision;
using DigestUpdateCmdBuild =
    keyple::calypso::command::sam::builder::security::DigestUpdateCmdBuild;
using AbstractApduCommandBuilder =
    keyple::core::command::AbstractApduCommandBuilder;
using ApduRequest = keyple::core::seproxy::message::ApduRequest;

void DigestUpdateCmdBuildTest::digestUpdateCmdBuild()
{
    std::vector<uint8_t> digestDAta = {0x94, 0xAE, 0x01, 0x02};
    std::vector<uint8_t> request = {
        0x94, 0x8C, 0x00, 0x80, static_cast<uint8_t>(digestDAta.size()), 0x94,
        0xAE, 0x01, 0x02};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<DigestUpdateCmdBuild>(SamRevision::S1D, true,
                                               digestDAta);
    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request, apduRequest->getBytes());

    std::vector<uint8_t> request2 = {
        0x80, 0x8C, 0x00, 0x80, static_cast<uint8_t>(digestDAta.size()), 0x94,
        0xAE, 0x01, 0x02};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder2 =
        std::make_shared<DigestUpdateCmdBuild>(SamRevision::C1, true,
                                               digestDAta);
    std::shared_ptr<ApduRequest> apduReq =
        apduCommandBuilder2->getApduRequest();
    ASSERT_EQ(request2, apduReq->getBytes());
}
}
}
}
}
}
}

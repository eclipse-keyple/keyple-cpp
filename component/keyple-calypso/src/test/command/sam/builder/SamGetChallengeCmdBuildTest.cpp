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

#include "SamGetChallengeCmdBuildTest.h"
//#include "SamRevision.h"
#include "SamGetChallengeCmdBuild.h"
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
using SamGetChallengeCmdBuild =
    keyple::calypso::command::sam::builder::security::SamGetChallengeCmdBuild;
using AbstractApduCommandBuilder =
    keyple::core::command::AbstractApduCommandBuilder;
using ApduRequest = keyple::core::seproxy::message::ApduRequest;

void SamGetChallengeCmdBuildTest::getChallengeCmdBuild()
{
    std::vector<char> request = {static_cast<char>(0x94),
                                 static_cast<char>(0x84), 0x00, 0x00, 0x04};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<SamGetChallengeCmdBuild>(
            SamRevision::S1D, static_cast<char>(0x04)); // 94
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

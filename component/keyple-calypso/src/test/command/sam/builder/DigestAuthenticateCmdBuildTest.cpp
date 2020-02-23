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

#include "DigestAuthenticateCmdBuildTest.h"
#include "DigestAuthenticateCmdBuild.h"
#include "AbstractApduCommandBuilder.h"
#include "ApduRequest.h"
#include "ByteArrayUtil.h"

using namespace keyple::calypso::command::sam::builder::security;

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using DigestAuthenticateCmdBuild = keyple::calypso::command::sam::builder::
    security::DigestAuthenticateCmdBuild;
using AbstractApduCommandBuilder =
    keyple::core::command::AbstractApduCommandBuilder;
using ApduRequest    = keyple::core::seproxy::message::ApduRequest;
using ByteArrayUtils = keyple::core::util::ByteArrayUtil;

void DigestAuthenticateCmdBuildTest::digestAuthenticate()
{

    std::vector<char> signaturePO = {0x00, 0x01, 0x02, 0x03};
    std::vector<char> request     = {static_cast<char>(0x94),
                                 static_cast<char>(0x82),
                                 0x00,
                                 0x00,
                                 0x04,
                                 0x00,
                                 0x01,
                                 0x02,
                                 0x03};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<DigestAuthenticateCmdBuild>(SamRevision::S1D,
                                                     signaturePO);
    ASSERT_EQ(ByteArrayUtils::toHex(request),
              ByteArrayUtils::toHex(
                  apduCommandBuilder->getApduRequest()->getBytes()));
}
}
}
}
}
}
}

TEST(DigestAuthenticateCmdBuildTest, testA)
{
    std::shared_ptr<DigestAuthenticateCmdBuildTest> LocalTest =
        std::make_shared<DigestAuthenticateCmdBuildTest>();
    LocalTest->digestAuthenticate();
}

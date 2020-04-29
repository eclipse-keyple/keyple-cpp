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

#include "DigestInitCmdBuildTest.h"
#include "DigestInitCmdBuild.h"
#include "AbstractApduCommandBuilder.h"

using namespace keyple::calypso::command::sam::builder::security;

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using SamRevision = keyple::calypso::command::sam::SamRevision;
using DigestInitCmdBuild =
    keyple::calypso::command::sam::builder::security::DigestInitCmdBuild;
using AbstractApduCommandBuilder =
    keyple::core::command::AbstractApduCommandBuilder;

void DigestInitCmdBuildTest::digestInitCmd_inconsistent()
{

    std::vector<uint8_t> digestData = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

    bool verificationMode    = false;
    bool rev3_2Mode          = false;
    uint8_t workKeyRecordNumber = 0x00;
    uint8_t workKeyKif          = 0x00;
    uint8_t workKeyKVC          = 0x7E;
    SamRevision revision     = SamRevision::S1D;

    try {
        /* code */
        std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
            std::make_shared<DigestInitCmdBuild>(
                revision, verificationMode, rev3_2Mode, workKeyRecordNumber,
                workKeyKif, workKeyKVC, digestData);
    } catch (...) {
    }
}

void DigestInitCmdBuildTest::digestInitCmd_inconsistent_digestNull()
{

    std::vector<uint8_t> digestData;

    bool verificationMode    = false;
    bool rev3_2Mode          = false;
    uint8_t workKeyRecordNumber = 0x10;
    uint8_t workKeyKif          = 0x30;
    uint8_t workKeyKVC          = 0x7E;
    SamRevision revision     = SamRevision::S1D;


    try {
        /* code */
        std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
            std::make_shared<DigestInitCmdBuild>(
                revision, verificationMode, rev3_2Mode, workKeyRecordNumber,
                workKeyKif, workKeyKVC, digestData);
    } catch (...) {
    }
}

void DigestInitCmdBuildTest::digestInitCmd()
{

    std::vector<uint8_t> digestData = {0x80, 0x8A, 0x00};
    uint8_t cla                     = 0x94;
    uint8_t zero                    = 0x00;
    uint8_t p1                      = zero + 1;
    uint8_t p1_2                    = p1 + 2;
    uint8_t p2                      = 0xFF;

    bool verificationMode       = true;
    bool rev3_2Mode             = true;
    uint8_t workKeyRecordNumber = 0xFF;
    uint8_t workKeyKif          = 0x30;
    uint8_t workKeyKVC          = 0x7E;
    SamRevision revision        = SamRevision::S1D;

    int size                  = digestData.size() + 2;
    std::vector<uint8_t> request = {
        cla,        0x8A, p1_2,
        p2,         static_cast<uint8_t>(size), workKeyKif,
        workKeyKVC, 0x80, 0x8A,
        0x00};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<DigestInitCmdBuild>(
            revision, verificationMode, rev3_2Mode, workKeyRecordNumber,
            workKeyKif, workKeyKVC, digestData);

    ASSERT_EQ(request, apduCommandBuilder->getApduRequest()->getBytes());
}
}
}
}
}
}
}

TEST(DigestInitCmdBuildTest, testA)
{
    std::shared_ptr<DigestInitCmdBuildTest> LocalTest =
        std::make_shared<DigestInitCmdBuildTest>();
    LocalTest->digestInitCmd_inconsistent();
}

TEST(DigestInitCmdBuildTest, testB)
{
    std::shared_ptr<DigestInitCmdBuildTest> LocalTest =
        std::make_shared<DigestInitCmdBuildTest>();
    LocalTest->digestInitCmd_inconsistent_digestNull();
}

TEST(DigestInitCmdBuildTest, testC)
{
    std::shared_ptr<DigestInitCmdBuildTest> LocalTest =
        std::make_shared<DigestInitCmdBuildTest>();
    LocalTest->digestInitCmd();
}

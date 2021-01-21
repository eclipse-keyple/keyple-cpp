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

#include "DigestAuthenticateCmdBuild.h"

#include <typeinfo>

/* Calypso */
#include "DigestAuthenticateRespPars.h"

/* Common */
#include "IllegalArgumentException.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::common::exception;
using namespace keyple::core::util;

static const std::string SW1SW2_OK = "9000";
static const std::string SIGNATURE_PO = "FEDCBA98";
static const std::string SIGNATURE_PO_BAD_LENGTH = "FEDC";
static const std::string APDU_CLA_80 = "8082000004" + SIGNATURE_PO;
static const std::string APDU_CLA_94 = "9482000004" + SIGNATURE_PO;

TEST(DigestAuthenticateCmdBuildTest,
     digestAuthenticateCmdBuild_defaultRevision_createParser)
{
    DigestAuthenticateCmdBuild digestAuthenticateCmdBuild(
        SamRevision::NO_REV, ByteArrayUtil::fromHex(SIGNATURE_PO));

    ASSERT_EQ(digestAuthenticateCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));

    std::shared_ptr<DigestAuthenticateRespPars> digestAuthenticateRespPars =
        digestAuthenticateCmdBuild.createResponseParser(
            std::make_shared<ApduResponse>(
                ByteArrayUtil::fromHex(SW1SW2_OK), nullptr));

    const auto& instance = *digestAuthenticateRespPars.get();

    ASSERT_EQ(typeid(instance), typeid(DigestAuthenticateRespPars));
}

TEST(DigestAuthenticateCmdBuildTest, digestAuthenticateCmdBuild_cla94)
{
    DigestAuthenticateCmdBuild digestAuthenticateCmdBuild(
        SamRevision::S1D, ByteArrayUtil::fromHex(SIGNATURE_PO));

    ASSERT_EQ(digestAuthenticateCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_94));
}

TEST(DigestAuthenticateCmdBuildTest, digestAuthenticateCmdBuild_cla80)
{
    DigestAuthenticateCmdBuild digestAuthenticateCmdBuild(
        SamRevision::C1, ByteArrayUtil::fromHex(SIGNATURE_PO));

    ASSERT_EQ(digestAuthenticateCmdBuild.getApduRequest()->getBytes(),
              ByteArrayUtil::fromHex(APDU_CLA_80));
}

TEST(DigestAuthenticateCmdBuildTest, digestAuthenticateCmdBuild_nullSignature)
{
    EXPECT_THROW(
        std::make_shared<DigestAuthenticateCmdBuild>(SamRevision::NO_REV,
                                                     std::vector<uint8_t>{}),
        IllegalArgumentException);
}

TEST(DigestAuthenticateCmdBuildTest,
    digestAuthenticateCmdBuild_badSignatureLength)
{
    EXPECT_THROW(
        std::make_shared<DigestAuthenticateCmdBuild>(
            SamRevision::NO_REV,
            ByteArrayUtil::fromHex(SIGNATURE_PO_BAD_LENGTH)),
        IllegalArgumentException);
}

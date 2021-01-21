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

#include "InvalidateCmdBuild.h"

/* Core */
#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::core::util;

static const std::vector<uint8_t> APDU_ISO_INVALIDATE = ByteArrayUtil::fromHex("0004000000");

TEST(InvalidateCmdBuildTest, invalidate)
{
    InvalidateCmdBuild builder(PoClass::ISO);
    const std::vector<uint8_t>& apduRequestBytes = builder.getApduRequest()->getBytes();

    ASSERT_EQ(apduRequestBytes, APDU_ISO_INVALIDATE);
}
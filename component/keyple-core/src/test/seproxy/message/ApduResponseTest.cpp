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

#include "ApduResponse.h"

#include "ByteArrayUtil.h"

using namespace testing;

using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

static std::shared_ptr<std::set<int>> getA9005CustomCode()
{
    std::shared_ptr<std::set<int>> successfulStatusCodes =
        std::make_shared< std::set<int> >();
    successfulStatusCodes->insert(0x9005);

    return successfulStatusCodes;
}

TEST(ApduResponseTest, constructorSuccessFullResponse)
{
    const std::vector<uint8_t> uApdu = {0xFE, 0xDC, 0xBA, 0x98, 0x90, 0x00};
    const std::vector<uint8_t> dataOut = {0xFE, 0xDC, 0xBA, 0x98};

    ApduResponse response(uApdu, nullptr);

    ASSERT_EQ(response.getStatusCode(), 0x9000);
    ASSERT_EQ(response.getBytes(), uApdu);
    ASSERT_EQ(response.getDataOut(), dataOut);
    ASSERT_TRUE(response.isSuccessful());
}

TEST(ApduResponseTest, constructorSuccessFullResponseWithCustomCode)
{
    const std::vector<uint8_t> uApdu = {0xFE, 0xDC, 0xBA, 0x98, 0x90, 0x05};
    const std::vector<uint8_t> dataOut = {0xFE, 0xDC, 0xBA, 0x98};

    ApduResponse response(uApdu, getA9005CustomCode());

    ASSERT_EQ(response.getStatusCode(), 0x9005);
    ASSERT_EQ(response.getBytes(), uApdu);
    ASSERT_EQ(response.getDataOut(), dataOut);
    ASSERT_TRUE(response.isSuccessful());
}

TEST(ApduResponseTest, constructorFailResponse)
{
    const std::vector<uint8_t> uApdu = {0xFE, 0xDC, 0xBA, 0x98, 0x90, 0x04};
    const std::vector<uint8_t> dataOut = {0xFE, 0xDC, 0xBA, 0x98};

    ApduResponse response(uApdu, nullptr);

    ASSERT_EQ(response.getBytes(), uApdu);
    ASSERT_EQ(response.getDataOut(), dataOut);
    ASSERT_EQ(response.getStatusCode(), 0x9004);
    ASSERT_FALSE(response.isSuccessful());
}

TEST(ApduResponseTest, constructorFailResponseWithCustomCode)
{
    const std::vector<uint8_t> uApdu = {0xFE, 0xDC, 0xBA, 0x98, 0x90, 0x04};
    const std::vector<uint8_t> dataOut = {0xFE, 0xDC, 0xBA, 0x98};

    ApduResponse response(uApdu, getA9005CustomCode());

    ASSERT_EQ(response.getBytes(), uApdu);
    ASSERT_EQ(response.getDataOut(), dataOut);
    ASSERT_EQ(response.getStatusCode(), 0x9004);
    ASSERT_FALSE(response.isSuccessful());
}


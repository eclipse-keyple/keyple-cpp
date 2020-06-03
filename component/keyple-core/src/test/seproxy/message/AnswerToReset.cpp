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

#include <algorithm>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "AnswerToReset.h"

using namespace testing;

using namespace keyple::core::seproxy::message;

TEST(AnswerToResetTest, AnswerToReset)
{
    const std::vector<uint8_t> atrBytes = {0x11, 0x22, 0x33, 0x44, 0x55};

    AnswerToReset atr(atrBytes);
}

TEST(AnswerToResetTest, getBytes)
{
    const std::vector<uint8_t> atrBytes = {0x11, 0x22, 0x33, 0x44, 0x55};

    AnswerToReset atr(atrBytes);

    ASSERT_EQ(atr.getBytes(), atrBytes);
}

TEST(AnswerToRestTest, operators)
{
    const std::vector<uint8_t> atrBytes1 = {0x11, 0x22, 0x33, 0x44, 0x55};
    const std::vector<uint8_t> atrBytes2 = {0x11, 0x22, 0x33, 0x44, 0x55};
    const std::vector<uint8_t> atrBytes3 = {0x12, 0x22, 0x33, 0x44, 0x55};

    AnswerToReset atr1(atrBytes1);
    AnswerToReset atr2(atrBytes2);
    AnswerToReset atr3(atrBytes3);

    ASSERT_EQ(atr1, atr2);
    ASSERT_NE(atr1, atr3);
}
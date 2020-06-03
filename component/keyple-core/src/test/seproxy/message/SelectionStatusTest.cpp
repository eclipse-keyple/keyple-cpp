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

#include "SelectionStatus.h"

#include "ApduResponse.h"

using namespace testing;

using namespace keyple::core::seproxy::message;

static const std::vector<uint8_t> atrBytes = {0x11, 0x22, 0x33, 0x44, 0x55};
static const std::shared_ptr<AnswerToReset> atr =
    std::make_shared<AnswerToReset>(atrBytes);

static const std::vector<uint8_t> fciBytes = {0x11, 0x22, 0x33, 0x90, 0x00};
static const std::shared_ptr<ApduResponse> fci =
    std::make_shared<ApduResponse>(fciBytes, nullptr);

TEST(SelectionStatusTest, SelectionStatus)
{
    SelectionStatus ss1(atr, nullptr, false);
    SelectionStatus ss2(nullptr, fci, false);
    SelectionStatus ss3(atr, fci, false);
}

TEST(SelectionStatusTest, getAtr)
{
    SelectionStatus ss1(nullptr, fci, false);
    SelectionStatus ss2(atr, nullptr, false);

    ASSERT_EQ(ss1.getAtr(), nullptr);
    ASSERT_EQ(ss2.getAtr(), atr);
}

TEST(SelectionStatusTest, getFci)
{
    SelectionStatus ss1(atr, nullptr, false);
    SelectionStatus ss2(nullptr, fci, false);

    ASSERT_EQ(ss1.getFci(), nullptr);
    ASSERT_EQ(ss2.getFci(), fci);
}

TEST(SelectionStatusTest, hasMatched)
{
    SelectionStatus ss1(atr, nullptr, false);
    SelectionStatus ss2(nullptr, fci, true);

    ASSERT_FALSE(ss1.hasMatched());
    ASSERT_TRUE(ss2.hasMatched());
}

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

#include "SeResponse.h"

#include "AnswerToReset.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"

using namespace testing;

using namespace keyple::core::seproxy::message;

static const std::vector<uint8_t> atrBytes = {0x11, 0x22, 0x33, 0x44, 0x55};
static const std::shared_ptr<AnswerToReset> atr =
    std::make_shared<AnswerToReset>(atrBytes);

static const std::vector<uint8_t> fciBytes = {0x11, 0x22, 0x33, 0x90, 0x00};
static const std::shared_ptr<ApduResponse> fci =
    std::make_shared<ApduResponse>(fciBytes, nullptr);

static const std::vector<uint8_t> r = {0xFE, 0xDC, 0xBA, 0x98, 0x90, 0x00};
static std::vector<std::shared_ptr<ApduResponse>> responses = {
    std::make_shared<ApduResponse>(r, nullptr),
};

TEST(SeResponseTest, constructorSuccessfullResponseMatch)
{
    SeResponse response(true, true,
                        std::make_shared<SelectionStatus>(atr, fci, true),
                        responses);

    ASSERT_EQ(response.getApduResponses(), responses);
    ASSERT_TRUE(response.wasChannelPreviouslyOpen());
    ASSERT_EQ(*(response.getSelectionStatus()->getAtr().get()), *(atr.get()));
    ASSERT_EQ(*(response.getSelectionStatus()->getFci().get()), *(fci.get()));
    ASSERT_TRUE(response.getSelectionStatus()->hasMatched());
}

TEST(SeResponseTest, constructorSuccessfullResponseNoMatch)
{
    SeResponse response(true, true,
                        std::make_shared<SelectionStatus>(atr, fci, false),
                        responses);

    ASSERT_EQ(response.getApduResponses(), responses);
    ASSERT_TRUE(response.wasChannelPreviouslyOpen());
    ASSERT_EQ(*(response.getSelectionStatus()->getAtr().get()), *(atr.get()));
    ASSERT_EQ(*(response.getSelectionStatus()->getFci().get()), *(fci.get()));
    ASSERT_FALSE(response.getSelectionStatus()->hasMatched());
}

TEST(SeResponseTest, constructorATRNull)
{
    SeResponse response(true, true,
                        std::make_shared<SelectionStatus>(nullptr, fci, true),
                        responses);
}

TEST(SeResponseTest, constructorFCINull)
{
    SeResponse response(true, true,
                        std::make_shared<SelectionStatus>(atr, nullptr, true),
                        responses);
}

TEST(SeResponseTest, constructorFCIAndATRNull)
{
    EXPECT_THROW(SeResponse(true, true,
                            std::make_shared<SelectionStatus>(nullptr, nullptr,
                                                              true),
                            responses),
                 std::invalid_argument);
}

TEST(SeResponseTest, testEquals)
{
    SeResponse response1(true, true,
                         std::make_shared<SelectionStatus>(atr, nullptr, true),
                         responses);
    SeResponse response2(true, true,
                         std::make_shared<SelectionStatus>(atr, nullptr, true),
                         responses);

    ASSERT_EQ(response1, response2);
}

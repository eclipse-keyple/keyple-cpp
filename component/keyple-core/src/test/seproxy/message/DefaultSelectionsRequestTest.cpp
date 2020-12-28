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

#include "DefaultSelectionsRequest.h"

using namespace testing;

using namespace keyple::core::seproxy::message;

static std::vector<std::shared_ptr<SeRequest>> requests;

TEST(DefaultSelectionsRequestTest, DefaultSelectionsRequest)
{
    DefaultSelectionsRequest request1(requests,
                                      MultiSeRequestProcessing::PROCESS_ALL,
                                      ChannelControl::CLOSE_AFTER);
    DefaultSelectionsRequest request2(requests);
}

TEST(DefaultSelectionsRequestTest, getMultiSeRequestProcessing)
{
    DefaultSelectionsRequest request1(requests,
                                      MultiSeRequestProcessing::PROCESS_ALL,
                                      ChannelControl::CLOSE_AFTER);
    DefaultSelectionsRequest request2(requests);

    ASSERT_EQ(request1.getMultiSeRequestProcessing(),
              MultiSeRequestProcessing::PROCESS_ALL);
    ASSERT_EQ(request2.getMultiSeRequestProcessing(),
              MultiSeRequestProcessing::FIRST_MATCH);
}

TEST(DefaultSelectionsRequestTest, getChannelControl)
{
    DefaultSelectionsRequest request1(requests,
                                      MultiSeRequestProcessing::PROCESS_ALL,
                                      ChannelControl::CLOSE_AFTER);
    DefaultSelectionsRequest request2(requests);

    ASSERT_EQ(request1.getChannelControl(), ChannelControl::CLOSE_AFTER);
    ASSERT_EQ(request2.getChannelControl(), ChannelControl::KEEP_OPEN);
}

TEST(DefaultSelectionsRequestTest, getSelectionSeRequestSet)
{
    DefaultSelectionsRequest request1(requests,
                                      MultiSeRequestProcessing::PROCESS_ALL,
                                      ChannelControl::CLOSE_AFTER);
    DefaultSelectionsRequest request2(requests);

    ASSERT_EQ(request1.getSelectionSeRequestSet(), requests);
    ASSERT_EQ(request2.getSelectionSeRequestSet(), requests);
}
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

#include "ReaderEvent.h"

using namespace testing;

using namespace keyple::core::seproxy::event;

using EventType = ReaderEvent::EventType;


TEST(ReaderEventTest, ReaderEvent)
{
    ReaderEvent event("plugin", "reader", EventType::SE_INSERTED, nullptr);
}

TEST(ReaderEventTest, getPluginName)
{
    ReaderEvent event("plugin", "reader", EventType::SE_INSERTED, nullptr);

    ASSERT_EQ(event.getPluginName(), "plugin");
}

TEST(ReaderEventTest, getReaderName)
{
    ReaderEvent event("plugin", "reader", EventType::SE_INSERTED, nullptr);

    ASSERT_EQ(event.getReaderName(), "reader");
}

TEST(ReaderEventTest, getEventType)
{
    ReaderEvent event("plugin", "reader", EventType::SE_INSERTED, nullptr);

    ASSERT_EQ(event.getEventType(), EventType::SE_INSERTED);
}


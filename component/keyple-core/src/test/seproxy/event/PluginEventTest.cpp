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

#include "PluginEvent.h"

using namespace testing;

using namespace keyple::core::seproxy::event;

using EventType = PluginEvent::EventType;

TEST(PluginEventTest, PluginEvent)
{
    PluginEvent event1("plugin1", "reader1", EventType::READER_CONNECTED);

    std::shared_ptr<std::set<std::string>> readerNames =
        std::make_shared<std::set<std::string>>();

    PluginEvent event2("plugin2", readerNames, EventType::READER_CONNECTED);
}

TEST(PluginEventTest, getPluginName)
{
    PluginEvent event1("plugin1", "reader1", EventType::READER_CONNECTED);

    ASSERT_EQ(event1.getPluginName(), "plugin1");

    std::shared_ptr<std::set<std::string>> readerNames =
        std::make_shared<std::set<std::string>>();

    PluginEvent event2("plugin2", readerNames, EventType::READER_CONNECTED);

    ASSERT_EQ(event2.getPluginName(), "plugin2");
}

TEST(PluginEventTest, getReaderNames)
{
    PluginEvent event1("plugin1", "reader1", EventType::READER_CONNECTED);

    const std::set<std::string>& readerNames1 = event1.getReaderNames();

    ASSERT_EQ(readerNames1.size(), 1);
    ASSERT_EQ(*readerNames1.begin(), "reader1");

    std::shared_ptr<std::set<std::string>> readerNames =
        std::make_shared<std::set<std::string>>();

    PluginEvent event2("plugin2", readerNames, EventType::READER_CONNECTED);

    const std::set<std::string>& readerNames2 = event2.getReaderNames();

    ASSERT_EQ(readerNames2.size(), 0);

    readerNames->insert("reader3");
    readerNames->insert("reader4");

    PluginEvent event3("plugin3", readerNames, EventType::READER_CONNECTED);

    const std::set<std::string>& readerNames3 = event3.getReaderNames();

    ASSERT_EQ(readerNames3.size(), 2);
    ASSERT_EQ(*readerNames3.begin(), "reader3");
    ASSERT_EQ(*readerNames3.rbegin(), "reader4");
}

TEST(PluginEventTest, getEventType)
{
    PluginEvent event1("plugin1", "reader1", EventType::READER_CONNECTED);

    ASSERT_EQ(event1.getEventType(), EventType::READER_CONNECTED);

    std::shared_ptr<std::set<std::string>> readerNames =
        std::make_shared<std::set<std::string>>();

    PluginEvent event2("plugin2", readerNames, EventType::READER_DISCONNECTED);

    ASSERT_EQ(event2.getEventType(), EventType::READER_DISCONNECTED);
}
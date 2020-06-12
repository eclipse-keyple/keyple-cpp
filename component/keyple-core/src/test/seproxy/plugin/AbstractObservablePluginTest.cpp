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

#include "AbstractObservablePlugin.h"

using namespace testing;

using namespace keyple::core::seproxy::plugin;

class ObservablePluginMock : public AbstractObservablePlugin {
public:
    ObservablePluginMock(const std::string& name)
    : AbstractObservablePlugin(name) {}

    MOCK_METHOD((const std::map<const std::string, const std::string>),
                getParameters, (), (const override));

    MOCK_METHOD(void, setParameter,
                (const std::string& key, const std::string& value), (override));

    MOCK_METHOD(std::set<std::shared_ptr<SeReader>>, initNativeReaders, (),
                (override));
};

class PublicObserverMock : public ObservablePlugin::PluginObserver {
public:
    void update(const std::shared_ptr<PluginEvent> event) override
    {
        (void)event;

        notified = true;
    }

    bool notified = false;
};

TEST(AbstractObservablePluginTest, AbstractObservablePlugin)
{
    ObservablePluginMock plugin("plugin");
}

TEST(AbstractObservablePluginTest, countObservers)
{
    ObservablePluginMock plugin("plugin");

    ASSERT_EQ(plugin.countObservers(), 0);
}

TEST(AbstractObservablePluginTest, addObserver)
{
    ObservablePluginMock plugin("plugin");

    std::shared_ptr<PublicObserverMock> observer =
        std::make_shared<PublicObserverMock>();

    plugin.addObserver(observer);

    ASSERT_EQ(plugin.countObservers(), 1);
}

TEST(AbstractObservablePluginTest, removeObserver)
{
    ObservablePluginMock plugin("plugin");

    std::shared_ptr<PublicObserverMock> observer =
        std::make_shared<PublicObserverMock>();

    plugin.addObserver(observer);

    ASSERT_EQ(plugin.countObservers(), 1);

    plugin.removeObserver(observer);

    ASSERT_EQ(plugin.countObservers(), 0);
}

TEST(AbstractObservablePluginTest, clearObservers)
{
    ObservablePluginMock plugin("plugin");

    std::shared_ptr<PublicObserverMock> observer =
        std::make_shared<PublicObserverMock>();

    plugin.addObserver(observer);

    ASSERT_EQ(plugin.countObservers(), 1);

    plugin.clearObservers();

    ASSERT_EQ(plugin.countObservers(), 0);
}


TEST(AbstractObservablePluginTest, notifyObservers)
{
    ObservablePluginMock plugin("plugin");

    std::shared_ptr<PublicObserverMock> observer =
        std::make_shared<PublicObserverMock>();

    ASSERT_FALSE(observer->notified);

    std::shared_ptr<PluginEvent> event =
        std::make_shared<PluginEvent>("plugin", "reader",
                                      PluginEvent::EventType::READER_CONNECTED);

    plugin.notifyObservers(event);

    ASSERT_TRUE(observer->notified);
}

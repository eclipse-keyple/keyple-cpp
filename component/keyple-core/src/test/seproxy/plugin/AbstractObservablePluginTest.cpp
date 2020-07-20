/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

class AOP_AbstractObservablePluginMock : public AbstractObservablePlugin {
public:
    AOP_AbstractObservablePluginMock(const std::string& name)
    : AbstractObservablePlugin(name) {}

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const, override));

    MOCK_METHOD(void,
                setParameter,
                (const std::string&, const std::string&),
                (override));

    MOCK_METHOD((std::map<const std::string, std::shared_ptr<SeReader>>),
                initNativeReaders,
                (),
                (override));
};

class AOP_PublicObserverMock : public ObservablePlugin::PluginObserver {
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
    AOP_AbstractObservablePluginMock plugin("plugin");
}

TEST(AbstractObservablePluginTest, countObservers)
{
    AOP_AbstractObservablePluginMock plugin("plugin");

    ASSERT_EQ(plugin.countObservers(), 0);
}

TEST(AbstractObservablePluginTest, addObserver)
{
    AOP_AbstractObservablePluginMock plugin("plugin");

    std::shared_ptr<AOP_PublicObserverMock> observer =
        std::make_shared<AOP_PublicObserverMock>();

    plugin.addObserver(observer);

    ASSERT_EQ(plugin.countObservers(), 1);
}

TEST(AbstractObservablePluginTest, removeObserver)
{
    AOP_AbstractObservablePluginMock plugin("plugin");

    std::shared_ptr<AOP_PublicObserverMock> observer =
        std::make_shared<AOP_PublicObserverMock>();

    plugin.addObserver(observer);

    ASSERT_EQ(plugin.countObservers(), 1);

    plugin.removeObserver(observer);

    ASSERT_EQ(plugin.countObservers(), 0);
}

TEST(AbstractObservablePluginTest, clearObservers)
{
    AOP_AbstractObservablePluginMock plugin("plugin");

    std::shared_ptr<AOP_PublicObserverMock> observer =
        std::make_shared<AOP_PublicObserverMock>();

    plugin.addObserver(observer);

    ASSERT_EQ(plugin.countObservers(), 1);

    plugin.clearObservers();

    ASSERT_EQ(plugin.countObservers(), 0);
}


TEST(AbstractObservablePluginTest, notifyObservers)
{
    AOP_AbstractObservablePluginMock plugin("plugin");

    std::shared_ptr<AOP_PublicObserverMock> observer =
        std::make_shared<AOP_PublicObserverMock>();

    plugin.addObserver(observer);

    ASSERT_FALSE(observer->notified);

    std::shared_ptr<PluginEvent> event =
        std::make_shared<PluginEvent>("plugin", "reader",
                                      PluginEvent::EventType::READER_CONNECTED);

    plugin.notifyObservers(event);

    ASSERT_TRUE(observer->notified);
}

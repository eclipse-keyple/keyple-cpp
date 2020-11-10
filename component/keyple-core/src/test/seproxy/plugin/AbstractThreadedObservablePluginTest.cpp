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

#include "AbstractThreadedObservablePlugin.h"

using namespace testing;

using namespace keyple::core::seproxy::plugin;

class ATOP_AbstractThreadedObservablePluginMock
: public AbstractThreadedObservablePlugin {
public:
    ATOP_AbstractThreadedObservablePluginMock(const std::string& name)
    : AbstractThreadedObservablePlugin(name) {}

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const override));

    MOCK_METHOD(void,
                setParameter,
                (const std::string& key, const std::string& value),
                (override));

    MOCK_METHOD((ConcurrentMap<const std::string, std::shared_ptr<SeReader>>&),
                initNativeReaders,
                (),
                (override));

    MOCK_METHOD(std::shared_ptr<SeReader>,
                fetchNativeReader,
                (const std::string& name),
                (override));

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

    bool isMonitoring()
    {
        return AbstractThreadedObservablePlugin::isMonitoring();
    }

    const std::set<std::string>& fetchNativeReadersNames() override
    {
        return readerNames;
    }

    const std::string& getName() const override
    {
        return AbstractThreadedObservablePlugin::getName();
    }

private:
    std::set<std::string> readerNames;
};

class ATOP_PublicObserverMock : public ObservablePlugin::PluginObserver {
public:
    void update(const std::shared_ptr<PluginEvent> event) override
    {
        (void)event;

        notified = true;
    }

    bool notified = false;
};

TEST(AbstractThreadedObservablePluginTest, AbstractThreadedObservablePlugin)
{
    ATOP_AbstractThreadedObservablePluginMock plugin("plugin");
}

TEST(AbstractThreadedObservablePluginTest, countObservers)
{
    ATOP_AbstractThreadedObservablePluginMock plugin("plugin");

    ASSERT_EQ(plugin.countObservers(), 0);
}

TEST(AbstractThreadedObservablePluginTest, addObserver)
{
    ATOP_AbstractThreadedObservablePluginMock plugin("plugin");

    auto observer = std::make_shared<ATOP_PublicObserverMock>();
   
    plugin.addObserver(observer);

    ASSERT_EQ(plugin.countObservers(), 1);
    ASSERT_TRUE(plugin.isMonitoring());
}

TEST(AbstractThreadedObservablePluginTest, removeObserver)
{
    ATOP_AbstractThreadedObservablePluginMock plugin("plugin");

    auto observer = std::make_shared<ATOP_PublicObserverMock>();
    
    plugin.addObserver(observer);

    ASSERT_EQ(plugin.countObservers(), 1);

    plugin.removeObserver(observer);

    ASSERT_EQ(plugin.countObservers(), 0);
    ASSERT_FALSE(plugin.isMonitoring());
}

TEST(AbstractThreadedObservablePluginTest, clearObservers)
{
    ATOP_AbstractThreadedObservablePluginMock plugin("plugin");

    auto observer = std::make_shared<ATOP_PublicObserverMock>();

    plugin.addObserver(observer);

    ASSERT_EQ(plugin.countObservers(), 1);

    plugin.clearObservers();

    ASSERT_EQ(plugin.countObservers(), 0);
    ASSERT_FALSE(plugin.isMonitoring());
}


TEST(AbstractThreadedObservablePluginTest, notifyObservers)
{
    ATOP_AbstractThreadedObservablePluginMock plugin("plugin");

    auto observer = std::make_shared<ATOP_PublicObserverMock>();

    plugin.addObserver(observer);

    ASSERT_FALSE(observer->notified);

    auto event = std::make_shared<PluginEvent>("plugin",
                                               "reader",
                                               PluginEvent::EventType::READER_CONNECTED);

    plugin.notifyObservers(event);

    ASSERT_TRUE(observer->notified);
}

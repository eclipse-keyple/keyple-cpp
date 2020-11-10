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

#include <atomic>
#include <regex>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "SeProxyService.h"

#include "AbstractPluginFactory.h"
#include "AbstractThreadedObservablePlugin.h"
#include "CountDownLatch.h"
#include "InterruptedException.h"
#include "MonitoringPool.h"
#include "ReaderPlugin.h"
#include "KeyplePluginInstantiationException.h"
#include "KeyplePluginNotFoundException.h"

using namespace testing;

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::plugin;

class SPS_AbstractThreadedObservablePluginMock
: public AbstractThreadedObservablePlugin {
public:
    SPS_AbstractThreadedObservablePluginMock(const std::string& name)
    : AbstractThreadedObservablePlugin(name) {}

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const override));

    MOCK_METHOD(void,
                setParameter,
                (const std::string&, const std::string&),
                (override));

    MOCK_METHOD((ConcurrentMap<const std::string, std::shared_ptr<SeReader>>&),
                initNativeReaders,
                (),
                (override));

    MOCK_METHOD(std::shared_ptr<SeReader>,
                fetchNativeReader,
                (const std::string& name),
                (override));

    MOCK_METHOD(const std::set<std::string>&,
                fetchNativeReadersNames,
                (),
                (override));

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

    const std::string& getName() const override
    {
        return AbstractThreadedObservablePlugin::getName();
    }
};

class SPS_PluginFactoryMock : public PluginFactory {
public:

    MOCK_METHOD(const std::string&,
                getPluginName,
                (),
                (const, override));

    MOCK_METHOD(std::shared_ptr<ReaderPlugin>,
                getPlugin,
                (),
                (const, override));
};

class SPS_ObservablePluginFactoryMock : public PluginFactory {
public:
    SPS_ObservablePluginFactoryMock(const std::string& pluginName)
    : PluginFactory(), mPluginName(pluginName) {}

    const std::string& getPluginName() const override
    {
        return mPluginName;
    }

    std::shared_ptr<ReaderPlugin> getPlugin() const override
    {
        return std::make_shared<SPS_AbstractThreadedObservablePluginMock>(mPluginName);
    }

private:
    const std::string mPluginName;
};

static const std::string PLUGIN_NAME_1 = "plugin1";
static const std::string PLUGIN_NAME_2 = "plugin2";

TEST(SeProxyServiceTest, testGetVersion)
{
    /* Test that version follows semver guidelines */
    std::string regex = "^([0-9]+)\\.([0-9]+)\\.([0-9]+)(?:-([0-9A-Za-z-]+(?:"
                        "\\.[0-9A-Za-z-]+)*))?(?:\\+[0-9A-Za-z-]+)?$";

    std::string version = SeProxyService::getInstance().getVersion();

    ASSERT_TRUE(std::regex_match(version, std::regex(regex)));
}

TEST(SeProxyServiceTest, testFailingPlugin)
{
    auto plugin1 = std::make_shared<SPS_AbstractThreadedObservablePluginMock>(PLUGIN_NAME_1);
    auto factory1 = std::make_shared<SPS_PluginFactoryMock>();

    SeProxyService& proxyService = SeProxyService::getInstance();

    EXPECT_CALL(*factory1, getPluginName()).WillOnce(ReturnRef(PLUGIN_NAME_1));
    EXPECT_CALL(*factory1, getPlugin())
        .Times(1)
        .WillOnce(Throw(KeyplePluginInstantiationException("")));

    EXPECT_THROW(proxyService.registerPlugin(factory1), KeyplePluginInstantiationException);
}

TEST(SeProxyServiceTest, testRegisterPlugin)
{
    auto plugin1 = std::make_shared<SPS_AbstractThreadedObservablePluginMock>(PLUGIN_NAME_1);
    auto factory1 = std::make_shared<SPS_PluginFactoryMock>();

    EXPECT_CALL(*factory1, getPlugin())
        .Times(1)
        .WillOnce(Return(plugin1));
    EXPECT_CALL(*factory1, getPluginName())
        .Times(1)
        .WillOnce(ReturnRef(PLUGIN_NAME_1));

    SeProxyService& proxyService = SeProxyService::getInstance();

    /* Register plugin 1 by its factory */
    std::shared_ptr<ReaderPlugin> testPlugin = proxyService.registerPlugin(factory1);

    /* Results */
    const std::map<const std::string, std::shared_ptr<ReaderPlugin>>&
        testPlugins = proxyService.getPlugins();

    ASSERT_NE(testPlugin, nullptr);
    ASSERT_EQ(testPlugin->getName(), PLUGIN_NAME_1);
    ASSERT_EQ(static_cast<int>(testPlugins.size()), 1);

    /* Unregister */
    proxyService.unregisterPlugin(PLUGIN_NAME_1);
}

TEST(SeProxyServiceTest, testRegisterTwicePlugin)
{
    auto plugin1 = std::make_shared<SPS_AbstractThreadedObservablePluginMock>(PLUGIN_NAME_1);
    auto factory1 = std::make_shared<SPS_PluginFactoryMock>();

    EXPECT_CALL(*factory1, getPlugin())
        .Times(1)
        .WillOnce(Return(plugin1));
    EXPECT_CALL(*factory1, getPluginName())
        .Times(2)
        .WillRepeatedly(ReturnRef(PLUGIN_NAME_1));

    SeProxyService& proxyService = SeProxyService::getInstance();

    /* Register plugin 1 by its factory */
    proxyService.registerPlugin(factory1);
    proxyService.registerPlugin(factory1);

    /* Should not be added twice */
    const std::map<const std::string, std::shared_ptr<ReaderPlugin>>&
        testPlugins = proxyService.getPlugins();

    ASSERT_EQ(static_cast<int>(testPlugins.size()), 1);

    /* Unregister */
    proxyService.unregisterPlugin(PLUGIN_NAME_1);
}

TEST(SeProxyServiceTest, testRegisterTwoPlugin)
{
    auto plugin1 = std::make_shared<SPS_AbstractThreadedObservablePluginMock>(PLUGIN_NAME_1);
    auto plugin2 = std::make_shared<SPS_AbstractThreadedObservablePluginMock>(PLUGIN_NAME_2);

    auto factory1 = std::make_shared<SPS_PluginFactoryMock>();
    auto factory2 = std::make_shared<SPS_PluginFactoryMock>();

    EXPECT_CALL(*factory1, getPlugin())
        .Times(1)
        .WillOnce(Return(plugin1));
    EXPECT_CALL(*factory2, getPlugin())
        .Times(1)
        .WillOnce(Return(plugin2));

    EXPECT_CALL(*factory1, getPluginName())
        .Times(1)
        .WillOnce(ReturnRef(PLUGIN_NAME_1));
    EXPECT_CALL(*factory2, getPluginName())
        .Times(1)
        .WillOnce(ReturnRef(PLUGIN_NAME_2));

    SeProxyService& proxyService = SeProxyService::getInstance();

    /* Register plugin 1 and 2 by their factory */
    proxyService.registerPlugin(factory1);
    proxyService.registerPlugin(factory2);

    /* Should be two */
    const std::map<const std::string, std::shared_ptr<ReaderPlugin>>&
        testPlugins = proxyService.getPlugins();

    ASSERT_EQ(static_cast<int>(testPlugins.size()), 2);

    /* Unregister */
    proxyService.unregisterPlugin(PLUGIN_NAME_1);
    proxyService.unregisterPlugin(PLUGIN_NAME_2);
}

TEST(SeProxyServiceTest, testGetPluginFail)
{
    SeProxyService& proxyService = SeProxyService::getInstance();

    EXPECT_THROW(proxyService.getPlugin("unknown"),KeyplePluginNotFoundException);
}

/**
 * Test that a plugin can not be added twice with multi thread
 *
 * @throws Exception
 */
TEST(SeProxyServiceTest, testRegister_MultiThread)
{
    SeProxyService& proxyService = SeProxyService::getInstance();

    auto factory = std::make_shared<SPS_ObservablePluginFactoryMock>(PLUGIN_NAME_1);
    auto latch = std::make_shared<CountDownLatch>(1);

    std::atomic<bool> running;
    std::atomic<int> overlaps;

    int threads = 10;
    MonitoringPool service;
    std::vector<std::future<void>> futures;

    for (int t = 0; t < threads; ++t) {
        futures.push_back(
            std::async(
                [&latch, &running, &overlaps, &proxyService, &factory]() {
                    try {
                        /*
                         * All thread wait for the countdown
                         */
                        latch->await();
                    } catch (const InterruptedException& e) {
                        (void)e;
                    }

                    if (running) {
                        overlaps++;
                    }

                    running = true;
                    try {
                        proxyService.registerPlugin(factory);
                    } catch (const KeyplePluginInstantiationException& e) {
                        (void)e;
                    }

                    running = false;
                }
            )
        );
    }

    /* Release all thread at once */
    latch->countDown();

    /* Wait for execution */
    Thread::sleep(500);

    ASSERT_EQ(static_cast<int>(proxyService.getPlugins().size()), 1);

    /* Unregister */
    proxyService.unregisterPlugin(PLUGIN_NAME_1);
}

/**
 * Test that a plugin can not be added twice with multi thread
 *
 * @throws Exception
 */
TEST(SeProxyServiceTest, unregisterMultiThread)
{
    SeProxyService& proxyService = SeProxyService::getInstance();

    auto factory = std::make_shared<SPS_ObservablePluginFactoryMock>(PLUGIN_NAME_1);

    /* Add a plugin */
    proxyService.registerPlugin(factory);

    auto latch = std::make_shared<CountDownLatch>(1);

    std::atomic<bool> running;
    std::atomic<int> overlaps;

    int threads = 10;
    MonitoringPool service;
    std::vector<std::future<void>> futures;

    for (int t = 0; t < threads; ++t) {
        futures.push_back(
            std::async(
                std::launch::async,
                [latch, &running, &overlaps, &proxyService, factory]() {
                    try {
                        /* All thread wait for the countdown */
                        latch->await();
                    } catch (const InterruptedException& e) {
                        (void)e;
                    }

                    if (running) {
                        overlaps++;
                    }

                    running = true;
                    proxyService.unregisterPlugin(factory->getPluginName());
                    running = false;
                }
            )
        );
    }

    /* Release all thread at once */
    latch->countDown();
    Thread::sleep(500);

    ASSERT_EQ(static_cast<int>(proxyService.getPlugins().size()), 0);

    /* Unregister */
    proxyService.unregisterPlugin(PLUGIN_NAME_1);
}

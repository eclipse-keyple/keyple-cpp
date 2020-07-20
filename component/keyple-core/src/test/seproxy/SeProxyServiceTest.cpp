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
using namespace keyple::core::seproxy::plugin::local;

class SPS_AbstractThreadedObservablePluginMock
    : public AbstractThreadedObservablePlugin {
public:
    const std::set<std::string> readerNames;

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

    MOCK_METHOD((std::map<const std::string, std::shared_ptr<SeReader>>),
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

static const std::string PLUGIN_NAME_1 = "plugin1";
static const std::string PLUGIN_NAME_2 = "plugin2";

static std::shared_ptr<SPS_AbstractThreadedObservablePluginMock> plugin1 =
    std::make_shared<SPS_AbstractThreadedObservablePluginMock>(PLUGIN_NAME_1);
static std::shared_ptr<SPS_AbstractThreadedObservablePluginMock> plugin2 =
    std::make_shared<SPS_AbstractThreadedObservablePluginMock>(PLUGIN_NAME_2);

TEST(SeProxyServiceTest, getInstance)
{
    SeProxyService& proxy = SeProxyService::getInstance();

    (void)proxy;
}

TEST(SeProxyServiceTest, getVersion)
{
    /* Test that version follows semver guidelines */
    std::string regex = "^([0-9]+)\\.([0-9]+)\\.([0-9]+)(?:-([0-9A-Za-z-]+(?:"
                        "\\.[0-9A-Za-z-]+)*))?(?:\\+[0-9A-Za-z-]+)?$";

    std::string version = SeProxyService::getInstance().getVersion();

    ASSERT_TRUE(std::regex_match(version, std::regex(regex)));
}

TEST(SeProxyServiceTest, failingPlugin)
{
    std::shared_ptr<SPS_PluginFactoryMock> factory =
        std::make_shared<SPS_PluginFactoryMock>();

    EXPECT_THROW(factory->getPlugin(),
                 KeyplePluginInstantiationException);
}

TEST(SeProxyServiceTest, registerPlugin)
{
    SeProxyService& proxyService = SeProxyService::getInstance();

    std::shared_ptr<SPS_PluginFactoryMock> factory =
        std::make_shared<SPS_PluginFactoryMock>();

    EXPECT_CALL(*factory.get(), getPluginName())
        .Times(1)
        .WillOnce(ReturnRef(PLUGIN_NAME_1));

    EXPECT_CALL(*factory.get(), getPlugin())
        .Times(1)
        .WillOnce(Return(plugin1));

    std::shared_ptr<ReaderPlugin> testPlugin =
        proxyService.registerPlugin(factory);

    /* Results */
    const std::map<const std::string, std::shared_ptr<ReaderPlugin>>&
        testPlugins = proxyService.getPlugins();

    ASSERT_EQ(testPlugin->getName(), PLUGIN_NAME_1);
    ASSERT_EQ(static_cast<int>(testPlugins.size()), 1);

    /* Unregister */
    proxyService.unregisterPlugin(PLUGIN_NAME_1);
}

TEST(SeProxyServiceTest, registerPlugin_Twice)
{
    SeProxyService& proxyService = SeProxyService::getInstance();

    std::shared_ptr<SPS_PluginFactoryMock> factory =
        std::make_shared<SPS_PluginFactoryMock>();

    EXPECT_CALL(*factory.get(), getPluginName())
        .Times(1)
        .WillOnce(ReturnRef(PLUGIN_NAME_1));

    EXPECT_CALL(*factory.get(), getPlugin())
        .Times(1)
        .WillOnce(Return(plugin1));

    proxyService.registerPlugin(factory);
    proxyService.registerPlugin(factory);

    /* Results */
    const std::map<const std::string, std::shared_ptr<ReaderPlugin>>&
        testPlugins = proxyService.getPlugins();

    /* Should not be added twice */
    ASSERT_EQ(static_cast<int>(testPlugins.size()), 1);

    /* Unregister */
    proxyService.unregisterPlugin(PLUGIN_NAME_1);
}

TEST(SeProxyServiceTest, registerPlugin_TwoPlugins)
{
    SeProxyService& proxyService = SeProxyService::getInstance();

    std::shared_ptr<SPS_PluginFactoryMock> factory1 =
        std::make_shared<SPS_PluginFactoryMock>();

    EXPECT_CALL(*factory1.get(), getPluginName())
        .Times(1)
        .WillOnce(ReturnRef(PLUGIN_NAME_1));

    EXPECT_CALL(*factory1.get(), getPlugin())
        .Times(1)
        .WillOnce(Return(plugin1));

    proxyService.registerPlugin(factory1);

    std::shared_ptr<SPS_PluginFactoryMock> factory2 =
        std::make_shared<SPS_PluginFactoryMock>();

    EXPECT_CALL(*factory2.get(), getPluginName())
        .Times(1)
        .WillOnce(ReturnRef(PLUGIN_NAME_2));

    EXPECT_CALL(*factory2.get(), getPlugin())
        .Times(1)
        .WillOnce(Return(plugin2));

    proxyService.registerPlugin(factory2);

    /* Results */
    const std::map<const std::string, std::shared_ptr<ReaderPlugin>>&
        testPlugins = proxyService.getPlugins();

    /* Should not be added twice */
    ASSERT_EQ(static_cast<int>(testPlugins.size()), 2);

    /* Unregister */
    proxyService.unregisterPlugin(PLUGIN_NAME_1);
    proxyService.unregisterPlugin(PLUGIN_NAME_2);
}

TEST(SeProxyServiceTest, getPlugin_Unknown)
{
    SeProxyService& proxyService = SeProxyService::getInstance();

    EXPECT_THROW(proxyService.getPlugin("unknown"),
                 KeyplePluginNotFoundException);
}

/**
 * Test that a plugin can not be added twice with multi thread
 *
 * @throws Exception
 */
TEST(SeProxyServiceTest, testRegister_MultiThread)
{
    SeProxyService& proxyService = SeProxyService::getInstance();

    std::shared_ptr<SPS_PluginFactoryMock> factory =
        std::make_shared<SPS_PluginFactoryMock>();

    EXPECT_CALL(*factory.get(), getPluginName())
        .Times(1)
        .WillOnce(ReturnRef(PLUGIN_NAME_1));

    EXPECT_CALL(*factory.get(), getPlugin())
        .Times(1)
        .WillOnce(Return(plugin1));

    std::shared_ptr<CountDownLatch> latch =
        std::make_shared<CountDownLatch>(1);

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

    std::shared_ptr<SPS_PluginFactoryMock> factory =
        std::make_shared<SPS_PluginFactoryMock>();

    EXPECT_CALL(*factory.get(), getPluginName())
        .Times(1)
        .WillOnce(ReturnRef(PLUGIN_NAME_1));

    EXPECT_CALL(*factory.get(), getPlugin())
        .Times(1)
        .WillOnce(Return(plugin1));

    /* Add a plugin */
    proxyService.registerPlugin(factory);

    std::shared_ptr<CountDownLatch> latch =
        std::make_shared<CountDownLatch>(1);

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

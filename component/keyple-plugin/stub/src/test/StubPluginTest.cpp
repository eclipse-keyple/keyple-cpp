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

#include "StubPlugin.h"

/* Common */
#include "CountDownLatch.h"
#include "Thread.h"

/* Plugin */
#include "PluginEvent.h"
#include "StubPluginImpl.h"
#include "StubPluginFactory.h"
#include "StubReader.h"

using namespace testing;

using namespace keyple::common;
using namespace keyple::core::seproxy::event;
using namespace keyple::plugin::stub;

class SP_PluginObserverMock final : public ObservablePlugin::PluginObserver {
public:
    SP_PluginObserverMock(const std::string& readerName)
    : mReaderConnected(1), mReaderName(readerName) {}

    void update(std::shared_ptr<PluginEvent> event) override
    {
        ASSERT_EQ(event->getEventType(), PluginEvent::EventType::READER_CONNECTED);
        ASSERT_EQ(static_cast<int>(event->getReaderNames().size()), 1);
        ASSERT_EQ(*event->getReaderNames().begin(), mReaderName);
        mReaderConnected.countDown();
    }

    CountDownLatch mReaderConnected;

private:
    const std::string mReaderName;
};

static const std::string PLUGIN_NAME = "stub1";
static std::shared_ptr<StubPluginImpl> stubPlugin;

static void setUp()
{
    auto factory = std::make_shared<StubPluginFactory>(PLUGIN_NAME);
    stubPlugin = std::dynamic_pointer_cast<StubPluginImpl>(factory->getPlugin());
}

static void tearDown()
{
    stubPlugin->unplugStubReaders(stubPlugin->getReaderNames(), true);
    stubPlugin->clearObservers();
}

TEST(StubPluginTest, instantiatePlugin)
 {
    StubPluginFactory factory(PLUGIN_NAME);

    std::shared_ptr<ReaderPlugin> plugin = factory.getPlugin();

    ASSERT_EQ(plugin->getName(), PLUGIN_NAME);
}

/* Plug one reader synchronously Check: Count if created */
TEST(StubPluginTest, plugOneReaderSync_success)
{
    setUp();

    /* Add a sleep to play with thread monitor timeout */
    Thread::sleep(100);

    const std::string READER_NAME = "plugOneReaderSync_sucess";

    /* Connect reader */
    stubPlugin->plugStubReader(READER_NAME, TransmissionMode::CONTACTLESS,
                               true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    std::shared_ptr<StubReader> stubReader =
        std::dynamic_pointer_cast<StubReader>(
            stubPlugin->getReaders().begin()->second);

    ASSERT_EQ(stubReader->getName(), READER_NAME);
    ASSERT_EQ(stubReader->getTransmissionMode(), TransmissionMode::CONTACTLESS);

    tearDown();
}

/* Plug one reader synchronously Check: Event thrown */
TEST(StubPluginTest, plugOneReaderSyncEvent_success)
{
    setUp();

    const std::string READER_NAME = "testA_PlugReaders";
    auto plugin = std::make_shared<SP_PluginObserverMock>(READER_NAME);

    /* add READER_CONNECTED assert observer */
    stubPlugin->addObserver(plugin);
    stubPlugin->plugStubReader(READER_NAME, true);
    plugin->mReaderConnected.await(std::chrono::seconds(2));

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);
    ASSERT_EQ(static_cast<int>(plugin->mReaderConnected.getCount()), 0);

    tearDown();
}

/* Unplug one reader synchronously Check: Count if removed */
TEST(StubPluginTest, unplugOneReader_success)
{
    setUp();

    const std::string READER_NAME = "unplugOneReader_success";

    /* Connect reader */
    stubPlugin->plugStubReader(READER_NAME, true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    stubPlugin->unplugStubReader(READER_NAME, true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 0);

    tearDown();
}



/* Plug same reader twice Check : only one reader */
TEST(StubPluginTest, plugSameReaderTwice_fail)
{
    setUp();

    const std::string READER_NAME = "testC_PlugSameReaderTwice";

    stubPlugin->plugStubReader(READER_NAME, true);
    stubPlugin->plugStubReader(READER_NAME, true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    tearDown();
}

/* Get name */
TEST(StubPluginTest, getName_success)
{
    setUp();

    ASSERT_NE(stubPlugin->getName(), "");

    tearDown();
}

/**
 * Plug many readers at once sync
 *
 * Check : count readers
 */
TEST(StubPluginTest, plugMultiReadersSync_success)
{
    setUp();

    const std::set<std::string> newReaders = {
        "EC_reader1", "EC_reader2", "EC_reader3"};

    /* Connect readers at once */
    stubPlugin->plugStubReaders(newReaders, true);

    ASSERT_EQ(stubPlugin->getReaderNames(), newReaders);
    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 3);

    tearDown();
}

/* Plug and unplug many readers at once synchronously Check : count */
TEST(StubPluginTest, plugUnplugMultiReadersSync_success)
{
    setUp();

    const std::set<std::string> READERS = {
        "FC_Reader1", "FC_Reader2", "FC_Reader3"};

    // connect readers at once
    stubPlugin->plugStubReaders(READERS, true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 3);

    stubPlugin->unplugStubReaders(READERS, true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 0);

    tearDown();
}

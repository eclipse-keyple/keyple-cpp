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

#include "CountDownLatch.h"
#include "StubPluginImpl.h"
#include "StubPluginFactory.h"
#include "StubReader.h"
#include "Thread.h"

using namespace testing;

using namespace keyple::common;
using namespace keyple::plugin::stub;

class SP_PluginObserverMock : public ObservablePlugin::PluginObserver {
public:
    SP_PluginObserverMock()
    : mReaderConnected(1)
    {
    }

    void update(std::shared_ptr<PluginEvent> event) override
    {
        ASSERT_EQ(event->getEventType(),
                  PluginEvent::EventType::READER_CONNECTED);
        ASSERT_EQ(static_cast<int>(event->getReaderNames().size()), 1);
        ASSERT_EQ(*event->getReaderNames().begin(), READER_NAME);

        mReaderConnected.countDown();
    }

    CountDownLatch mReaderConnected;

private:
    const std::string READER_NAME = "plugOneReaderSync_sucess";
};

static const std::string PLUGIN_NAME = "stub1";


TEST(StubPluginTest, instantiatePlugin)
 {
    const std::string PLUGIN_NAME = "test1";

    StubPluginFactory factory(PLUGIN_NAME);

    std::shared_ptr<ReaderPlugin> plugin = factory.getPlugin();

   ASSERT_EQ(plugin->getName(), PLUGIN_NAME);
}

/* Plug one reader synchronously Check: Count if created */
TEST(StubPluginTest, plugOneReaderSync_success)
{
    std::shared_ptr<StubPluginImpl> stubPlugin =
        std::dynamic_pointer_cast<StubPluginImpl>(
            std::make_shared<StubPluginFactory>(PLUGIN_NAME)->getPlugin());

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
}

/* Plug one reader synchronously Check: Event thrown */
TEST(StubPluginTest, plugOneReaderSyncEvent_success)
{
    std::shared_ptr<StubPluginImpl> stubPlugin =
        std::dynamic_pointer_cast<StubPluginImpl>(
            std::make_shared<StubPluginFactory>(PLUGIN_NAME)->getPlugin());

    const std::string READER_NAME = "testA_PlugReaders";

    std::shared_ptr<SP_PluginObserverMock> plugin =
        std::make_shared<SP_PluginObserverMock>();

    /* add READER_CONNECTED assert observer */
    stubPlugin->addObserver(plugin);

    stubPlugin->plugStubReader(READER_NAME, true);
    plugin->mReaderConnected.await(std::chrono::seconds(2));

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);
    ASSERT_EQ(plugin->mReaderConnected.getCount(), 0);
}

/* Unplug one reader synchronously Check: Count if removed */
TEST(StubPluginTest, unplugOneReader_success)
{
    std::shared_ptr<StubPluginImpl> stubPlugin =
        std::dynamic_pointer_cast<StubPluginImpl>(
            std::make_shared<StubPluginFactory>(PLUGIN_NAME)->getPlugin());

    const std::string READER_NAME = "unplugOneReader_success";

    /* Connect reader */
    stubPlugin->plugStubReader(READER_NAME, true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    stubPlugin->unplugStubReader(READER_NAME, true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 0);
}



/* Plug same reader twice Check : only one reader */
TEST(StubPluginTest, plugSameReaderTwice_fail)
{
    std::shared_ptr<StubPluginImpl> stubPlugin =
        std::dynamic_pointer_cast<StubPluginImpl>(
            std::make_shared<StubPluginFactory>(PLUGIN_NAME)->getPlugin());

    const std::string READER_NAME = "testC_PlugSameReaderTwice";

    stubPlugin->plugStubReader(READER_NAME, true);
    stubPlugin->plugStubReader(READER_NAME, true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);
}

/* Get name */
TEST(StubPluginTest, getName_success)
{
    std::shared_ptr<StubPluginImpl> stubPlugin =
        std::dynamic_pointer_cast<StubPluginImpl>(
            std::make_shared<StubPluginFactory>(PLUGIN_NAME)->getPlugin());

    ASSERT_NE(stubPlugin->getName(), "");
}

/**
 * Plug many readers at once sync
 *
 * Check : count readers
 */
TEST(StubPluginTest, plugMultiReadersSync_success)
{
    std::shared_ptr<StubPluginImpl> stubPlugin =
        std::dynamic_pointer_cast<StubPluginImpl>(
            std::make_shared<StubPluginFactory>(PLUGIN_NAME)->getPlugin());

    const std::set<std::string> newReaders = {
        "EC_reader1", "EC_reader2", "EC_reader3"};

    /* Connect readers at once */
    stubPlugin->plugStubReaders(newReaders, true);

    ASSERT_EQ(stubPlugin->getReaderNames(), newReaders);
    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 3);
}



/* Plug and unplug many readers at once synchronously Check : count */
TEST(StubPluginTest, plugUnplugMultiReadersSync_success)
{
    std::shared_ptr<StubPluginImpl> stubPlugin =
        std::dynamic_pointer_cast<StubPluginImpl>(
            std::make_shared<StubPluginFactory>(PLUGIN_NAME)->getPlugin());

    const std::set<std::string> READERS = {
        "FC_Reader1", "FC_Reader2", "FC_Reader3"};

    // connect readers at once
    stubPlugin->plugStubReaders(READERS, true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 3);

    stubPlugin->unplugStubReaders(READERS, true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 0);
}

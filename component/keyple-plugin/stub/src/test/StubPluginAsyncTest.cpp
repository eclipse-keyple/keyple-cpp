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
#include "StubPluginImpl.h"
#include "StubPluginFactory.h"
#include "StubReader.h"

using namespace testing;

using namespace keyple::common;
using namespace keyple::plugin::stub;

class SPA_PluginObserverMock1 final : public ObservablePlugin::PluginObserver {
public:
    SPA_PluginObserverMock1(const std::string& readerName)
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

class SPA_PluginObserverMock2 final : public ObservablePlugin::PluginObserver {
public:
    SPA_PluginObserverMock2(const std::string& readerName)
    : mReaderConnected(1), mReaderDisconnected(1), mReaderName(readerName) {}

    void update(std::shared_ptr<PluginEvent> event) override
    {
        if (event_i == 1) {
            ASSERT_EQ(event->getEventType(), PluginEvent::EventType::READER_CONNECTED);
            mReaderConnected.countDown();
        }

        /* Analyze the second event, should be a READER_DISCONNECTED */
        if (event_i == 2) {
            ASSERT_EQ(event->getEventType(), PluginEvent::EventType::READER_DISCONNECTED);
            ASSERT_EQ(static_cast<int>(event->getReaderNames().size()), 1);
            ASSERT_EQ(*event->getReaderNames().begin(), mReaderName);
            mReaderDisconnected.countDown();
        }

        event_i++;
    }

    CountDownLatch mReaderConnected;
    CountDownLatch mReaderDisconnected;

private:
    int event_i = 1;
    const std::string mReaderName;
};

class SPA_PluginObserverMock3 final : public ObservablePlugin::PluginObserver {
public:
    SPA_PluginObserverMock3(const std::string& readerName)
    : mReaderConnected(3), mReaderName(readerName) {}

    void update(std::shared_ptr<PluginEvent> event) override
    {
        ASSERT_EQ(event->getEventType(), PluginEvent::EventType::READER_CONNECTED);

        /* Can be one or three */
        ASSERT_GE(event->getReaderNames().size(), 1);

        /* We are waiting for 3 notifications of reader insertion */
        for (int i = 0; i < static_cast<int>(event->getReaderNames().size()); i++)
            mReaderConnected.countDown();

    }

    CountDownLatch mReaderConnected;

private:
    const std::string mReaderName;
};


class SPA_PluginObserverMock4 final : public ObservablePlugin::PluginObserver {
public:
    SPA_PluginObserverMock4(const std::set<std::string>& readerNames)
    : mReaderConnected(1), mReaderDisconnected(1), mReaderNames(readerNames) {}

    void update(std::shared_ptr<PluginEvent> event) override
    {
        if (event_i == 1) {
            ASSERT_EQ(event->getEventType(), PluginEvent::EventType::READER_CONNECTED);
            mReaderConnected.countDown();
        }

        /* Analyze the second event, should be a READER_DISCONNECTED */
        if (event_i == 2) {
            ASSERT_EQ(event->getEventType(), PluginEvent::EventType::READER_DISCONNECTED);
            //ASSERT_EQ(static_cast<int>(event->getReaderNames().size()), 3);
            ASSERT_EQ(event->getReaderNames(), mReaderNames);
            mReaderDisconnected.countDown();
        }

        event_i++;
    }

    CountDownLatch mReaderConnected;
    CountDownLatch mReaderDisconnected;

private:
    int event_i = 1;
    const std::set<std::string> mReaderNames;
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

/* Plug one reader asynchronously Check: Event thrown */
TEST(StubPluginAsyncTest, plugOneReaderAsync_success)
{
    setUp();

    /* Add READER_CONNECTED assert observer */
    const std::string READER_NAME = "plugOneReaderAsync_sucess";
    auto observer = std::make_shared<SPA_PluginObserverMock1>(READER_NAME);
    stubPlugin->addObserver(observer);

    stubPlugin->plugStubReader(READER_NAME, TransmissionMode::CONTACTLESS, false);

    observer->mReaderConnected.await(std::chrono::seconds(2));

    ASSERT_EQ(observer->mReaderConnected.getCount(), 0);

    tearDown();
}

/* Unplug one reader and wait for event */
TEST(StubPluginAsyncTest, unplugOneReaderAsync_success)
{
    setUp();

    const std::string READER_NAME = "unplugOneReaderAsync_success";
    auto disconnected_obs = std::make_shared<SPA_PluginObserverMock2>(READER_NAME);

    /* Add ReaderEvent observer */
    stubPlugin->addObserver(disconnected_obs);

    /* Plug a reader */
    stubPlugin->plugStubReader(READER_NAME, false);

    disconnected_obs->mReaderConnected.await(std::chrono::seconds(2));

    /* Inplug reader */
    stubPlugin->unplugStubReader(READER_NAME, false);

    /* Wait for event to be raised */
    disconnected_obs->mReaderDisconnected.await(std::chrono::seconds(2));

    ASSERT_EQ(disconnected_obs->mReaderDisconnected.getCount(), 0);

    tearDown();
}

/* Plug many readers at once async Check : check event, count event */
TEST(StubPluginAsyncTest, plugMultiReadersAsync_success)
{
    setUp();

    /* Add READER_CONNECTED assert observer */
    const std::string READER_NAME = "plugMultiReadersAsync_success";
    auto observer = std::make_shared<SPA_PluginObserverMock3>(READER_NAME);
    stubPlugin->addObserver(observer);

    /* Connect readers */
    const std::set<std::string> READERS = {"E_Reader1", "E_Reader2", "E_Reader3"};
    stubPlugin->plugStubReaders(READERS, false);

    /* Wait for event to be raised */
    observer->mReaderConnected.await(std::chrono::seconds(2));

    ASSERT_EQ(observer->mReaderConnected.getCount(), 0);

    tearDown();
}


/* Plug and unplug many readers at once asynchronously Check : event and count events */
TEST(StubPluginAsyncTest, plugUnplugMultiReadersAsync_success)
{
    setUp();

    /* Add assert DISCONNECT assert observer */
    const std::set<std::string> READERS = {"F_Reader1", "F_Reader2", "F_Reader3"};
    auto assertDisconnect = std::make_shared<SPA_PluginObserverMock4>(READERS);
    stubPlugin->addObserver(assertDisconnect);

    /* Connect reader */
    stubPlugin->plugStubReaders(READERS, false);

    ASSERT_TRUE(assertDisconnect->mReaderConnected.await(std::chrono::seconds(2)));

    Thread::sleep(1000);

    stubPlugin->unplugStubReaders(READERS, false);

    ASSERT_TRUE(assertDisconnect->mReaderDisconnected.await(std::chrono::seconds(2)));

    Thread::sleep(1000);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 0);
    ASSERT_EQ(assertDisconnect->mReaderConnected.getCount(), 0);
    ASSERT_EQ(assertDisconnect->mReaderDisconnected.getCount(), 0);

    tearDown();
}

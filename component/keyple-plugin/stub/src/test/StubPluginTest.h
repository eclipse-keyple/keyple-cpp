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

#pragma once

#include "BaseStubTest.h"
#include "StubPlugin.h"
#include "ObservablePlugin.h"
#include "ObservableReaderNotificationEngine.h"
#include "CountDownLatch.h"

#include <string>
#include <unordered_set>
#include <stdexcept>
#include <memory>
#include <typeinfo>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

/* Common */
#include "LoggerFactory.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace keyple { namespace core { namespace seproxy { namespace exception { class KeypleReaderException; } } } }
namespace keyple { namespace core { namespace seproxy { namespace event { class PluginEvent; } } } }
namespace keyple { namespace util { class CountDownLatch; } }
namespace keyple { namespace core { namespace seproxy { namespace event { class ObservablePlugin; } } } }

namespace keyple {
namespace plugin {
namespace stub {
using StubPlugin            = keyple::plugin::stub::StubPlugin;
using PluginEvent           = keyple::core::seproxy::event::PluginEvent;
using KeypleReaderException = keyple::core::seproxy::exception::KeypleReaderException;
using Logger                = keyple::common::Logger;
using LoggerFactory         = keyple::common::LoggerFactory;
using ObservablePlugin      = keyple::core::seproxy::event::ObservablePlugin;
using ::testing::Mock;

class StubPluginTest : public BaseStubTest {
public:
    /**
     *
     */
    std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(StubPluginTest));

    /**
     *
     */
    void setUp() override;

    /**
     *
     */
    void tearDown() override;

    /**
     * Plug one reader and count if created
     */
    virtual void testA_PlugOneReaderCount();


    /**
     * Plug one reader and wait for event
     */
    virtual void testA_PlugOneReaderEvent();

// private
    class PluginObserverAnonymousInnerClass
    : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass>,
      public ObservablePlugin::PluginObserver {
    private:
        /**
         *
         */
        std::shared_ptr<StubPluginTest> outerInstance;

        /**
         *
         */
        std::shared_ptr<CountDownLatch> readerConnected;

        /**
         *
         */
        std::string READER_NAME;

    public:
        /**
         *
         */
        PluginObserverAnonymousInnerClass(
            std::shared_ptr<StubPluginTest> outerInstance,
            std::shared_ptr<CountDownLatch> readerConnected,
            const std::string &READER_NAME);

        /**
         *
         */
        void update(std::shared_ptr<PluginEvent> event);
    };

    /**
     * Plug one reader synchronously, an event is raised no matter what
     *
     * @Test public void testA_PlugOneReaderEventSynchronous() throws InterruptedException,
     *       KeypleReaderException { final CountDownLatch readerConnected = new CountDownLatch(1);
     *       final String READER_NAME = "testA_PlugReaders";
     *
     *       // add READER_CONNECTED assert observer stubPlugin.addObserver(new
     *       ObservablePlugin.PluginObserver() {
     * @Override public void update(PluginEvent event) {
     *           Assert.assertEquals(PluginEvent.EventType.READER_CONNECTED, event.getEventType());
     *           Assert.assertEquals(1, event.getReaderNames().size());
     *           Assert.assertEquals(READER_NAME, event.getReaderNames().first());
     *           readerConnected.countDown(); } });
     *
     *           stubPlugin.plugStubReader(READER_NAME, true); readerConnected.await(2,
     *            TimeUnit.SECONDS);
     *
     *           //TODO event should not be raised Assert.assertEquals(1,
     *           stubPlugin.getReaders().size()); Assert.assertEquals(1,
     *           readerConnected.getCount()); }
     */

public:
    /**
     * Unplug one reader and count if removed
     */
    virtual void testA_UnplugOneReaderCount();

    /**
     * Unplug one reader and wait for event
     */
    virtual void testB_UnplugOneReaderEvent();

//private:
    class PluginObserverAnonymousInnerClass2
    : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass2>,
      public ObservablePlugin::PluginObserver {
    private:
        std::shared_ptr<StubPluginTest> outerInstance;

        std::shared_ptr<CountDownLatch> readerConnected;
        std::shared_ptr<CountDownLatch> readerDisconnected;
        std::string READER_NAME;

    public:
        PluginObserverAnonymousInnerClass2(
            std::shared_ptr<StubPluginTest> outerInstance,
            std::shared_ptr<CountDownLatch> readerConnected,
            std::shared_ptr<CountDownLatch> readerDisconnected,
            const std::string &READER_NAME);

        int event_i = 0;

        void update(std::shared_ptr<PluginEvent> event_Renamed);
    };

public:
    /**
     * Plug same reader twice
     */
    virtual void testC_PlugSameReaderTwice();

    /**
     * Get name
     */
    virtual void testD_GetName();

    /**
     * Plug many readers at once and count
     */
    virtual void testE_PlugMultiReadersCount();

    /**
     * Plug many readers at once and wait for event
     */
    virtual void testE_PlugMultiReadersEvent();

//private:
    class PluginObserverAnonymousInnerClass3
    : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass3>,
      public ObservablePlugin::PluginObserver {
    private:
        std::shared_ptr<StubPluginTest> outerInstance;

        const std::set<std::string>& READERS;
        std::shared_ptr<CountDownLatch> readerConnected;

    public:
        PluginObserverAnonymousInnerClass3(
            std::shared_ptr<StubPluginTest> outerInstance,
            const std::set<std::string>& READERS,
            std::shared_ptr<CountDownLatch> readerConnected);

        virtual void update(std::shared_ptr<PluginEvent> event_Renamed);

    };

public:
    /**
     * Plug and unplug many readers at once and count
     */
    virtual void testF_PlugUnplugMultiReadersCount();

    /**
     * Plug and unplug many readers at once and wait for events
     */
    virtual void testF_PlugUnplugMultiReadersEvent();

//private:
public:
    class PluginObserverAnonymousInnerClass4
    : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass4>,
      public ObservablePlugin::PluginObserver
    {
    private:
        std::shared_ptr<StubPluginTest> outerInstance;

        const std::set<std::string>& READERS;
        std::shared_ptr<CountDownLatch> readerConnected;
        std::shared_ptr<CountDownLatch> readerDisconnected;

    public:
        PluginObserverAnonymousInnerClass4(
            std::shared_ptr<StubPluginTest> outerInstance,
            const std::set<std::string>& READERS,
            std::shared_ptr<CountDownLatch> readerConnected,
            std::shared_ptr<CountDownLatch> readerDisconnected);

        int event_i = 0;

        void update(std::shared_ptr<PluginEvent> event_Renamed);
    };

    protected:
    std::shared_ptr<StubPluginTest> shared_from_this()
    {
        return std::static_pointer_cast<StubPluginTest>(
                  BaseStubTest::shared_from_this());
    }
};

}
}
}

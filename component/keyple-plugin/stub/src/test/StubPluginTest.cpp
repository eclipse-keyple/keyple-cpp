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

#include "StubPluginTest.h"

#include "KeypleReaderException.h"
#include "PluginEvent.h"
#include "StubPluginFactory.h"

namespace keyple {
namespace plugin {
namespace stub {

using StubPlugin            = keyple::plugin::stub::StubPlugin;
using ObservablePlugin      = keyple::core::seproxy::event::ObservablePlugin;
using PluginEvent           = keyple::core::seproxy::event::PluginEvent;
using KeypleReaderException =
   keyple::core::seproxy::exception::KeypleReaderException;
using Logger                = keyple::common::Logger;
using LoggerFactory         = keyple::common::LoggerFactory;

using namespace std::chrono; // nanoseconds, system_clock, seconds

void StubPluginTest::setUp()
{
    BaseStubTest::setUp();
}

void StubPluginTest::tearDown()
{
    BaseStubTest::tearDown();
}

void StubPluginTest::testA_PlugOneReaderCount()
{
    const std::string READER_NAME = "testA_PlugOneReaderCount";

    // connect reader
    stubPlugin->plugStubReader(READER_NAME, true);
    ASSERT_EQ(1, stubPlugin->getReaders().size());
}

void StubPluginTest::testA_PlugOneReaderEvent()
{
    std::shared_ptr<CountDownLatch> readerConnected = std::make_shared<CountDownLatch>(1);
    const std::string READER_NAME = "testA_PlugReaders";

    // add READER_CONNECTED assert observer
    stubPlugin->addObserver(
        std::make_shared<PluginObserverAnonymousInnerClass>(
            shared_from_this(), readerConnected, READER_NAME));

    stubPlugin->plugStubReader(READER_NAME, false);
    //readerConnected->await(2, SECONDS );
    sleep( 2 );
    ASSERT_EQ(0, readerConnected->getCount());
}

StubPluginTest::PluginObserverAnonymousInnerClass
    ::PluginObserverAnonymousInnerClass(
        std::shared_ptr<StubPluginTest> outerInstance,
        std::shared_ptr<CountDownLatch> readerConnected,
        const std::string &READER_NAME)
{
    this->outerInstance = outerInstance;
    this->readerConnected = readerConnected;
    this->READER_NAME = READER_NAME;
}

void StubPluginTest::PluginObserverAnonymousInnerClass::update(
    std::shared_ptr<PluginEvent> event_Renamed)
{
    //ASSERT_EQ(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
    ASSERT_EQ(1, event_Renamed->getReaderNames().size());
    std::string Var1 = READER_NAME;
    std::string Var2 = (*event_Renamed->getReaderNames().begin());// first());
    ASSERT_EQ(Var1, Var2 );
    readerConnected->countDown();
}

void StubPluginTest::testA_UnplugOneReaderCount()
{
    const std::string READER_NAME = "testA_UnplugOneReaderCount";
    // connect reader
    stubPlugin->plugStubReader(READER_NAME, true);
    ASSERT_EQ(1, stubPlugin->getReaders().size());
    stubPlugin->unplugStubReader(READER_NAME, true);
    ASSERT_EQ(0, stubPlugin->getReaders().size());
}

void StubPluginTest::testB_UnplugOneReaderEvent()
{
    std::shared_ptr<CountDownLatch> readerConnected =
        std::make_shared<CountDownLatch>(1);
    std::shared_ptr<CountDownLatch> readerDisconnected =
        std::make_shared<CountDownLatch>(1);
    const std::string READER_NAME = "testB_PlugUnplugOneReaders";

    std::shared_ptr<ObservablePlugin::PluginObserver> disconnected_obs =
        std::make_shared<PluginObserverAnonymousInnerClass2>(
            shared_from_this(), readerConnected, readerDisconnected,
            READER_NAME);

    // add READER_DISCONNECTED assert observer
    stubPlugin->addObserver(disconnected_obs);

    // plug a reader
    stubPlugin->plugStubReader(READER_NAME, false);

    //readerConnected->await(2, TimeUnit::SECONDS);

    // unplug reader
    stubPlugin->unplugStubReader(READER_NAME, false);

    // wait for event to be raised
    //readerDisconnected->await(2, TimeUnit::SECONDS);
    ASSERT_EQ(0, readerDisconnected->getCount());
}

StubPluginTest::PluginObserverAnonymousInnerClass2
    ::PluginObserverAnonymousInnerClass2(
        std::shared_ptr<StubPluginTest> outerInstance,
        std::shared_ptr<CountDownLatch> readerConnected,
        std::shared_ptr<CountDownLatch> readerDisconnected,
        const std::string &READER_NAME)
{
    this->outerInstance = outerInstance;
    this->readerConnected = readerConnected;
    this->readerDisconnected = readerDisconnected;
    this->READER_NAME = READER_NAME;
    event_i = 1;
}

void StubPluginTest::PluginObserverAnonymousInnerClass2::update(
    std::shared_ptr<PluginEvent> event_Renamed)
{
    outerInstance->logger->info("event {} {}", event_Renamed->getEventType(),
                                event_Renamed->getReaderNames().size());
    if (event_i == 1)
    {
        //ASSERT_EQ(PluginEvent::EventType::READER_CONNECTED,
        //          event_Renamed->getEventType());
        readerConnected->countDown();
    }
    // analyze the second event, should be a READER_DISCONNECTED
    if (event_i == 2)
    {
        //ASSERT_EQ(PluginEvent::EventType::READER_DISCONNECTED,
        //          event_Renamed->getEventType());
        ASSERT_EQ(1, event_Renamed->getReaderNames().size());
        ASSERT_EQ(READER_NAME, *event_Renamed->getReaderNames().begin());
        readerDisconnected->countDown();
    }
    event_i++;
}

void StubPluginTest::testC_PlugSameReaderTwice()
{
    const std::string READER_NAME = "testC_PlugSameReaderTwice";

    stubPlugin->plugStubReader(READER_NAME, true);
    stubPlugin->plugStubReader(READER_NAME, true);
    logger->debug("Stubplugin readers size %s\n",
                  stubPlugin->getReaders().size());

    ASSERT_EQ(1, stubPlugin->getReaders().size());
}

void StubPluginTest::testD_GetName()
{
    //ASSERT_STREQ( nullptr, stubPlugin->getName() );
}

void StubPluginTest::testE_PlugMultiReadersCount()
{
    const std::set<std::string> READERS_LIST = {
        "E_Reader1", "E_Reader2", "E_Reader3"};

    // connect readers at once
    stubPlugin->plugStubReaders(READERS_LIST, true);
    logger->info("Stub Readers connected %s\n",
                 "stubPlugin.getReaderNames()<fixme>");
    ASSERT_EQ(READERS_LIST, stubPlugin->getReaderNames());
    ASSERT_EQ(3, stubPlugin->getReaders().size());
}

void StubPluginTest::testE_PlugMultiReadersEvent()
{
    std::set<std::string> READERS_LIST = {
        "E_Reader1", "E_Reader2", "E_Reader3"};

    // lock test until message is received
    std::shared_ptr<CountDownLatch> readerConnected =
    std::make_shared<CountDownLatch>(1);

    // add READER_CONNECTED assert observer
    stubPlugin->addObserver(
        std::make_shared<PluginObserverAnonymousInnerClass3>(
            shared_from_this(), READERS_LIST, readerConnected));

    // connect readers
    stubPlugin->plugStubReaders(READERS_LIST, false);

    // wait for event to be raised
    //readerConnected->await(2, TimeUnit::SECONDS);
    ASSERT_EQ(0, readerConnected->getCount());
}

StubPluginTest::PluginObserverAnonymousInnerClass3
    ::PluginObserverAnonymousInnerClass3(
         std::shared_ptr<StubPluginTest> outerInstance,
         const std::set<std::string>& READERS,
         std::shared_ptr<CountDownLatch> readerConnected)
: READERS(READERS)
{
    this->outerInstance = outerInstance;
    this->readerConnected = readerConnected;
}

void StubPluginTest::PluginObserverAnonymousInnerClass3::update(
    std::shared_ptr<PluginEvent> event_Renamed)
{
    outerInstance->logger->info("event %d %d\n", event_Renamed->getEventType(),
                                event_Renamed->getReaderNames().size());

    //ASSERT_EQ(PluginEvent::EventType::READER_CONNECTED,
    //          event_Renamed->getEventType());
    ASSERT_EQ(3, event_Renamed->getReaderNames().size());
    ASSERT_EQ(READERS, event_Renamed->getReaderNames());
    readerConnected->countDown();
}

void StubPluginTest::testF_PlugUnplugMultiReadersCount()
{
    std::set<std::string> READERS_LIST = {
        "FC_Reader1", "FC_Reader2", "FC_Reader3"};

    // connect readers at once
    stubPlugin->plugStubReaders(READERS_LIST, true);
    ASSERT_EQ(3, stubPlugin->getReaders().size());
    stubPlugin->unplugStubReaders(READERS_LIST, true);
    ASSERT_EQ(0, stubPlugin->getReaders().size());
}

void StubPluginTest::testF_PlugUnplugMultiReadersEvent()
{
    std::set<std::string> READERS_LIST = {
        "F_Reader1", "F_Reader2", "F_Reader3"};

    // lock test until message is received
    std::shared_ptr<CountDownLatch> readerConnected =
        std::make_shared<CountDownLatch>(1);
    std::shared_ptr<CountDownLatch> readerDisconnected =
        std::make_shared<CountDownLatch>(1);

    std::shared_ptr<ObservablePlugin::PluginObserver> assertDisconnect =
        std::make_shared<PluginObserverAnonymousInnerClass4>(
            shared_from_this(), READERS_LIST, readerConnected,
            readerDisconnected);

    // add assert DISCONNECT assert observer
    stubPlugin->addObserver(assertDisconnect);

    // connect reader
    stubPlugin->plugStubReaders(READERS_LIST, false);

    //ASSERT_TRUE(readerConnected->await(5, TimeUnit::SECONDS));

    stubPlugin->unplugStubReaders(READERS_LIST, false);

    //ASSERT_TRUE(readerDisconnected->await(5, TimeUnit::SECONDS));

    sleep(1000); // Todo fix me, should works without sleep
    logger->debug("Stub Readers connected {}", stubPlugin->getReaderNames());
    ASSERT_EQ(0, stubPlugin->getReaders().size());
    ASSERT_EQ(0, readerConnected->getCount());
    ASSERT_EQ(0, readerDisconnected->getCount());
}

StubPluginTest::PluginObserverAnonymousInnerClass4
    ::PluginObserverAnonymousInnerClass4(
        std::shared_ptr<StubPluginTest> outerInstance,
        const std::set<std::string>& READERS,
        std::shared_ptr<CountDownLatch> readerConnected,
        std::shared_ptr<CountDownLatch> readerDisconnected)
: READERS(READERS)
{
    this->outerInstance = outerInstance;
    this->readerConnected = readerConnected;
    this->readerDisconnected = readerDisconnected;
    event_i = 1;
}

void StubPluginTest::PluginObserverAnonymousInnerClass4::update(
    std::shared_ptr<PluginEvent> event_Renamed)
{
    outerInstance->logger->info("event %s %s\n", event_Renamed->getEventType(),
                                event_Renamed->getReaderNames().size());
    if (event_i == 1)
    {
        //ASSERT_EQ(PluginEvent::EventType::READER_CONNECTED,
        //          event_Renamed->getEventType());
        readerConnected->countDown();
    }

    // analyze the second event, should be a READER_DISCONNECTED
    if (event_i == 2)
    {
        //ASSERT_EQ(PluginEvent::EventType::READER_DISCONNECTED,
        //          event_Renamed->getEventType());
        ASSERT_EQ(3, event_Renamed->getReaderNames().size());
        ASSERT_EQ(READERS, event_Renamed->getReaderNames());
        readerDisconnected->countDown();
    }
    event_i++;
}

}
}
}

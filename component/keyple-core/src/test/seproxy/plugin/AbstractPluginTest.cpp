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

#include <mutex>
#include <thread>
#include <random>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "AbstractPlugin.h"

#include "AbstractReader.h"
#include "ChannelControl.h"
#include "CountDownLatch.h"
#include "exceptionhelper.h"
#include "InterruptedException.h"
#include "LoggerFactory.h"
#include "MultiSeRequestProcessing.h"
#include "SeProtocol.h"
#include "Thread.h"

using namespace testing;

using namespace keyple::common;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

class PluginMock : public AbstractPlugin {
public:
    PluginMock(const std::string& name) : AbstractPlugin(name) {}

    MOCK_METHOD((const std::map<const std::string, const std::string>),
                getParameters, (), (const, override));

    MOCK_METHOD(void, setParameter, (const std::string& key,
                const std::string& value), (override));

protected:
    MOCK_METHOD(std::set<std::shared_ptr<SeReader>>, initNativeReaders, (),
                (override));
};

class ReaderMock : public AbstractReader {
public:
    ReaderMock(const std::string& pluginName, const std::string& readerName)
    : AbstractReader(pluginName, readerName) {}

    MOCK_METHOD(bool, isSePresent, (), (override));

    MOCK_METHOD(void, addSeProtocolSetting,
                (std::shared_ptr<SeProtocol> seProtocol,
                 const std::string& protocolRule), (override));

    MOCK_METHOD(void, setSeProtocolSetting, 
                ((const std::map<std::shared_ptr<SeProtocol>,
                std::string>& protocolSetting)), (override));
    
    MOCK_METHOD(const TransmissionMode&, getTransmissionMode, (),
                (const, override));

    MOCK_METHOD((const std::map<const std::string, const std::string>),
                getParameters, (), (const, override));

    MOCK_METHOD(void, setParameter, (const std::string& key,
                const std::string& value), (override));

protected:
    MOCK_METHOD(std::list<std::shared_ptr<SeResponse>>, processSeRequestSet,
                (const std::vector<std::shared_ptr<SeRequest>>& requestSet,
                 const MultiSeRequestProcessing& multiSeRequestProcessing,
                 const ChannelControl& channelControl), (override));

    MOCK_METHOD(std::shared_ptr<SeResponse>, processSeRequest, 
                (const std::shared_ptr<SeRequest> seRequest,
                 const ChannelControl& channelControl),
                (override));
};

static std::shared_ptr<Logger> logger =
    LoggerFactory::getLogger(typeid(AbstractPlugin));

static int random(const int& max)
{
    static std::mt19937 eng {
            static_cast<unsigned int>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                    .count())
    };

    return std::uniform_int_distribution<>(0, max)(eng);
}

static void listReaders(std::set<std::shared_ptr<SeReader>>& readers, int n,
                          std::shared_ptr<CountDownLatch> lock)
{
    for (int i = 0; i < n; i++) {
        for (const auto& reader : readers) {
            logger->trace("list, readers: %s, reader %\n", readers.size(),
                          reader->getName());
        }

        try {
            Thread::sleep(10);
        } catch (const InterruptedException& e) {
            (void)e;
        }
    }

    /* If no error, count down latch */
    lock->countDown();
}

static void removeReaderThread(std::set<std::shared_ptr<SeReader>>& readers,
                               int n, std::shared_ptr<CountDownLatch> lock)
{
    for (int i = 0; i < n; i++) {
        try {
            if (readers.begin() != readers.end())
                readers.erase(readers.begin());
            logger->trace("readers: %, remove first reader\n",readers.size());

        } catch (const NoSuchElementException& e) {
            (void)e;

            /* List is empty */
            logger->trace("readers: %, list is empty\n", readers.size());
        }

        try {
            Thread::sleep(10);
        } catch (const InterruptedException& e) {
            (void)e;
        }
    }

    /* If no error, count down latch */
    lock->countDown();
}

static void addReaderThread(std::set<std::shared_ptr<SeReader>>& readers,
                            int n, std::shared_ptr<CountDownLatch> lock)
{
   for (int i = 0; i < n; i++) {
        std::shared_ptr<SeReader> reader =
            std::make_shared<ReaderMock>("pluginName",
                                         std::to_string(random(1000)));
            readers.insert(reader);
            logger->trace("readers: %, add reader %\n", readers.size(),
                          reader->getName());
        try {
            Thread::sleep(10);
        } catch (const InterruptedException& e) {
            (void)e;
        }
   }

    /* If no error, count down latch */
    lock->countDown();
}

TEST(AbstractPluginTest, AbstractPlugin)
{
    PluginMock plugin("plugin");
}

/**
 * Test if readers list does not send ConcurrentModificationException
 * https://keyple.atlassian.net/browse/KEYP-195
 *
 * @throws Throwable
 */
TEST(AbstractPluginTest, addRemoveReadersMultiThreaded)
{
    PluginMock plugin("addRemoveReadersMultiThreaded");
    std::set<std::shared_ptr<SeReader>> readers = plugin.getReaders();
    std::shared_ptr<CountDownLatch> lock = std::make_shared<CountDownLatch>(10);
    std::thread thread[10];

    srand(time(NULL));

    thread[0] = std::thread(addReaderThread, readers, 10, lock);
    thread[1] = std::thread(addReaderThread, readers, 10, lock);
    thread[2] = std::thread(removeReaderThread, readers, 10, lock);
    thread[3] = std::thread(listReaders, readers, 10, lock);
    thread[4] = std::thread(addReaderThread, readers, 10, lock);
    thread[5] = std::thread(removeReaderThread, readers, 10, lock);
    thread[6] = std::thread(listReaders, readers, 10, lock);
    thread[7] = std::thread(removeReaderThread,readers, 10, lock);
    thread[8] = std::thread(listReaders, readers, 10, lock);
    thread[9] = std::thread(removeReaderThread,readers, 10, lock);
    
    /* Wait for all thread to finish with timeout */
    //lock->await(std::chrono::seconds(10));
    for (int i = 0; i < 10; i++)
        thread[i].join();

    /* If all thread finished correctly, lock count should be 0 */
    ASSERT_EQ(static_cast<int>(lock->getCount()), 0);
}

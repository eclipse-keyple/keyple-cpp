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

#include <mutex>
#include <thread>
#include <random>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "AbstractPlugin.h"

#include "AbstractReader.h"
#include "ChannelControl.h"
#include "CountDownLatch.h"
#include "InterruptedException.h"
#include "LoggerFactory.h"
#include "MultiSeRequestProcessing.h"
#include "NoSuchElementException.h"
#include "SeProtocol.h"
#include "Thread.h"

using namespace testing;

using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::protocol;

class AP_AbstractPluginMock : public AbstractPlugin {
public:
    AP_AbstractPluginMock(const std::string& name) : AbstractPlugin(name) {}

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const, override));

    MOCK_METHOD(void,
                setParameter,
                (const std::string&, const std::string&),
                (override));

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

    MOCK_METHOD((const std::string&),
                getName,
                (),
                (const, override));

protected:
    MOCK_METHOD((ConcurrentMap<const std::string, std::shared_ptr<SeReader>>),
                initNativeReaders,
                (),
                (override));
};

class AP_AbstractReaderMock : public AbstractReader {
public:
    AP_AbstractReaderMock(const std::string& pluginName,
                       const std::string& readerName)
    : AbstractReader(pluginName, readerName) {}

    MOCK_METHOD(bool,
                isSePresent,
                (),
                (override));

    MOCK_METHOD(void,
                addSeProtocolSetting,
                (std::shared_ptr<SeProtocol>, const std::string&),
                (override));

    MOCK_METHOD(void,
                setSeProtocolSetting,
                ((const std::map<std::shared_ptr<SeProtocol>, std::string>&)),
                (override));

    MOCK_METHOD(const TransmissionMode&,
                getTransmissionMode,
                (),
                (const, override));

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const, override));

    MOCK_METHOD(void,
                setParameter,
                (const std::string&, const std::string&),
                (override));

    const std::string& getName() const override
    {
        return mName;
    }

protected:
    MOCK_METHOD(std::vector<std::shared_ptr<SeResponse>>,
                processSeRequests,
                (const std::vector<std::shared_ptr<SeRequest>>&,
                 const MultiSeRequestProcessing&, const ChannelControl&),
                (override));

    MOCK_METHOD(std::shared_ptr<SeResponse>,
                processSeRequest,
                (const std::shared_ptr<SeRequest>, const ChannelControl&),
                (override));

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

private:
    const std::string mName = "AP_AbstractReaderMock";
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

static void listReaders(
    ConcurrentMap<const std::string, std::shared_ptr<SeReader>>& readers,
    int n, std::shared_ptr<CountDownLatch> lock)
{
    for (int i = 0; i < n; i++) {
        /* Copy map to avoid listing something that has already changed */
        const ConcurrentMap<const std::string, std::shared_ptr<SeReader>>
            _readers = readers;

        for (const auto& pair : _readers) {
            logger->trace("list, readers: %, reader %\n",
                          _readers.size(), pair.second->getName());
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

static void removeReaderThread(
    ConcurrentMap<const std::string, std::shared_ptr<SeReader>>& readers,
    int n, std::shared_ptr<CountDownLatch> lock)
{
    for (int i = 0; i < n; i++) {
        try {
            if (readers.size() > 0) {
                readers.erase(readers.begin()->first);
                logger->trace("readers: %, remove first reader\n",
                              readers.size());
            } else {
                throw NoSuchElementException("Empty reader list");
            }

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

static void addReaderThread(
    ConcurrentMap<const std::string, std::shared_ptr<SeReader>>& readers,
    int n, std::shared_ptr<CountDownLatch> lock)
{
   for (int i = 0; i < n; i++) {
        std::shared_ptr<SeReader> reader =
            std::make_shared<AP_AbstractReaderMock>(
                "pluginName", std::to_string(random(1000)));
            readers.insert({reader->getName(), reader});
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
    AP_AbstractPluginMock plugin("plugin");
}

/**
 * Test if readers list does not send ConcurrentModificationException
 * https://keyple.atlassian.net/browse/KEYP-195
 *
 * @throws Throwable
 */
TEST(AbstractPluginTest, addRemoveReadersMultiThreaded)
{
    AP_AbstractPluginMock plugin("addRemoveReadersMultiThreaded");
    ConcurrentMap<const std::string, std::shared_ptr<SeReader>>& readers =
        plugin.getReaders();

    std::shared_ptr<CountDownLatch> lock = std::make_shared<CountDownLatch>(10);
    std::thread thread[10];

    thread[0] = std::thread(addReaderThread, std::ref(readers), 10, lock);
    thread[1] = std::thread(addReaderThread, std::ref(readers), 10, lock);
    thread[2] = std::thread(removeReaderThread, std::ref(readers), 10, lock);
    thread[3] = std::thread(listReaders, std::ref(readers), 10, lock);
    thread[4] = std::thread(addReaderThread, std::ref(readers), 10, lock);
    thread[5] = std::thread(removeReaderThread, std::ref(readers), 10, lock);
    thread[6] = std::thread(listReaders, std::ref(readers), 10, lock);
    thread[7] = std::thread(removeReaderThread, std::ref(readers), 10, lock);
    thread[8] = std::thread(listReaders, std::ref(readers), 10, lock);
    thread[9] = std::thread(removeReaderThread, std::ref(readers), 10, lock);

    /* Wait for all thread to finish with timeout */
    //lock->await(std::chrono::seconds(10));
    for (int i = 0; i < 10; i++)
        thread[i].join();

    /* If all thread finished correctly, lock count should be 0 */
    ASSERT_EQ(static_cast<int>(lock->getCount()), 0);
}

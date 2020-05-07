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

#include "CoreBaseTest.h"
#include "AbstractLocalReader.h"
#include "TransmissionMode.h"
#include "ReaderEvent.h"
#include "SelectionStatus.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <memory>

namespace keyple {
namespace core {
namespace seproxy {
namespace event_Renamed
{
    class ReaderEvent;
}
}
}
}


namespace keyple {
namespace core {
namespace seproxy {
namespace event_Renamed
{
    class ReaderObserver;
}
}
}
}


namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
class AbstractObservableReader;
}
}
}
}


namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {
class SeProtocol;
}
}
}
}


namespace keyple {
namespace core {
namespace seproxy {
class SeSelector;
}
}
}


namespace keyple {
namespace core {
namespace seproxy {
namespace message {
class SelectionStatus;
}
}
}
}

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using CoreBaseTest = keyple::core::CoreBaseTest;
using ObservableReader = keyple::core::seproxy::event::ObservableReader;
using namespace keyple::core::seproxy::exception;

/**
 * Test methods linked to observability
 */
class AbstractObservableReaderTest : public CoreBaseTest {

private:
    static const std::shared_ptr<Logger> logger;

public:
    const std::string PLUGIN_NAME = "abstractObservablePluginTest";
    const std::string READER_NAME = "abstractObservableReaderTest";

    const std::shared_ptr<ObservableReader::ReaderObserver> obs1 =
        getObserver();
    const std::shared_ptr<ObservableReader::ReaderObserver> obs2 =
        getObserver();

    std::shared_ptr<AbstractObservableReader> spyReader;

    std::shared_ptr<CountDownLatch> startObservationCall;
    std::shared_ptr<CountDownLatch> stopObservationCall;

    virtual void setUp();

    /*
     * TESTS
     */

    virtual void testAddObserver();

    virtual void testRemoveObserver();

    virtual void testAddRemoveObserver();

    /*
     * HELPERS
     */

    virtual std::shared_ptr<AbstractObservableReader>
    getBlankAbstractObservableReader(const std::string& pluginName,
                                     const std::string& readerName);

private:
    class AbstractLocalReaderAnonymousInnerClass : public AbstractLocalReader {
    private:
        std::shared_ptr<AbstractObservableReaderTest> outerInstance;

    public:
        AbstractLocalReaderAnonymousInnerClass(
            std::shared_ptr<AbstractObservableReaderTest> outerInstance,
            const std::string& pluginName, const std::string& readerName);

    protected:
        void startObservation() override;

        void stopObservation() override;

        void closePhysicalChannel() throw(KeypleChannelStateException) override;

        bool isPhysicalChannelOpen() override;

        std::vector<char> transmitApdu(std::vector<char>& apduIn) throw(
            KeypleIOReaderException) override;

        bool
        protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) throw(
            KeypleReaderException) override;

        bool checkSePresence() throw(NoStackTraceThrowable) override;

        std::vector<char> getATR() override;

        std::shared_ptr<SelectionStatus>
        openLogicalChannel(std::shared_ptr<SeSelector> selector) override;

        void openPhysicalChannel() throw(KeypleChannelStateException) override;

    public:
        TransmissionMode getTransmissionMode() override;

        std::unordered_map<std::string, std::string> getParameters() override;

        void
        setParameter(const std::string& key, const std::string& value) throw(
            std::invalid_argument, KeypleBaseException) override;

    protected:
        std::shared_ptr<AbstractLocalReaderAnonymousInnerClass>
        shared_from_this()
        {
            return std::static_pointer_cast<
                AbstractLocalReaderAnonymousInnerClass>(
                AbstractLocalReader::shared_from_this());
        }
    };

public:
    virtual std::shared_ptr<ObservableReader::ReaderObserver> getObserver();

private:
    class ReaderObserverAnonymousInnerClass
    : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass>,
      public ObservableReader::ReaderObserver {
    private:
        std::shared_ptr<AbstractObservableReaderTest> outerInstance;

    public:
        ReaderObserverAnonymousInnerClass(
            std::shared_ptr<AbstractObservableReaderTest> outerInstance);

        void update(std::shared_ptr<ReaderEvent> readerEvent);
    };

public:
    virtual void initSpyReader();

private:
    class AnswerAnonymousInnerClass : public Answer {
    private:
        std::shared_ptr<AbstractObservableReaderTest> outerInstance;

    public:
        AnswerAnonymousInnerClass(
            std::shared_ptr<AbstractObservableReaderTest> outerInstance);

        std::shared_ptr<void>
        answer(std::shared_ptr<InvocationOnMock> invocation) throw(
            std::runtime_error) override;

    protected:
        std::shared_ptr<AnswerAnonymousInnerClass> shared_from_this()
        {
            return std::static_pointer_cast<AnswerAnonymousInnerClass>(
                org.mockito.stubbing.Answer::shared_from_this());
        }
    };

private:
    class AnswerAnonymousInnerClass2 : public Answer {
    private:
        std::shared_ptr<AbstractObservableReaderTest> outerInstance;

    public:
        AnswerAnonymousInnerClass2(
            std::shared_ptr<AbstractObservableReaderTest> outerInstance);

        std::shared_ptr<void>
        answer(std::shared_ptr<InvocationOnMock> invocation) throw(
            std::runtime_error) override;

    protected:
        std::shared_ptr<AnswerAnonymousInnerClass2> shared_from_this()
        {
            return std::static_pointer_cast<AnswerAnonymousInnerClass2>(
                org.mockito.stubbing.Answer::shared_from_this());
        }
    };

protected:
    std::shared_ptr<AbstractObservableReaderTest> shared_from_this()
    {
        return std::static_pointer_cast<AbstractObservableReaderTest>(
            org.eclipse.keyple.CoreBaseTest::shared_from_this());
    }
};

}
}
}
}

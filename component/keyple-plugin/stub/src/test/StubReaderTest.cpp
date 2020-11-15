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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "StubReader.h"

/* Calypso */
#include "IncreaseCmdBuild.h"
#include "PoSelectionRequest.h"
#include "PoSelector.h"
#include "ReadRecordsCmdBuild.h"

/* Common */
#include "CountDownLatch.h"
#include "KeypleStd.h"
#include "System.h"
#include "Thread.h"

/* Core */
#include "AbstractSeSelectionRequest.h"
#include "ByteArrayUtil.h"
#include "ChannelControl.h"
#include "KeypleReaderException.h"
#include "MultiSeRequestProcessing.h"
#include "SeCommonProtocols.h"
#include "SeSelection.h"

/* Plugin */
#include "StubPluginImpl.h"
#include "StubPluginFactory.h"
#include "StubProtocolSetting.h"
#include "StubReader.h"

using namespace testing;

using namespace keyple::calypso::transaction;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::common;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::util;
using namespace keyple::plugin::stub;

class StubReaderTest {};

static const std::string PLUGIN_NAME = "stub1";
static const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(StubReaderTest));

static std::shared_ptr<StubPluginImpl> setUp()
{
    auto factory = std::make_shared<StubPluginFactory>(PLUGIN_NAME);

    return std::dynamic_pointer_cast<StubPluginImpl>(factory->getPlugin());
}

static void tearDown(std::shared_ptr<StubPluginImpl> stubPlugin,
                     std::shared_ptr<ObservableReader::ReaderObserver> readerObs)
{
    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));
    stubPlugin->clearObservers();
    reader->removeObserver(readerObs);
    ASSERT_EQ(reader->countObservers(), 0);
    stubPlugin->unplugStubReader("StubReaderTest", true);

    readerObs = nullptr;
    stubPlugin = nullptr;
}

class SRT_StubSecureElementMock_hopLinkSe final : public StubSecureElement {
public:
    std::vector<uint8_t> processApdu(const std::vector<uint8_t>& apduIn) override
    {
        addHexCommand("00 A4 04 00 0A A0 00 00 02 91 A0 00 00 01 91 00",
                      "6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C14" \
                      "10019000");
        addHexCommand("00 A4 04 00 0A A0 00 00 02 91 A0 00 00 01 92 00", "6A82");
        addHexCommand("00 B2 01 A4 20",
                      "00000000000000000000000000000000000000000000000000000000000000009000");

        return StubSecureElement::processApdu(apduIn);
    }

    const std::vector<uint8_t>& getATR() override
    {
        return mAtr;
    }

    std::string getSeProcotol() override
    {
        return "PROTOCOL_ISO14443_4";
    }

private:
    const std::vector<uint8_t> mAtr =
        ByteArrayUtil::fromHex("3B 8E 80 01 80 31 80 66 40 90 89 12 08 02 83 01 90 00 0B");
};

static std::shared_ptr<StubSecureElement> hoplinkSE()
{
     return std::make_shared<SRT_StubSecureElementMock_hopLinkSe>();
}

class SRT_StubSecureElementMock_revision1SE final : public StubSecureElement {
public:
    std::vector<uint8_t> processApdu(const std::vector<uint8_t>& apduIn) override
    {
        addHexCommand("00 A4 04 00 0A A0 00 00 02 91 A0 00 00 01 91 00",
                      "6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C14" \
                      "10019000");
        addHexCommand("00 A4 04 00 0A A0 00 00 02 91 A0 00 00 01 92 00", "6A82");
        addHexCommand("00 B2 01 A4 20",
                      "00000000000000000000000000000000000000000000000000000000000000009000");

        return StubSecureElement::processApdu(apduIn);
    }

    const std::vector<uint8_t>& getATR() override
    {
        return mAtr;
    }

    std::string getSeProcotol() override
    {
        return "PROTOCOL_B_PRIME";
    }

private:
    const std::vector<uint8_t> mAtr =
        ByteArrayUtil::fromHex("3b 8f 80 01 80 5a 08 03 04 00 02 00 21 72 90 ff 82 90 00 f3");
};

static std::shared_ptr<StubSecureElement> revision1SE()
{
    return std::make_shared<SRT_StubSecureElementMock_revision1SE>();
}

class SRT_StubSecureElementMock_noApduResponseSE final : public StubSecureElement {
public:
    std::vector<uint8_t> processApdu(const std::vector<uint8_t>& apduIn) override
    {
        addHexCommand("00 A4 04 00 0A A0 00 00 02 91 A0 00 00 01 91 00",
                      "6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C14" \
                      "10019000");

        return StubSecureElement::processApdu(apduIn);
    }

    const std::vector<uint8_t>& getATR() override
    {
        return mAtr;
    }

    std::string getSeProcotol() override
    {
        return "PROTOCOL_ISO14443_4";
    }

private:
    const std::vector<uint8_t> mAtr =
        ByteArrayUtil::fromHex("3B 8E 80 01 80 31 80 66 40 90 89 12 08 02 83 01 90 00 0B");
};

static std::shared_ptr<StubSecureElement> noApduResponseSE()
{
    return std::make_shared<SRT_StubSecureElementMock_noApduResponseSE>();
}

class SRT_StubSecureElementMock_partialSE final : public StubSecureElement {
public:
    std::vector<uint8_t> processApdu(const std::vector<uint8_t>& apduIn) override
    {
        addHexCommand("00 A4 04 00 0A A0 00 00 02 91 A0 00 00 01 91 00",
                      "6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C14" \
                      "10019000");
        addHexCommand("00 B2 01 A4 00",
                      "00000000000000000000000000000000000000000000000000000000009000");

        return StubSecureElement::processApdu(apduIn);
    }

    const std::vector<uint8_t>& getATR() override
    {
        return mAtr;
    }

    std::string getSeProcotol() override
    {
        return "PROTOCOL_ISO14443_4";
    }

private:
    const std::vector<uint8_t> mAtr =
        ByteArrayUtil::fromHex("3B 8E 80 01 80 31 80 66 40 90 89 12 08 02 83 01 90 00 0B");
};

static std::shared_ptr<StubSecureElement> partialSE()
{
    return std::make_shared<SRT_StubSecureElementMock_partialSE>();
}

static std::vector<std::shared_ptr<SeRequest>> getRequestIsoDepSetSample()
{
    //const std::string poAid = "A000000291A000000191";

    ReadRecordsCmdBuild poReadRecordCmd_T2Env(PoClass::ISO,
                                              0x14,
                                              1,
                                              ReadRecordsCmdBuild::ReadMode::ONE_RECORD,
                                              32);

    std::vector<std::shared_ptr<ApduRequest>> poApduRequests;
    poApduRequests.push_back(poReadRecordCmd_T2Env.getApduRequest());
    auto seRequest = std::make_shared<SeRequest>(poApduRequests);

    std::vector<std::shared_ptr<SeRequest>> seRequests;
    seRequests.push_back(seRequest);

    return seRequests;
}

/*
 * No Response: increase command is not defined in the StubSE
 *
 * An Exception will be thrown.
 */
static std::vector<std::shared_ptr<SeRequest>> getNoResponseRequest()
{
    IncreaseCmdBuild poIncreaseCmdBuild(PoClass::ISO, 0x14, 0x01, 0);

    std::vector<std::shared_ptr<ApduRequest>> poApduRequests;
    poApduRequests.push_back(poIncreaseCmdBuild.getApduRequest());
    auto seRequest = std::make_shared<SeRequest>(poApduRequests);

    std::vector<std::shared_ptr<SeRequest>> seRequests;
    seRequests.push_back(seRequest);

    return seRequests;
}

/*
 * Partial response set: multiple read records commands, one is not defined in the StubSE
 *
 * An Exception will be thrown.
 */
static std::vector<std::shared_ptr<SeRequest>> getPartialRequestList(int scenario)
{
    // const std::string poAid = "A000000291A000000191";

    ReadRecordsCmdBuild poReadRecord1CmdBuild(PoClass::ISO,
                                              0x14,
                                              1,
                                              ReadRecordsCmdBuild::ReadMode::ONE_RECORD,
                                              0);

    /* This command doesn't in the PartialSE */
    ReadRecordsCmdBuild poReadRecord2CmdBuild(PoClass::ISO,
                                              0x1E,
                                              1,
                                              ReadRecordsCmdBuild::ReadMode::ONE_RECORD,
                                              0);

    std::vector<std::shared_ptr<ApduRequest>> poApduRequests1;
    poApduRequests1.push_back(poReadRecord1CmdBuild.getApduRequest());
    poApduRequests1.push_back(poReadRecord1CmdBuild.getApduRequest());
    poApduRequests1.push_back(poReadRecord1CmdBuild.getApduRequest());
    poApduRequests1.push_back(poReadRecord1CmdBuild.getApduRequest());

    std::vector<std::shared_ptr<ApduRequest>> poApduRequests2;
    poApduRequests2.push_back(poReadRecord1CmdBuild.getApduRequest());
    poApduRequests2.push_back(poReadRecord1CmdBuild.getApduRequest());
    poApduRequests2.push_back(poReadRecord1CmdBuild.getApduRequest());
    poApduRequests2.push_back(poReadRecord1CmdBuild.getApduRequest());

    std::vector<std::shared_ptr<ApduRequest>> poApduRequests3;
    poApduRequests3.push_back(poReadRecord1CmdBuild.getApduRequest());
    poApduRequests3.push_back(poReadRecord1CmdBuild.getApduRequest());
    poApduRequests3.push_back(poReadRecord2CmdBuild.getApduRequest());
    poApduRequests3.push_back(poReadRecord1CmdBuild.getApduRequest());

    auto seRequest1 = std::make_shared<SeRequest>(poApduRequests1);
    auto seRequest2 = std::make_shared<SeRequest>(poApduRequests2);
    /* This SeRequest fails at step 3 */
    auto seRequest3 = std::make_shared<SeRequest>(poApduRequests3);
    auto seRequest4 = std::make_shared<SeRequest>(poApduRequests1);

    std::vector<std::shared_ptr<SeRequest>> seRequests;

    switch (scenario) {
    case 0:
        /* 0 response */
        seRequests.push_back(seRequest3); // fails
        seRequests.push_back(seRequest1); // succeeds
        seRequests.push_back(seRequest2); // succeeds
        break;
    case 1:
        /* 1 response */
        seRequests.push_back(seRequest1); // succeeds
        seRequests.push_back(seRequest3); // fails
        seRequests.push_back(seRequest2); // succeeds
        break;
    case 2:
        /* 2 responses */
        seRequests.push_back(seRequest1); // succeeds
        seRequests.push_back(seRequest2); // succeeds
        seRequests.push_back(seRequest3); // fails
        break;
    case 3:
        /* 3 responses */
        seRequests.push_back(seRequest1); // succeeds
        seRequests.push_back(seRequest2); // succeeds
        seRequests.push_back(seRequest4); // succeeds
        break;
    default:
        break;
    }

    return seRequests;
}


/*
 * Partial response: multiple read records commands, one is not defined in the StubSE
 *
 * An Exception will be thrown.
 */
static std::shared_ptr<SeRequest> getPartialRequest(int scenario)
{
    //const std::string poAid = "A000000291A000000191";

    ReadRecordsCmdBuild poReadRecord1CmdBuild(PoClass::ISO,
                                              0x14,
                                              1,
                                              ReadRecordsCmdBuild::ReadMode::ONE_RECORD,
                                              0);

    /* This command doesn't in the PartialSE */
    ReadRecordsCmdBuild poReadRecord2CmdBuild(PoClass::ISO,
                                              0x1E,
                                              1,
                                              ReadRecordsCmdBuild::ReadMode::ONE_RECORD,
                                              0);

    std::vector<std::shared_ptr<ApduRequest>> poApduRequests;

    switch (scenario) {
    case 0:
        poApduRequests.push_back(poReadRecord2CmdBuild.getApduRequest()); // fails
        poApduRequests.push_back(poReadRecord1CmdBuild.getApduRequest()); // succeeds
        poApduRequests.push_back(poReadRecord1CmdBuild.getApduRequest()); // succeeds
        break;
    case 1:
        poApduRequests.push_back(poReadRecord1CmdBuild.getApduRequest()); // succeeds
        poApduRequests.push_back(poReadRecord2CmdBuild.getApduRequest()); // fails
        poApduRequests.push_back(poReadRecord1CmdBuild.getApduRequest()); // succeeds
        break;
    case 2:
        poApduRequests.push_back(poReadRecord1CmdBuild.getApduRequest()); // succeeds
        poApduRequests.push_back(poReadRecord1CmdBuild.getApduRequest()); // succeeds
        poApduRequests.push_back(poReadRecord2CmdBuild.getApduRequest()); // fails
        break;
    case 3:
        poApduRequests.push_back(poReadRecord1CmdBuild.getApduRequest()); // succeeds
        poApduRequests.push_back(poReadRecord1CmdBuild.getApduRequest()); // succeeds
        poApduRequests.push_back(poReadRecord1CmdBuild.getApduRequest()); // succeeds
        break;
    default:
        break;
    }

    return std::make_shared<SeRequest>(poApduRequests);
}

class GenericMatchingSe final : public AbstractMatchingSe {
public:
    GenericMatchingSe(std::shared_ptr<SeResponse> selectionResponse,
                      TransmissionMode transmissionMode)
    : AbstractMatchingSe(selectionResponse, transmissionMode) {}
};

class GenericSeSelectionRequest final
: public AbstractSeSelectionRequest<AbstractApduCommandBuilder> {
public:
    GenericSeSelectionRequest(std::shared_ptr<SeSelector> seSelector)
    : AbstractSeSelectionRequest(seSelector),
      mTransmissionMode(seSelector->getSeProtocol()->getTransmissionMode()) {}

protected:
        const std::shared_ptr<AbstractMatchingSe> parse(std::shared_ptr<SeResponse> seResponse)
            override
        {
            return std::make_shared<GenericMatchingSe>(seResponse, mTransmissionMode);
        }

private:
    TransmissionMode mTransmissionMode;
};

static void genericSelectSe(std::shared_ptr<SeReader> reader)
{
   SeSelection seSelection;

    // SeSelection seSelection = new SeSelection(MultiSeRequestProcessing.PROCESS_ALL,
    // ChannelControl.CLOSE_AFTER);

    auto atrFilter = std::make_shared<SeSelector::AtrFilter>("3B.*");
    auto seSelector = SeSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                            .atrFilter(atrFilter)
                                            .build();
    auto genericSeSelectionRequest = std::make_shared<GenericSeSelectionRequest>(seSelector);

    /* Prepare selector, ignore AbstractMatchingSe here */
    seSelection.prepareSelection(genericSeSelectionRequest);

    try {
        seSelection.processExplicitSelection(reader);
    } catch (const KeypleException& e) {
        (void)e;
        FAIL();
    }
}

class SRT_ReaderObserverMock1 final : public ObservableReader::ReaderObserver {
public:
    SRT_ReaderObserverMock1(std::shared_ptr<StubReader> reader,
                            std::shared_ptr<StubPluginImpl> plugin)
    : mLock(1), mReader(reader), mPlugin(plugin) {}

    void update(std::shared_ptr<ReaderEvent> event) override
    {
        ASSERT_EQ(event->getReaderName(), mReader->getName());
        ASSERT_EQ(event->getPluginName(), mPlugin->getName());
        ASSERT_EQ(event->getEventType(),  ReaderEvent::EventType::SE_INSERTED);

        mLock.countDown();
    }

    CountDownLatch mLock;

private:
    std::shared_ptr<StubReader> mReader;
    std::shared_ptr<StubPluginImpl> mPlugin;
};

/**
 * Insert SE check : event and se presence
 *
 * @throws InterruptedException
 */
TEST(StubReaderTest, testInsert)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    ASSERT_FALSE(reader->isSePresent());

    readerObs = std::make_shared<SRT_ReaderObserverMock1>(reader, stubPlugin);

    /* Add observer */
    reader->addObserver(readerObs);

    /* Test */
    reader->insertSe(hoplinkSE());

    /* Lock thread for 2 seconds max to wait for the event */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock1>(readerObs)
        ->mLock.await(std::chrono::seconds(2));

    ASSERT_EQ(static_cast<int>(
                  std::dynamic_pointer_cast<SRT_ReaderObserverMock1>(readerObs)->mLock.getCount()),
              0);
    ASSERT_TRUE(reader->isSePresent());

    tearDown(stubPlugin, readerObs);
}

class SRT_ReaderObserverMock2 final : public ObservableReader::ReaderObserver {
public:
    SRT_ReaderObserverMock2(std::shared_ptr<StubReader> reader,
                            std::shared_ptr<StubPluginImpl> plugin)
    : mInsertLock(1), mRemoveLock(1), mReader(reader), mPlugin(plugin) {}

    void update(std::shared_ptr<ReaderEvent> event) override
    {
        /* First event */
        if (event_i == 1) {
            ASSERT_EQ(event->getReaderName(), mReader->getName());
            ASSERT_EQ(event->getPluginName(), mPlugin->getName());
            ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_INSERTED);
            mInsertLock.countDown();
        }

        /* Analyze the second event, should be a SE_REMOVED */
        if (event_i == 2) {
            ASSERT_EQ(event->getReaderName(), mReader->getName());
            ASSERT_EQ(event->getPluginName(), mPlugin->getName());
            ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_REMOVED);
            mRemoveLock.countDown();
        }
        event_i++;
    }

    CountDownLatch mInsertLock;
    CountDownLatch mRemoveLock;

private:
    std::shared_ptr<StubReader> mReader;
    std::shared_ptr<StubPluginImpl> mPlugin;
    int event_i = 1;
};

/**
 * Remove SE check : event and se presence
 *
 * @throws InterruptedException
 */
TEST(StubReaderTest, testRemove)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Create observer */
    readerObs = std::make_shared<SRT_ReaderObserverMock2>(reader, stubPlugin);

    /* Add observer */
    reader->addObserver(readerObs);
    reader->startSeDetection(ObservableReader::PollingMode::REPEATING);

    /* Test */
    reader->insertSe(hoplinkSE());

    /* Lock thread for 2 seconds max to wait for the event SE_INSERTED */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock2>(readerObs)
        ->mInsertLock.await(std::chrono::seconds(2));

    /* Should be 0 because insertLock is countDown by obs */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock2>(readerObs)
                                   ->mInsertLock.getCount()),
              0);

    auto proxy = std::dynamic_pointer_cast<ProxyReader>(reader);
    proxy->transmitSeRequest({}, ChannelControl::CLOSE_AFTER);
    reader->removeSe();

    /* Lock thread for 2 seconds max to wait for the event SE_REMOVED */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock2>(readerObs)
        ->mRemoveLock.await(std::chrono::seconds(2));

    /* Should be 0 because removeLock is countDown by obs */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock2>(readerObs)
                                  ->mRemoveLock.getCount()), 0);

    ASSERT_FALSE(reader->isSePresent());

    tearDown(stubPlugin, readerObs);
}

class SRT_ReaderObserverMock3 final : public ObservableReader::ReaderObserver {
public:
    SRT_ReaderObserverMock3(std::shared_ptr<StubReader> reader,
                            std::shared_ptr<StubPluginImpl> plugin)
    : mFirstInsertLock(1),
      mFirstRemoveLock(1),
      mSecondInsertLock(1),
      mSecondRemoveLock(1),
      mReader(reader),
      mPlugin(plugin) {}

    void update(std::shared_ptr<ReaderEvent> event) override
    {
        /* First event */
        if (event_i == 1) {
            ASSERT_EQ(event->getReaderName(), mReader->getName());
            ASSERT_EQ(event->getPluginName(), mPlugin->getName());
            ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_INSERTED);
            mFirstInsertLock.countDown();
        }

        // analyze the second event, should be a SE_REMOVED
        if (event_i == 2) {
            ASSERT_EQ(event->getReaderName(), mReader->getName());
            ASSERT_EQ(event->getPluginName(), mPlugin->getName());
            ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_REMOVED);
            mFirstRemoveLock.countDown();
        }
        if (event_i == 3) {
            ASSERT_EQ(event->getReaderName(), mReader->getName());
            ASSERT_EQ(event->getPluginName(), mPlugin->getName());
            ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_INSERTED);
            mSecondInsertLock.countDown();
        }
        if (event_i == 4) {
            ASSERT_EQ(event->getReaderName(), mReader->getName());
            ASSERT_EQ(event->getPluginName(), mPlugin->getName());
            ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_REMOVED);
            mSecondRemoveLock.countDown();
        }
        event_i++;
    }

    CountDownLatch mFirstInsertLock;
    CountDownLatch mFirstRemoveLock;
    CountDownLatch mSecondInsertLock;
    CountDownLatch mSecondRemoveLock;

private:
    std::shared_ptr<StubReader> mReader;
    std::shared_ptr<StubPluginImpl> mPlugin;
    int event_i = 1;
};

/**
 * Remove SE check : event and se presence
 *
 * @throws InterruptedException
 */
TEST(StubReaderTest, A_testInsertRemoveTwice)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Create observer */
    readerObs = std::make_shared<SRT_ReaderObserverMock3>(reader, stubPlugin);

    /* Add observer */
    reader->addObserver(readerObs);

    /* Set PollingMode to Continue */
    reader->startSeDetection(ObservableReader::PollingMode::REPEATING);

    /* Test first sequence */
    reader->insertSe(hoplinkSE());

    // Thread.sleep(200);

    /* Lock thread for 2 seconds max to wait for the event SE_INSERTED */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
        ->mFirstInsertLock.await(std::chrono::seconds(2));

    /* Should be 0 because insertLock is countDown by obs */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
                                   ->mFirstInsertLock.getCount()),
              0);

    // Thread.sleep(1000);

    auto proxy = std::dynamic_pointer_cast<ProxyReader>(reader);
    proxy->transmitSeRequest({}, ChannelControl::CLOSE_AFTER);
    reader->removeSe();

    /* Lock thread for 2 seconds max to wait for the event SE_REMOVED */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
        ->mFirstRemoveLock.await(std::chrono::seconds(2));

    /* Should be 0 because removeLock is countDown by obs */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
                                   ->mFirstRemoveLock.getCount()),
              0);

    /*
     * BUG, insert event is not throw without (1)
     * BUG (1) make thread sleep
     * BUG, solved by setting a lower threadWaitTimeout (100ms)
     */
    // Thread.sleep(1000);

    /* Test second sequence */
    reader->insertSe(hoplinkSE());

    /* Mock thread for 2 seconds max to wait for the event SE_INSERTED */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
        ->mSecondInsertLock.await(std::chrono::seconds(2));


    /* Should be 0 because insertLock is countDown by obs */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
                                   ->mSecondInsertLock.getCount()),
              0);

    proxy->transmitSeRequest({}, ChannelControl::CLOSE_AFTER);

    // Thread.sleep(1000);

    reader->removeSe();

    /* Lock thread for 2 seconds max to wait for the event SE_REMOVED */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
        ->mSecondRemoveLock.await(std::chrono::seconds(2));

    /* Should be 0 because removeLock is countDown by obs */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
                                   ->mSecondRemoveLock.getCount()),
              0);

    ASSERT_FALSE(reader->isSePresent());

    tearDown(stubPlugin, readerObs);
}

TEST(StubReaderTest, A_testInsertRemoveTwiceFast)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Create observer */
    readerObs = std::make_shared<SRT_ReaderObserverMock3>(reader, stubPlugin);

    /* Add observer */
    reader->addObserver(readerObs);

    /* Set PollingMode to Continue */
    reader->startSeDetection(ObservableReader::PollingMode::REPEATING);

    /* Test first sequence */
    reader->insertSe(hoplinkSE());

    /* Lock thread for 2 seconds max to wait to the event SE_INSERTED */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
        ->mFirstInsertLock.await(std::chrono::seconds(2));

    /* Should be 0 because removeLock is countDown by obs */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
                                   ->mFirstInsertLock.getCount()),
              0);

    auto proxy = std::dynamic_pointer_cast<ProxyReader>(reader);
    proxy->transmitSeRequest({}, ChannelControl::CLOSE_AFTER);
    reader->removeSe();

    /* Lock thread for 2 seconds max to wait for the event SE_REMOVED */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
        ->mFirstRemoveLock.await(std::chrono::seconds(2));

    /* Should be 0 because removeLock is countDown by obs */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
                                   ->mFirstRemoveLock.getCount()),
              0);

    /*
     * BUG, insert event is not throw without (1)
     * BUG (1) make thread sleep
     * BUG, solved by setting a lower threadWaitTimeout (100ms)
     */

    /* Test second sequence */
    reader->insertSe(hoplinkSE());

    /* Lock thread for 2 seconds max to wait to the event SE_INSERTED */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
        ->mSecondInsertLock.await(std::chrono::seconds(2));

    /* Should be 0 because removeLock is countDown by obs */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
                                   ->mSecondInsertLock.getCount()),
              0);

    proxy->transmitSeRequest({}, ChannelControl::CLOSE_AFTER);
    reader->removeSe();

    /* Lock thread for 2 seconds max to wait for the event SE_REMOVED */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
        ->mSecondRemoveLock.await(std::chrono::seconds(2));

    /* Should be 0 because removeLock is countDown by obs */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock3>(readerObs)
                                   ->mSecondRemoveLock.getCount()),
              0);

    ASSERT_FALSE(reader->isSePresent());

    tearDown(stubPlugin, readerObs);
}

class SRT_ReaderObserverMock4 final : public ObservableReader::ReaderObserver {
public:
    SRT_ReaderObserverMock4(std::shared_ptr<StubReader> reader,
                            std::shared_ptr<StubPluginImpl> plugin,
                            const std::string& poAid)
    : mLock(1), mReader(reader), mPlugin(plugin), mPoAid(poAid) {}

    void update(std::shared_ptr<ReaderEvent> event) override
    {
        ASSERT_EQ(event->getReaderName(), mReader->getName());
        ASSERT_EQ(event->getPluginName(), mPlugin->getName());
        ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_MATCHED);
        ASSERT_TRUE(event->getDefaultSelectionsResponse()
                         ->getSelectionSeResponses()[0]
                         ->getSelectionStatus()
                         ->hasMatched());
        ASSERT_EQ(event->getDefaultSelectionsResponse()
                       ->getSelectionSeResponses()[0]
                       ->getSelectionStatus()
                       ->getAtr()
                       ->getBytes(),
                  hoplinkSE()->getATR());

        /*
         * Retrieve the expected FCI from the Stub SE running the select
         * application command
         */
        const std::vector<uint8_t> aid = ByteArrayUtil::fromHex(mPoAid);
        std::vector<uint8_t> selectApplicationCommand(6 + aid.size());
        selectApplicationCommand[0] = 0x00; // CLA
        selectApplicationCommand[1] = 0xA4; // INS
        selectApplicationCommand[2] = 0x04; // P1: select by name
        selectApplicationCommand[3] = 0x00; // P2: requests the first
        selectApplicationCommand[4] = static_cast<uint8_t>(aid.size()); // Lc
        System::arraycopy(aid, 0, selectApplicationCommand, 5, aid.size()); // data

        selectApplicationCommand[5 + aid.size()] = 0x00; // Le
        std::vector<uint8_t> fci;
        try {
            fci = hoplinkSE()->processApdu(selectApplicationCommand);
        } catch (const KeypleReaderIOException& e) {
            (void)e;
        }

        ASSERT_EQ(event->getDefaultSelectionsResponse()
                       ->getSelectionSeResponses()[0]
                       ->getSelectionStatus()
                       ->getFci()
                       ->getBytes(),
                  fci);

        /* Unlock thread */
        mLock.countDown();
    }

    CountDownLatch mLock;

private:
    std::shared_ptr<StubReader> mReader;
    std::shared_ptr<StubPluginImpl> mPlugin;
    const std::string mPoAid;
};

TEST(StubReaderTest, testInsertMatchingSe)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Add Protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

    /* Create observer */
    const std::string poAid = "A000000291A000000191";
    readerObs = std::make_shared<SRT_ReaderObserverMock4>(reader, stubPlugin, poAid);

    /* Add observer */
    reader->addObserver(readerObs);

    SeSelection seSelection;

    auto aidSelector = SeSelector::AidSelector::builder()->aidToSelect(poAid).build();
    auto seSelector = PoSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                            .aidSelector(aidSelector)
                                            .invalidatedPo(PoSelector::InvalidatedPo::REJECT)
                                            .build();
    auto poSelector =  std::dynamic_pointer_cast<PoSelector>(seSelector);
    auto poSelectionRequest = std::make_shared<PoSelectionRequest>(poSelector);
    auto selection =
        std::reinterpret_pointer_cast<AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(
            poSelectionRequest);

    seSelection.prepareSelection(selection);

    reader->setDefaultSelectionRequest(seSelection.getSelectionOperation(),
                                       ObservableReader::NotificationMode::MATCHED_ONLY);

    /* Test */
    reader->insertSe(hoplinkSE());

    /* Lock thread for 2 seconds max to wait for the event */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock4>(readerObs)
        ->mLock.await(std::chrono::seconds(2));

    /* Should be 0 because countDown is called by observer */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock4>(readerObs)
                                   ->mLock.getCount()),
              0);

    tearDown(stubPlugin, readerObs);
}
class SRT_ReaderObserverMock5 final : public ObservableReader::ReaderObserver {
public:
    SRT_ReaderObserverMock5(std::shared_ptr<StubReader> reader,
                            std::shared_ptr<StubPluginImpl> plugin)
    : mLock(1), mReader(reader), mPlugin(plugin) {}

    void update(std::shared_ptr<ReaderEvent> event) override
    {
        (void)event;

        /* No event is thrown, should not be called */
        mLock.countDown();
    }

    CountDownLatch mLock;

private:
    std::shared_ptr<StubReader> mReader;
    std::shared_ptr<StubPluginImpl> mPlugin;
};

TEST(StubReaderTest, testInsertNotMatching_MatchedOnly)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Add observer */
    readerObs = std::make_shared<SRT_ReaderObserverMock5>(reader, stubPlugin);

    /* Add Protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

    /* Add observer */
    reader->addObserver(readerObs);

    /* Not matching poAid */
    const std::string poAid = "A000000291A000000192";
    SeSelection seSelection;
    auto aidSelector = SeSelector::AidSelector::builder()->aidToSelect(poAid).build();
    auto seSelector = PoSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                            .aidSelector(aidSelector)
                                            .invalidatedPo(PoSelector::InvalidatedPo::REJECT)
                                            .build();
    auto poSelector = std::dynamic_pointer_cast<PoSelector>(seSelector);
    auto poSelectionRequest = std::make_shared<PoSelectionRequest>(poSelector);
    auto selection = std::reinterpret_pointer_cast
                             <AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(
                                 poSelectionRequest);

    seSelection.prepareSelection(selection);

    reader->setDefaultSelectionRequest(seSelection.getSelectionOperation(),
                                       ObservableReader::NotificationMode::MATCHED_ONLY);

    /* Test */
    reader->insertSe(hoplinkSE());

    Thread::sleep(100);
    reader->removeSe();

    /* Lock thread for 2 seconds max to wait for the event */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock5>(readerObs)
         ->mLock.await(std::chrono::milliseconds(100));

    /* Should be 1 because countDown is never called */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock5>(readerObs)
                                   ->mLock.getCount()),
              1);

    tearDown(stubPlugin, readerObs);
}

class SRT_ReaderObserverMock6 final : public ObservableReader::ReaderObserver {
public:
    SRT_ReaderObserverMock6(std::shared_ptr<StubReader> reader,
                            std::shared_ptr<StubPluginImpl> plugin)
    : mLock(1), mReader(reader), mPlugin(plugin) {}

    void update(std::shared_ptr<ReaderEvent> event) override
    {
        ASSERT_EQ(event->getReaderName(), mReader->getName());
        ASSERT_EQ(event->getPluginName(), mPlugin->getName());
        /* An SE_INSERTED event is thrown */
        ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_INSERTED);
        /* Card has not match */
        ASSERT_FALSE(event->getDefaultSelectionsResponse()
                          ->getSelectionSeResponses()[0]
                          ->getSelectionStatus()
                          ->hasMatched());

        /* Should be called */
        mLock.countDown();
     }

    CountDownLatch mLock;

private:
    std::shared_ptr<StubReader> mReader;
    std::shared_ptr<StubPluginImpl> mPlugin;
};

TEST(StubReaderTest, testInsertNotMatching_Always)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Add Protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

    /* Add observer */
    readerObs = std::make_shared<SRT_ReaderObserverMock6>(reader, stubPlugin);

    /* Add observer */
    reader->addObserver(readerObs);

    /* Not matching poAid */
    const std::string poAid = "A000000291A000000192";
    SeSelection seSelection;
    auto aidSelector = SeSelector::AidSelector::builder()->aidToSelect(poAid).build();
    auto seSelector = PoSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                            .aidSelector(aidSelector)
                                            .invalidatedPo(PoSelector::InvalidatedPo::REJECT)
                                            .build();
    auto poSelector = std::dynamic_pointer_cast<PoSelector>(seSelector);
    auto poSelectionRequest = std::make_shared<PoSelectionRequest>(poSelector);
    auto selection = std::reinterpret_pointer_cast
                        <AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(
                            poSelectionRequest);

    seSelection.prepareSelection(selection);

    reader->setDefaultSelectionRequest(seSelection.getSelectionOperation(),
                                       ObservableReader::NotificationMode::ALWAYS);

    /* Test */
    reader->insertSe(hoplinkSE());

    /* Lock thread for 2 seconds max to wait for the event */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock6>(readerObs)
         ->mLock.await(std::chrono::seconds(2));

    /* Should be 0 because countDown is called by observer */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock6>(readerObs)
                                   ->mLock.getCount()),
              0);

    tearDown(stubPlugin, readerObs);
}

class SRT_ReaderObserverMock7 final : public ObservableReader::ReaderObserver {
public:
    SRT_ReaderObserverMock7(std::shared_ptr<StubReader> reader,
                            std::shared_ptr<StubPluginImpl> plugin)
    : mLock(1), mReader(reader), mPlugin(plugin) {}

    void update(std::shared_ptr<ReaderEvent> event) override
    {
        ASSERT_EQ(ReaderEvent::EventType::SE_INSERTED, event->getEventType());

        SeSelection seSelection;
        auto atrFilter = std::make_shared<PoSelector::AtrFilter>("3B.*");
        auto seSelector = PoSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_B_PRIME)
                                                .atrFilter(atrFilter)
                                                .invalidatedPo(PoSelector::InvalidatedPo::REJECT)
                                                .build();
        auto poSelector =  std::dynamic_pointer_cast<PoSelector>(seSelector);
        auto poSelectionRequest = std::make_shared<PoSelectionRequest>(poSelector);
        auto selection = std::reinterpret_pointer_cast
                             <AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(
                                 poSelectionRequest);

        /* Prepare selector, ignore AbstractMatchingSe here */
        seSelection.prepareSelection(selection);

        try {
            std::shared_ptr<SelectionsResult> selectionsResult =
                seSelection.processExplicitSelection(mReader);
            std::shared_ptr<AbstractMatchingSe> matchingSe =
                selectionsResult->getActiveMatchingSe();

            ASSERT_NE(matchingSe, nullptr);

        } catch (const KeypleReaderException& e) {
            (void)e;
            FAIL();
        } catch (const KeypleException& e) {
            (void)e;
            FAIL();
        }

        /* Unlock thread */
        mLock.countDown();
     }

    CountDownLatch mLock;

private:
    std::shared_ptr<StubReader> mReader;
    std::shared_ptr<StubPluginImpl> mPlugin;
};

TEST(StubReaderTest, testExplicitSelection_onEvent)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Add Protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_B_PRIME,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_B_PRIME]);

    /* Add observer */
    readerObs = std::make_shared<SRT_ReaderObserverMock7>(reader, stubPlugin);

    /* Add observer */
    reader->addObserver(readerObs);

    /* Test */
    reader->insertSe(revision1SE());

    /* Lock thread for 2 seconds max to wait for the event */
    std::dynamic_pointer_cast<SRT_ReaderObserverMock7>(readerObs)
         ->mLock.await(std::chrono::seconds(2));

    /* Should be 0 because countDown is called by observer */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock7>(readerObs)
                                   ->mLock.getCount()),
              0);

    tearDown(stubPlugin, readerObs);
}

class SRT_ReaderObserverMock8 final : public ObservableReader::ReaderObserver {
public:
    SRT_ReaderObserverMock8(std::shared_ptr<StubReader> reader,
                            std::shared_ptr<StubPluginImpl> plugin)
    : mLock(1), mReader(reader), mPlugin(plugin) {}

    void update(std::shared_ptr<ReaderEvent> event) override
    {
        if (event->getEventType() == ReaderEvent::EventType::SE_MATCHED) {
            auto proxy = std::dynamic_pointer_cast<ProxyReader>(mReader);
            proxy->transmitSeRequest(nullptr, ChannelControl::CLOSE_AFTER);
            mLock.countDown();
        }
     }

    CountDownLatch mLock;

private:
    std::shared_ptr<StubReader> mReader;
    std::shared_ptr<StubPluginImpl> mPlugin;
};

TEST(StubReaderTest, testReleaseSeChannel)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    reader->startSeDetection(ObservableReader::PollingMode::SINGLESHOT);

    /* Add Protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

    /* Add observer */
    readerObs = std::make_shared<SRT_ReaderObserverMock8>(reader, stubPlugin);

    /* Add observer */
    reader->addObserver(readerObs);

    const std::string poAid = "A000000291A000000191";
    SeSelection seSelection;
    auto aidSelector = SeSelector::AidSelector::builder()->aidToSelect(poAid).build();
    auto seSelector = PoSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                            .aidSelector(aidSelector)
                                            .invalidatedPo(PoSelector::InvalidatedPo::REJECT)
                                            .build();
    auto poSelector =  std::dynamic_pointer_cast<PoSelector>(seSelector);
    auto poSelectionRequest = std::make_shared<PoSelectionRequest>(poSelector);
    auto selection = std::reinterpret_pointer_cast
                        <AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(
                            poSelectionRequest);

    seSelection.prepareSelection(selection);

    reader->setDefaultSelectionRequest(seSelection.getSelectionOperation(),
                                       ObservableReader::NotificationMode::MATCHED_ONLY);

    /* Test */
    reader->insertSe(hoplinkSE());

    /* Lock thread for 2 seconds max to wait for the event */
   std::dynamic_pointer_cast<SRT_ReaderObserverMock8>(readerObs)
         ->mLock.await(std::chrono::seconds(2));

    /* Should be 0 because countDown is called by observer */
    ASSERT_EQ(static_cast<int>(std::dynamic_pointer_cast<SRT_ReaderObserverMock8>(readerObs)
                                   ->mLock.getCount()), 0);

    tearDown(stubPlugin, readerObs);
}

/*
 * TEST
 *
 * TRANSMIT
 *
 */

TEST(StubReaderTest, transmit_Hoplink_Successful)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Init Request */
    std::vector<std::shared_ptr<SeRequest>> requests = getRequestIsoDepSetSample();

    /* Init SE */
    reader->insertSe(hoplinkSE());

    /* Add Protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

    /* Send the selection request */
    genericSelectSe(reader);

    /* Test */
    auto proxy = std::dynamic_pointer_cast<ProxyReader>(reader);
    std::vector<std::shared_ptr<SeResponse>> seResponse =
        proxy->transmitSeRequests(requests,
                                  MultiSeRequestProcessing::FIRST_MATCH,
                                  ChannelControl::KEEP_OPEN);

    /* Assert */
    ASSERT_TRUE(seResponse[0]->getApduResponses()[0]->isSuccessful());

    tearDown(stubPlugin, readerObs);
}

/* Commented out in Java code */
// @Test
// public void transmit_null_Selection() {
// // init SE
// // no SE
//
// // init request
// SeRequest seRequest = getRequestIsoDepSetSample();
//
// // add Protocol flag
// reader->addSeProtocolSetting(
// new SeProtocolSetting(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));
//
// // test
// SeResponse resp = reader->transmit(seRequest);
//
// Assert.assertNull(resp.get(0));
// }

TEST(StubReaderTest, transmit_no_response)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Init Request */
    std::vector<std::shared_ptr<SeRequest>> requests = getNoResponseRequest();

    /* Init SE */
    reader->insertSe(noApduResponseSE());

    /* Add Protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

    /* Send the selection request */
    genericSelectSe(reader);

    auto proxy = std::dynamic_pointer_cast<ProxyReader>(reader);

    EXPECT_THROW(proxy->transmitSeRequests(requests,
                                           MultiSeRequestProcessing::FIRST_MATCH,
                                           ChannelControl::KEEP_OPEN),
                 KeypleReaderException);

    tearDown(stubPlugin, readerObs);
}

TEST(StubReaderTest, transmit_partial_response_set_0)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Init Request */
    std::vector<std::shared_ptr<SeRequest>> requests = getPartialRequestList(0);

    /* Init SE */
    reader->insertSe(partialSE());

    /* Add Protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

    /* Send the selection request */
    genericSelectSe(reader);

    /* Test */
    try {
        auto proxy = std::dynamic_pointer_cast<ProxyReader>(reader);
        std::vector<std::shared_ptr<SeResponse>> seResponses =
            proxy->transmitSeRequests(requests,
                                      MultiSeRequestProcessing::FIRST_MATCH,
                                      ChannelControl::KEEP_OPEN);
        FAIL();

    } catch (const KeypleReaderIOException& ex) {
        ASSERT_EQ(static_cast<int>(ex.getSeResponses().size()), 1);
        ASSERT_EQ(static_cast<int>(ex.getSeResponses()[0]->getApduResponses().size()), 2);
    }

    tearDown(stubPlugin, readerObs);
}

/* @Test commented in Java code */
// @Test
// public void transmit_partial_response_set_1() throws Exception {
//     stubPlugin->plugStubReader("StubReaderTest", true);
//     ASSERT_EQ(1, stubPlugin->getReaders().size());
//     StubReader reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest");
//     // init Request
//     List<SeRequest> seRequests = getPartialRequestList(1);

//     // init SE
//     reader->insertSe(partialSE());

//     // add Protocol flag
//     reader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO14443_4,
//             StubProtocolSetting::STUB_PROTOCOL_SETTING
//                     .get(SeCommonProtocols::PROTOCOL_ISO14443_4));

//     // send the selection request
//     genericSelectSe(reader);

//     // test
//     try {
//         List<SeResponse> seResponses = ((ProxyReader) reader).transmitSeRequests(seRequests,
//                 MultiSeRequestProcessing.FIRST_MATCH, ChannelControl.KEEP_OPEN);
//         Assert.fail("Should throw exception");

//     } catch (KeypleReaderIOException ex) {
//         ASSERT_EQ(ex.getSeResponses().size(), 2);
//         ASSERT_EQ(ex.getSeResponses().get(0).getApduResponses().size(), 4);
//         ASSERT_EQ(ex.getSeResponses().get(1).getApduResponses().size(), 2);
//         ASSERT_EQ(ex.getSeResponses().get(1).getApduResponses().size(), 2);
//     }
// }

/* @Test commented in Java code */
// // @Test
// public void transmit_partial_response_set_2() throws Exception {
//     stubPlugin->plugStubReader("StubReaderTest", true);
//     ASSERT_EQ(1, stubPlugin->getReaders().size());
//     StubReader reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest");
//     // init Request
//     List<SeRequest> seRequests = getPartialRequestList(2);

//     // init SE
//     reader->insertSe(partialSE());

//     // add Protocol flag
//     reader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO14443_4,
//             StubProtocolSetting::STUB_PROTOCOL_SETTING
//                     .get(SeCommonProtocols::PROTOCOL_ISO14443_4));

//     // send the selection request
//     genericSelectSe(reader);

//     // test
//     try {
//         List<SeResponse> seResponses = ((ProxyReader) reader).transmitSeRequests(seRequests,
//                 MultiSeRequestProcessing.FIRST_MATCH, ChannelControl.KEEP_OPEN);
//         Assert.fail("Should throw exception");

//     } catch (KeypleReaderIOException ex) {
//         ASSERT_EQ(ex.getSeResponses().size(), 3);
//         ASSERT_EQ(ex.getSeResponses().get(0).getApduResponses().size(), 4);
//         ASSERT_EQ(ex.getSeResponses().get(1).getApduResponses().size(), 4);
//         ASSERT_EQ(ex.getSeResponses().get(2).getApduResponses().size(), 2);
//     }
// }

/* @Test commented in Java code */
// // @Test
// public void transmit_partial_response_set_3() throws Exception {
//     stubPlugin->plugStubReader("StubReaderTest", true);
//     ASSERT_EQ(1, stubPlugin->getReaders().size());
//     StubReader reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest");
//     // init Request
//     List<SeRequest> seRequests = getPartialRequestList(3);

//     // init SE
//     reader->insertSe(partialSE());

//     // add Protocol flag
//     reader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO14443_4,
//             StubProtocolSetting::STUB_PROTOCOL_SETTING
//                     .get(SeCommonProtocols::PROTOCOL_ISO14443_4));

//     // send the selection request
//     genericSelectSe(reader);

//     // test
//     try {
//         List<SeResponse> seResponses = ((ProxyReader) reader).transmitSeRequests(seRequests,
//                 MultiSeRequestProcessing.FIRST_MATCH, ChannelControl.KEEP_OPEN);
//         ASSERT_EQ(seResponses.size(), 3);
//         ASSERT_EQ(seResponses.get(0).getApduResponses().size(), 4);
//         ASSERT_EQ(seResponses.get(1).getApduResponses().size(), 4);
//         ASSERT_EQ(seResponses.get(2).getApduResponses().size(), 4);
//     } catch (KeypleReaderException ex) {
//         Assert.fail("Should not throw exception");
//     }
// }

TEST(StubReaderTest, transmit_partial_response_0)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Init Request */
    std::shared_ptr<SeRequest> seRequest = getPartialRequest(0);

    /* Init SE */
    reader->insertSe(partialSE());

    /* Add Protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

    /* Send the selection request */
    genericSelectSe(reader);

    /* Test */
    try {
        auto proxy = std::dynamic_pointer_cast<ProxyReader>(reader);
        proxy->transmitSeRequest(seRequest, ChannelControl::KEEP_OPEN);
        FAIL();

    } catch (const KeypleReaderIOException& ex) {
        ASSERT_EQ(static_cast<int>(ex.getSeResponse()->getApduResponses().size()), 0);
    }

    tearDown(stubPlugin, readerObs);
}

TEST(StubReaderTest, transmit_partial_response_1)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Init Request */
    std::shared_ptr<SeRequest> seRequest = getPartialRequest(1);

    /* Init SE */
    reader->insertSe(partialSE());

    /* Add Protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

    /* Send the selection request */
    genericSelectSe(reader);

    /* Test */
    try {
        auto proxy = std::dynamic_pointer_cast<ProxyReader>(reader);
        proxy->transmitSeRequest(seRequest, ChannelControl::KEEP_OPEN);
        FAIL();

    } catch (const KeypleReaderIOException& ex) {
        ASSERT_EQ(static_cast<int>(ex.getSeResponse()->getApduResponses().size()), 1);
    }

    tearDown(stubPlugin, readerObs);
}

TEST(StubReaderTest, transmit_partial_response_2)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Init Request */
    std::shared_ptr<SeRequest> seRequest = getPartialRequest(2);

    /* Init SE */
    reader->insertSe(partialSE());

    /* Add Protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

    /* Send the selection request */
    genericSelectSe(reader);

    /* Test */
    try {
        auto proxy = std::dynamic_pointer_cast<ProxyReader>(reader);
        proxy->transmitSeRequest(seRequest, ChannelControl::KEEP_OPEN);
        FAIL();

    } catch (const KeypleReaderIOException& ex) {
        ASSERT_EQ(static_cast<int>(ex.getSeResponse()->getApduResponses().size()), 2);
    }

    tearDown(stubPlugin, readerObs);
}

TEST(StubReaderTest, transmit_partial_response_3)
{
    std::shared_ptr<ObservableReader::ReaderObserver> readerObs;
    std::shared_ptr<StubPluginImpl> stubPlugin;

    stubPlugin = setUp();

    stubPlugin->plugStubReader("StubReaderTest", true);

    ASSERT_EQ(static_cast<int>(stubPlugin->getReaders().size()), 1);

    auto reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest"));

    /* Init Request */
    std::shared_ptr<SeRequest> seRequest = getPartialRequest(3);

    /* Init SE */
    reader->insertSe(partialSE());

    /* Add Protocol flag */
    reader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

    /* Send the selection request */
    genericSelectSe(reader);

    /* Test */
    try {
        auto proxy = std::dynamic_pointer_cast<ProxyReader>(reader);
        std::shared_ptr<SeResponse> seResponse =
            proxy->transmitSeRequest(seRequest, ChannelControl::KEEP_OPEN);
        ASSERT_EQ(static_cast<int>(seResponse->getApduResponses().size()), 3);
    } catch (const KeypleReaderException& ex) {
        (void)ex;
        FAIL();
    }

    tearDown(stubPlugin, readerObs);
}


// /*
//     * NAME and PARAMETERS
//     */

// @Test
// public void testGetName() throws Exception {
//     stubPlugin->plugStubReader("StubReaderTest", true);
//     ASSERT_EQ(1, stubPlugin->getReaders().size());
//     StubReader reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest");
//     Assert.assertNotNull(reader->getName());
// }


// // Set correct parameters
// @Test
// public void testSetAllowedParameters() throws Exception {
//     stubPlugin->plugStubReader("StubReaderTest", true);
//     ASSERT_EQ(1, stubPlugin->getReaders().size());
//     StubReader reader = std::dynamic_pointer_cast<StubReader>(stubPlugin->getReader("StubReaderTest");

//     Map<String, String> p1 = new HashMap<String, String>();
//     p1.put("aParameter", "a");
//     reader->setParameters(p1);

//     Map<String, String> p2 = reader->getParameters();
//     assert (p1.equals(p2));
// }


// /*
//     * HELPER METHODS
//     */

// }

// public static StubSecureElement getSENoconnection() {
//     return new StubSecureElement() {
//         @Override
//         public byte[] getATR() {
//             return new byte[0];
//         }

//         @Override
//         public boolean isPhysicalChannelOpen() {
//             return false;
//         }

//         // override methods to fail open connection
//         @Override
//         public void openPhysicalChannel() {
//             throw new KeypleReaderIOException("Impossible to establish connection");
//         }

//         @Override
//         public void closePhysicalChannel() {
//             throw new KeypleReaderIOException("Channel is not open");
//         }

//         @Override
//         public byte[] processApdu(byte[] apduIn) {
//             throw new KeypleReaderIOException("Error while transmitting apdu");
//         }

//         @Override
//         public String getSeProcotol() {
//             return null;
//         }
//     };

// }

// public static ApduRequest getApduSample() {
//     return new ApduRequest(ByteArrayUtil.fromHex("FEDCBA98 9005h"), false);
// }

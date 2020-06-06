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

#include "AbstractObservableLocalReader.h"

#include "AbstractObservableState.h"
#include "ByteArrayUtil.h"
#include "ObservableReaderStateService.h"
#include "ReaderEvent.h"
#include "SeCommonProtocols.h"
#include "WaitForSeInsertion.h"
#include "WaitForSeProcessing.h"
#include "WaitForSeRemoval.h"
#include "WaitForStartDetect.h"

using namespace testing;

using namespace keyple::core::seproxy::plugin::local;
using namespace keyple::core::seproxy::plugin::local::state;
using namespace keyple::core::util;

class DefaultSelectionsRequestMock: public AbstractDefaultSelectionsRequest {
public:
    const std::vector<std::shared_ptr<SeRequest>>& getSelectionSeRequestSet()
        const override
    {
        return requests;
    }

    const MultiSeRequestProcessing& getMultiSeRequestProcessing() const override
    {
        return processing;
    }

    const ChannelControl& getChannelControl() const override
    {
        return control;
    }

private:
    std::vector<std::shared_ptr<SeRequest>> requests;

    const MultiSeRequestProcessing processing =
        MultiSeRequestProcessing::FIRST_MATCH;

    const ChannelControl control = ChannelControl::CLOSE_AFTER;
};

class ObservableLocalReaderMock : public AbstractObservableLocalReader {
public:
    ObservableLocalReaderMock(
      const std::string& pluginName, const std::string& readerName)
    : AbstractObservableLocalReader(pluginName, readerName)
    {
        this->stateService = initStateService();
    }

    const std::map<const std::string, const std::string> getParameters() const
        override
    {
        return parameters;
    }

    void setParameter(const std::string& key, const std::string& value) override
    {
        (void)key;
        (void)value;
    }

    const TransmissionMode& getTransmissionMode() const override
    {
        return  transmissionMode;
    }

    bool checkSePresence() override
    {
        return true;
    }

    const std::vector<uint8_t>& getATR() override
    {
        return atr;
    }

    void openPhysicalChannel() override
    {
    }

    void closePhysicalChannel() override
    {
    }

    bool isPhysicalChannelOpen() override
    {
        return false;
    }

    bool protocolFlagMatches(const std::shared_ptr<SeProtocol> protocolFlag)
        override
    {
        (void)protocolFlag;

        return false;
    }

    std::vector<uint8_t> transmitApdu(const std::vector<uint8_t>& apduIn)
        override
    {
        (void)apduIn;

        return {0x11, 0x22, 0x33, 0x44, 0x90, 0x00};
    }

    std::shared_ptr<ObservableReaderStateService> initStateService() override
    {
        std::map<MonitoringState, std::shared_ptr<AbstractObservableState>> states;

        states.insert(
            std::pair<MonitoringState,
                      std::shared_ptr<AbstractObservableState>>(
                MonitoringState::WAIT_FOR_START_DETECTION,
                std::make_shared<WaitForStartDetect>(this)));

        states.insert(
            std::pair<MonitoringState,
                      std::shared_ptr<AbstractObservableState>>(
                MonitoringState::WAIT_FOR_SE_INSERTION,
                std::make_shared<WaitForSeInsertion>(this)));

        states.insert(
            std::pair<MonitoringState,
                      std::shared_ptr<AbstractObservableState>>(
                MonitoringState::WAIT_FOR_SE_PROCESSING,
                std::make_shared<WaitForSeProcessing>(this)));

        states.insert(
            std::pair<MonitoringState,
                      std::shared_ptr<AbstractObservableState>>(
                MonitoringState::WAIT_FOR_SE_REMOVAL,
                std::make_shared<WaitForSeRemoval>(this)));

        return std::make_shared<ObservableReaderStateService>(
            this, states, MonitoringState::WAIT_FOR_START_DETECTION);
    }

private:
    std::map<const std::string, const std::string> parameters;

    const TransmissionMode transmissionMode = TransmissionMode::CONTACTLESS;

    const std::vector<uint8_t> atr = {0x11, 0x22, 0x33, 0x44, 0x55};
};

class ReaderObserverMock : public ObservableReader::ReaderObserver {
public:
    void update(const std::shared_ptr<ReaderEvent> event) override
    {
        (void)event;

        updated = true;
    }

    bool updated = false;
};

static std::shared_ptr<std::set<int>> getSuccessFulStatusCode()
{
    std::shared_ptr<std::set<int>> successfulStatusCodes =
        std::make_shared< std::set<int>>();

    int iData = std::stoi("9000", 0, 16);
    successfulStatusCodes->insert(iData);

    return successfulStatusCodes;
}

static std::vector<uint8_t> getCommand()
{
    return ByteArrayUtil::fromHex("FEDCBA98 9005h");
}

static std::shared_ptr<ApduRequest> getApduSample()
{
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(getCommand(), true,
	                              getSuccessFulStatusCode());
    request->setName("TEST");

    return request;
}

static std::vector<std::shared_ptr<ApduRequest>> getApduList()
{
    std::vector<std::shared_ptr<ApduRequest>> apdus;
    apdus.push_back(getApduSample());
    apdus.push_back(getApduSample());

    return apdus;
}

static std::shared_ptr<SeSelector> getSelector(
    std::shared_ptr<std::set<int>> selectionStatusCode)
{
    /*
     * We can use a fake AID here because it is not fully interpreted, the
     * purpose of this unit test is to verify the proper format of the request.
     */
    const std::vector<uint8_t> aid = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

    return std::make_shared<SeSelector>(
               SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr,
               std::make_shared<SeSelector::AidSelector>(
                   std::make_shared<SeSelector::AidSelector::IsoAid>(aid),
                   selectionStatusCode),
               "seselector");
}

static std::shared_ptr<SeRequest> getSeRequestSample()
{
    return std::make_shared<SeRequest>(
               getSelector(getSuccessFulStatusCode()), getApduList());
}

TEST(AbstractObservableLocalReaderTest, AbstractObservableLocalReader)
{
    ObservableLocalReaderMock reader("pluginName", "readerName");
}

TEST(AbstractObservableLocalReaderTest, isSePresent)
{
    ObservableLocalReaderMock reader("pluginName", "readerName");

    ASSERT_TRUE(reader.isSePresent());
}

TEST(AbstractObservableLocalReaderTest, setDefaultSelectionRequest)
{
    ObservableLocalReaderMock reader("pluginName", "readerName");

    reader.setDefaultSelectionRequest(
        std::make_shared<DefaultSelectionsRequestMock>(),
        ObservableReader::NotificationMode::ALWAYS);

    ASSERT_EQ(reader.getPollingMode(),
              ObservableReader::PollingMode::SINGLESHOT);

    reader.setDefaultSelectionRequest(
        std::make_shared<DefaultSelectionsRequestMock>(),
        ObservableReader::NotificationMode::ALWAYS,
        ObservableReader::PollingMode::REPEATING);

    ASSERT_EQ(reader.getPollingMode(),
              ObservableReader::PollingMode::REPEATING);
}

TEST(AbstractObservableLocalReaderTest, processSeInserted)
{
    ObservableLocalReaderMock reader("pluginName", "readerName");

    std::shared_ptr<ReaderEvent> event = reader.processSeInserted();

    ASSERT_NE(event, nullptr);
    ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_INSERTED);
}

TEST(AbstractObservableLocalReaderTest, isSePresentPing)
{
    ObservableLocalReaderMock reader("pluginName", "readerName");

    ASSERT_TRUE(reader.isSePresentPing());
}

TEST(AbstractObservableLocalReaderTest, getPollingMode)
{
    ObservableLocalReaderMock reader("pluginName", "readerName");

    reader.setDefaultSelectionRequest(
        std::make_shared<DefaultSelectionsRequestMock>(),
        ObservableReader::NotificationMode::ALWAYS);

    ASSERT_EQ(reader.getPollingMode(),
              ObservableReader::PollingMode::SINGLESHOT);

    reader.setDefaultSelectionRequest(
        std::make_shared<DefaultSelectionsRequestMock>(),
        ObservableReader::NotificationMode::ALWAYS,
        ObservableReader::PollingMode::REPEATING);

    ASSERT_EQ(reader.getPollingMode(),
              ObservableReader::PollingMode::REPEATING);
}

TEST(AbstractObservableLocalReaderTest, addObserver)
{
    ObservableLocalReaderMock reader("pluginName", "readerName");

    ASSERT_EQ(reader.countObservers(), 0);

    reader.addObserver(nullptr);

    ASSERT_EQ(reader.countObservers(), 0);

    reader.addObserver(std::make_shared<ReaderObserverMock>());

    ASSERT_EQ(reader.countObservers(), 1);
}

TEST(AbstractObservableLocalReaderTest, removeObserver)
{
    ObservableLocalReaderMock reader("pluginName", "readerName");

    ASSERT_EQ(reader.countObservers(), 0);

    reader.removeObserver(nullptr);

    ASSERT_EQ(reader.countObservers(), 0);

    std::shared_ptr<ReaderObserverMock> observer =
        std::make_shared<ReaderObserverMock>();

    reader.addObserver(observer);

    ASSERT_EQ(reader.countObservers(), 1);

    reader.removeObserver(std::make_shared<ReaderObserverMock>());

    ASSERT_EQ(reader.countObservers(), 1);

    reader.removeObserver(observer);

    ASSERT_EQ(reader.countObservers(), 0);
}

TEST(AbstractObservableLocalReaderTest, clearObservers)
{
    ObservableLocalReaderMock reader("pluginName", "readerName");

    reader.addObserver(std::make_shared<ReaderObserverMock>());

    ASSERT_EQ(reader.countObservers(), 1);

    reader.clearObservers();

    ASSERT_EQ(reader.countObservers(), 0);
}

TEST(AbstractObservableLocalReaderTest, notifyObservers)
{
    ObservableLocalReaderMock reader("pluginName", "readerName");

    std::shared_ptr<ReaderObserverMock> observer =
        std::make_shared<ReaderObserverMock>();

    reader.addObserver(observer);

    ASSERT_FALSE(observer->updated);

    reader.notifyObservers(
        std::make_shared<ReaderEvent>("plugin", "reader",
                                      ReaderEvent::EventType::SE_INSERTED,
                                      nullptr));

    ASSERT_TRUE(observer->updated);
}

TEST(AbstractObservableLocalReaderTest, getCurrentMonitoringState)
{
    ObservableLocalReaderMock reader("pluginName", "readerName");

    ASSERT_EQ(reader.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_START_DETECTION);

    /* Start detection */
    reader.startSeDetection(ObservableReader::PollingMode::REPEATING);

    /* Assert currentState has changed */
    ASSERT_EQ(reader.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_SE_INSERTION);

    /* Stop detection */
    reader.stopSeDetection();

    /* Assert currentState has changed */
    ASSERT_EQ(reader.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_START_DETECTION);

    /* Start detection */
    reader.startSeDetection(ObservableReader::PollingMode::REPEATING);

    /* Insert SE */
    reader.onEvent(InternalEvent::SE_INSERTED);

    /* Assert currentState has changed */
    ASSERT_EQ(reader.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_SE_PROCESSING);

    /* SE has been processed */
    reader.startRemovalSequence();

    /* Assert currentState has changed */
    ASSERT_EQ(reader.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_SE_REMOVAL);

    /* Remove SE */
    reader.onEvent(InternalEvent::SE_REMOVED);

    /* Assert currentState has changed */
    ASSERT_EQ(reader.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_SE_INSERTION);
}

TEST(AbstractObservableLocalReaderTest, notifySeProcessed_withForceClosing)
{
    ObservableLocalReaderMock reader("pluginName", "readerName");

    /* Keep open */
    std::shared_ptr<SeRequest> request = getSeRequestSample();
    //reader.transmit(request, ChannelControl::KEEP_OPEN);

    /* Force closing */
    //reader.notifySeProcessed();

    /*
     * processSeRequest is 'protected final'. can't override it in mock,
     * can't use it directly with 'reader'. Not sure I can make that test
     * happen...
     *
     * EXPECT_CALL(reader, processSeRequest(nullptr,
     *                                      ChannelControl::CLOSE_AFTER))
     *   .times(1);
     */
}

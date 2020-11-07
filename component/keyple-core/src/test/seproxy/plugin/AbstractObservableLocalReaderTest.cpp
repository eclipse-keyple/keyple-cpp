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

#include "AbstractObservableLocalReader.h"

#include "AbstractObservableState.h"
#include "ByteArrayUtil.h"
#include "ObservableReaderStateService.h"
#include "ReaderEvent.h"
#include "SeCommonProtocols.h"
#include "SeResponse.h"
#include "WaitForSeInsertion.h"
#include "WaitForSeProcessing.h"
#include "WaitForSeRemoval.h"
#include "WaitForStartDetect.h"

using namespace testing;

using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::util;

class AOLR_AbstractDefaultSelectionsRequestMock
: public AbstractDefaultSelectionsRequest {
public:
    AOLR_AbstractDefaultSelectionsRequestMock(
      std::vector<std::shared_ptr<SeRequest>> selectionSeRequests,
      const MultiSeRequestProcessing& multiSeRequestProcessing,
      const ChannelControl& channelControl)
    : AbstractDefaultSelectionsRequest(selectionSeRequests,
          multiSeRequestProcessing, channelControl) {}
};

class AOLR_AbstractObservableLocalReaderMock
: public AbstractObservableLocalReader {
public:
    AOLR_AbstractObservableLocalReaderMock(
      const std::string& pluginName, const std::string& readerName)
    : AbstractObservableLocalReader(pluginName, readerName)
    {
        mStateService = initStateService();
    }

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const, override));

    MOCK_METHOD(void,
                setParameter,
                (const std::string&, const std::string&),
                (override));

    MOCK_METHOD((const TransmissionMode&),
                getTransmissionMode,
                (),
                (const, override));

    MOCK_METHOD(bool,
                checkSePresence,
                (),
                (override));

    MOCK_METHOD((const std::vector<uint8_t>&),
                getATR,
                (),
                (override));

    MOCK_METHOD(void,
                openPhysicalChannel,
                (),
                (override));

    MOCK_METHOD(void,
                closePhysicalChannel,
                (),
                (override));

    MOCK_METHOD(bool,
                isPhysicalChannelOpen,
                (),
                (override));

    MOCK_METHOD(bool,
                protocolFlagMatches,
                (const std::shared_ptr<SeProtocol>),
                (override));

    MOCK_METHOD(std::vector<uint8_t>,
                transmitApdu,
                (const std::vector<uint8_t>&),
                (override));

    MOCK_METHOD(std::shared_ptr<SeResponse>,
                processSeRequest,
                (const std::shared_ptr<SeRequest>, const ChannelControl&),
                (override));

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

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

    const std::string& getName() const override
    {
        return mName;
    }

private:
    const std::string mName = "AOLR_AbstractObservableLocalReaderMock";
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

static void configure(AOLR_AbstractObservableLocalReaderMock& reader)
{
    const std::vector<uint8_t> atr;
    const std::map<const std::string, const std::string> parameters;

    EXPECT_CALL(reader, checkSePresence())
        .WillRepeatedly(Return(false));

    EXPECT_CALL(reader, getATR())
        .WillRepeatedly(ReturnRef(atr));

    EXPECT_CALL(reader, openPhysicalChannel())
        .WillRepeatedly(Return());

    EXPECT_CALL(reader, closePhysicalChannel())
        .WillRepeatedly(Return());

    EXPECT_CALL(reader, isPhysicalChannelOpen())
        .WillRepeatedly(Return(false));

    EXPECT_CALL(reader, protocolFlagMatches(_))
        .WillRepeatedly(Return(false));

    EXPECT_CALL(reader, transmitApdu(_))
        .WillRepeatedly(Return(std::vector<uint8_t>({})));

    EXPECT_CALL(reader, getTransmissionMode())
        .WillRepeatedly(ReturnRefOfCopy(TransmissionMode::NONE));

    EXPECT_CALL(reader, getParameters())
        .WillRepeatedly(ReturnRef(parameters));

    EXPECT_CALL(reader, setParameter(_,_))
        .WillRepeatedly(Return());
}

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

    std::shared_ptr<SeSelector::AidSelector> aidSelector =
        SeSelector::AidSelector::builder()
            ->aidToSelect(aid)
            .build();

    if (selectionStatusCode) {
        for (const auto& code : *selectionStatusCode.get())
            aidSelector->addSuccessfulStatusCode(code);
    }

    return SeSelector::builder()
              ->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
              .atrFilter(nullptr)
              .aidSelector(aidSelector)
              .build();
}

static std::shared_ptr<SeRequest> getSeRequestSample()
{
    return std::make_shared<SeRequest>(
               getSelector(getSuccessFulStatusCode()), getApduList());
}

static std::shared_ptr<ApduResponse> getSuccessfullResponse()
{
    return std::make_shared<ApduResponse>(
               ByteArrayUtil::fromHex("FEDCBA98 9000h"), nullptr);
}

static std::vector<std::shared_ptr<ApduResponse>> getAListOfAPDUs()
{
    std::vector<std::shared_ptr<ApduResponse>> apdus;

    apdus.push_back(getSuccessfullResponse());

    return apdus;
}

static std::shared_ptr<SeResponse> getASeResponse()
{
    std::vector<std::shared_ptr<ApduResponse>> responses =
        getAListOfAPDUs();

    return std::make_shared<SeResponse>(
               true, true,
               std::make_shared<SelectionStatus>(
                   std::make_shared<AnswerToReset>(
                       ByteArrayUtil::fromHex(
                           "3B8F8001804F0CA000000306030001000000006A")),
                   std::make_shared<ApduResponse>(
                       ByteArrayUtil::fromHex("9000"), nullptr),
                   true),
               responses);
}

#if 0
static std::vector<std::shared_ptr<SeResponse>> getSeResponses()
{
    std::vector<std::shared_ptr<SeResponse>> responses;

    responses.push_back(getASeResponse());

    return responses;
}
#endif

TEST(AbstractObservableLocalReaderTest, AbstractObservableLocalReader)
{
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");
}

TEST(AbstractObservableLocalReaderTest, isSePresent)
{
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");

    EXPECT_CALL(reader, checkSePresence())
        .Times(1)
        .WillOnce(Return(true));

    ASSERT_TRUE(reader.isSePresent());
}

TEST(AbstractObservableLocalReaderTest, setDefaultSelectionRequest)
{
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");

    std::vector<std::shared_ptr<SeRequest>> selectionSeRequests;

    reader.setDefaultSelectionRequest(
        std::make_shared<AOLR_AbstractDefaultSelectionsRequestMock>(
            selectionSeRequests, MultiSeRequestProcessing::FIRST_MATCH,
            ChannelControl::CLOSE_AFTER),
        ObservableReader::NotificationMode::ALWAYS);

    ASSERT_EQ(reader.getPollingMode(),
              ObservableReader::PollingMode::SINGLESHOT);

    reader.setDefaultSelectionRequest(
        std::make_shared<AOLR_AbstractDefaultSelectionsRequestMock>(
            selectionSeRequests, MultiSeRequestProcessing::FIRST_MATCH,
            ChannelControl::CLOSE_AFTER),
        ObservableReader::NotificationMode::ALWAYS,
        ObservableReader::PollingMode::REPEATING);

    ASSERT_EQ(reader.getPollingMode(),
              ObservableReader::PollingMode::REPEATING);
}

TEST(AbstractObservableLocalReaderTest, processSeInserted)
{
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");

    std::shared_ptr<ReaderEvent> event = reader.processSeInserted();

    ASSERT_NE(event, nullptr);
    ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_INSERTED);
}

TEST(AbstractObservableLocalReaderTest, isSePresentPing)
{
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");

    EXPECT_CALL(reader, transmitApdu(_))
        .Times(1)
        .WillOnce(Return(std::vector<uint8_t>({0x90, 0X00})));

    ASSERT_TRUE(reader.isSePresentPing());
}

TEST(AbstractObservableLocalReaderTest, getPollingMode)
{
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");

    std::vector<std::shared_ptr<SeRequest>> selectionSeRequests;

    reader.setDefaultSelectionRequest(
        std::make_shared<AOLR_AbstractDefaultSelectionsRequestMock>(
            selectionSeRequests, MultiSeRequestProcessing::FIRST_MATCH,
            ChannelControl::CLOSE_AFTER),
        ObservableReader::NotificationMode::ALWAYS);

    ASSERT_EQ(reader.getPollingMode(),
              ObservableReader::PollingMode::SINGLESHOT);

    reader.setDefaultSelectionRequest(
        std::make_shared<AOLR_AbstractDefaultSelectionsRequestMock>(
            selectionSeRequests, MultiSeRequestProcessing::FIRST_MATCH,
            ChannelControl::CLOSE_AFTER),
        ObservableReader::NotificationMode::ALWAYS,
        ObservableReader::PollingMode::REPEATING);

    ASSERT_EQ(reader.getPollingMode(),
              ObservableReader::PollingMode::REPEATING);
}

TEST(AbstractObservableLocalReaderTest, addObserver)
{
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");

    ASSERT_EQ(reader.countObservers(), 0);

    reader.addObserver(nullptr);

    ASSERT_EQ(reader.countObservers(), 0);

    reader.addObserver(std::make_shared<ReaderObserverMock>());

    ASSERT_EQ(reader.countObservers(), 1);
}

TEST(AbstractObservableLocalReaderTest, removeObserver)
{
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");

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
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");

    reader.addObserver(std::make_shared<ReaderObserverMock>());

    ASSERT_EQ(reader.countObservers(), 1);

    reader.clearObservers();

    ASSERT_EQ(reader.countObservers(), 0);
}

TEST(AbstractObservableLocalReaderTest, notifyObservers)
{
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");

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

TEST(AbstractObservableLocalReaderTest, states)
{
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");

    EXPECT_CALL(reader, closePhysicalChannel())
        .WillRepeatedly(Return());

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
    reader.terminateSeCommunication();

    /* Assert currentState has changed */
    ASSERT_EQ(reader.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_SE_REMOVAL);

    /* Remove SE */
    reader.onEvent(InternalEvent::SE_REMOVED);

    /* Assert currentState has changed */
    ASSERT_EQ(reader.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_SE_INSERTION);
}

TEST(AbstractObservableLocalReaderTest, communicationClosing_forced)
{
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");

    configure(reader);

    EXPECT_CALL(reader, processSeRequest(_,_))
        .Times(1)
        .WillRepeatedly(Return(getASeResponse()));

    /* Keep open */
    reader.transmitSeRequest(getSeRequestSample(), ChannelControl::KEEP_OPEN);

    /* Force closing */
    reader.transmitSeRequest(nullptr, ChannelControl::CLOSE_AFTER);
}

TEST(AbstractObservableLocalReaderTest, communicationClosing_standard)
{
    AOLR_AbstractObservableLocalReaderMock reader("pluginName", "readerName");

    configure(reader);

    /* FIXME: Should verify that arg2 is never CLOSE_AFTER */
    EXPECT_CALL(reader, processSeRequest(_,_))
        .Times(1);

    /* Close after */
    reader.transmitSeRequest(getSeRequestSample(), ChannelControl::CLOSE_AFTER);

    /* Force closing is not called (only the transmit) */
}

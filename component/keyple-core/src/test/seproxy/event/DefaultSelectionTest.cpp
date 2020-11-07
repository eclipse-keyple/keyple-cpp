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

/* Core */
#include "AbstractObservableLocalReader.h"
#include "ByteArrayUtil.h"
#include "KeypleReaderIOException.h"
#include "ObservableReaderStateService.h"
#include "WaitForSeInsertion.h"
#include "WaitForSeProcessing.h"
#include "WaitForSeRemoval.h"
#include "WaitForStartDetect.h"

using namespace testing;

using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::util;

using NotificationMode = ObservableReader::NotificationMode;

class AbstractObservableLocalReaderMock : public AbstractObservableLocalReader {
public:
    AbstractObservableLocalReaderMock(const std::string& pluginName, const std::string& readerName)
    : AbstractObservableLocalReader(pluginName, readerName)
    {
        mStateService = initStateService();
    }

    std::shared_ptr<ObservableReaderStateService> initStateService() override
    {
        std::map<MonitoringState, std::shared_ptr<AbstractObservableState>> states = {
            {MonitoringState::WAIT_FOR_SE_INSERTION, std::make_shared<WaitForSeInsertion>(this)},
            {MonitoringState::WAIT_FOR_SE_PROCESSING, std::make_shared<WaitForSeProcessing>(this)},
            {MonitoringState::WAIT_FOR_SE_REMOVAL, std::make_shared<WaitForSeRemoval>(this)},
            {MonitoringState::WAIT_FOR_START_DETECTION, std::make_shared<WaitForStartDetect>(this)}
        };

        return std::make_shared<ObservableReaderStateService>(
                   this, states, MonitoringState::WAIT_FOR_START_DETECTION);
    }

    bool checkSePresence() override
    {
        return false;
    }

    const std::vector<uint8_t>& getATR() override
    {
        return mAtr;
    }

    void openPhysicalChannel() override {}

    void closePhysicalChannel() override {}

    bool isPhysicalChannelOpen() override
    {
        return false;
    }

    bool protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) override
    {
        (void)protocolFlag;

        return false;
    }

    std::vector<uint8_t> transmitApdu(const std::vector<uint8_t>& apduIn) override
    {
        (void)apduIn;

        return std::vector<uint8_t>{};
    }

    const TransmissionMode& getTransmissionMode() const override
    {
        return mMode;
    }

    const std::map<const std::string, const std::string>& getParameters() const override
    {
        return mParameters;
    }

    void setParameter(const std::string& key, const std::string& value) override
    {
        (void)key;
        (void)value;
    }

    /**
     * The purpose of this method is to provide certain test methods with public access to
     * processSeInserted that is package-private.
     *
     * @return ReaderEvent returned by processSeInserted
     */
    std::shared_ptr<ReaderEvent> processSeInsertedTest()
    {
        return processSeInserted();
    }

    void setParameters(const std::map<const std::string, const std::string>& parameters) override
    {
        (void)parameters;
    }

    const std::string& getName() const override
    {
        return mName;
    }

    MOCK_METHOD((std::shared_ptr<SeResponse>),
                processSeRequest,
                (const std::shared_ptr<SeRequest>, const ChannelControl&),
                (override));

    MOCK_METHOD(std::vector<std::shared_ptr<SeResponse>>,
                processSeRequests,
                (const std::vector<std::shared_ptr<SeRequest>>&,
                 const MultiSeRequestProcessing&,
                 const ChannelControl&),
                 (override));

    MOCK_METHOD((std::shared_ptr<SeResponse>),
                transmitSeRequest,
                (std::shared_ptr<SeRequest>, const ChannelControl&),
                (override));

    MOCK_METHOD((std::vector<std::shared_ptr<SeResponse>>),
                transmitSeRequests,
                (const std::vector<std::shared_ptr<SeRequest>>&,
                 const MultiSeRequestProcessing&,
                 const ChannelControl&),
                (override));

private:
    std::vector<uint8_t> mAtr;
    std::map<const std::string, const std::string> mParameters;
    TransmissionMode mMode = TransmissionMode::NONE;
    const std::string mName = "AbstractObservableLocalReaderMock";
};

static std::shared_ptr<AnswerToReset> getAAtr()
{
    return std::make_shared<AnswerToReset>(
               ByteArrayUtil::fromHex("3B8F8001804F0CA000000306030001000000006A"));
}

static std::shared_ptr<ApduResponse> getAFCI()
{
    return std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("9000"), nullptr);
}

static std::shared_ptr<ApduResponse> getSuccessfullResponse()
{
    return std::make_shared<ApduResponse>(ByteArrayUtil::fromHex("FEDCBA98 9000h"), nullptr);
}

static std::vector<std::shared_ptr<ApduResponse>> getAListOfAPDUs()
{
    std::vector<std::shared_ptr<ApduResponse>> apdus;
    apdus.push_back(getSuccessfullResponse());

    return apdus;
}

static std::shared_ptr<SeResponse> getASeResponse()
{
    auto status = std::make_shared<SelectionStatus>(getAAtr(), getAFCI(), true);

    return std::make_shared<SeResponse>(true, true, status, getAListOfAPDUs());
}

static std::vector<std::shared_ptr<SeResponse>> getSeResponses()
{
    std::vector<std::shared_ptr<SeResponse>> responses;
    responses.push_back(getASeResponse());

    return responses;
}

static std::shared_ptr<AbstractObservableLocalReader> getSpy(const std::string& pluginName,
                                                             const std::string& readerName)
{
    auto r = std::make_shared<AbstractObservableLocalReaderMock>(pluginName, readerName);

    ON_CALL(*r, processSeRequest(_,_)).WillByDefault(Return(getASeResponse()));
    ON_CALL(*r, processSeRequests(_,_,_)).WillByDefault(Return(getSeResponses()));

    return r;
}

/* ==== Card event ====================================== */
static const std::string PLUGIN_NAME = "AbsObservableLocalReaderTestP";
static const std::string READER_NAME = "AbsObservableLocalReaderTest";

static const std::vector<uint8_t> RESP_SUCCESS = {0x90, 0x00};
static const std::vector<uint8_t> RESP_FAIL = {0x00, 0x00};

static const int STATUS_CODE_1 = 1;
static const int STATUS_CODE_2 = 2;
static const std::set<int> _STATUS_CODE_LIST = {STATUS_CODE_1, STATUS_CODE_2};
static const std::shared_ptr<std::set<int>> STATUS_CODE_LIST =
    std::make_shared<std::set<int>>(_STATUS_CODE_LIST);

static const std::vector<std::shared_ptr<ApduResponse>> empty;

static std::vector<std::shared_ptr<SeResponse>> getMatchingResponses()
{
    auto response = std::make_shared<ApduResponse>(RESP_SUCCESS, STATUS_CODE_LIST);
    auto selectionStatus = std::make_shared<SelectionStatus>(nullptr, response, false);
    auto seResponse = std::make_shared<SeResponse>(true, false, selectionStatus, empty);

    std::vector<std::shared_ptr<SeResponse>> responses;
    responses.push_back(seResponse);

    return responses;
}

static std::vector<std::shared_ptr<SeResponse>> getNotMatchingResponses()
{
    auto response = std::make_shared<ApduResponse>(RESP_FAIL, STATUS_CODE_LIST);
    auto selectionStatus = std::make_shared<SelectionStatus>(nullptr, response, false);
    auto seResponse = std::make_shared<SeResponse>(false, false, selectionStatus, empty);

    std::vector<std::shared_ptr<SeResponse>> responses;
    responses.push_back(seResponse);

    return responses;
}

/* No default selection */
TEST(DefaultSelection, seInserted)
{
    /* Empty reader */
    std::shared_ptr<AbstractObservableLocalReader> spy = getSpy(PLUGIN_NAME, READER_NAME);
    auto r = std::dynamic_pointer_cast<AbstractObservableLocalReaderMock>(spy);

    /* Test */
    std::shared_ptr<ReaderEvent> event = r->processSeInsertedTest();

    ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_INSERTED);
    ASSERT_EQ(event->getDefaultSelectionsResponse(), nullptr);
    ASSERT_EQ(event->getPluginName(), PLUGIN_NAME);
    ASSERT_EQ(event->getReaderName(), READER_NAME);
}

/* Selection is not successful */
TEST(DefaultSelection, seInserted_ALWAYS)
{
    std::shared_ptr<AbstractObservableLocalReader> spy = getSpy(PLUGIN_NAME, READER_NAME);
    auto r = std::dynamic_pointer_cast<AbstractObservableLocalReaderMock>(spy);

    /* Configure parameters */
    std::vector<std::shared_ptr<SeRequest>> selections;
    MultiSeRequestProcessing multi = MultiSeRequestProcessing::PROCESS_ALL;
    ChannelControl channel = ChannelControl::CLOSE_AFTER;
    ObservableReader::NotificationMode mode = ObservableReader::NotificationMode::ALWAYS;

    /* Mock return matching selection */
    std::vector<std::shared_ptr<SeResponse>> responses = getNotMatchingResponses();

    EXPECT_CALL(*r, transmitSeRequests(selections, multi, channel))
        .Times(1)
        .WillOnce(Return(responses));

    /* Test */
    r->setDefaultSelectionRequest(
        std::make_shared<DefaultSelectionsRequest>(selections, multi, channel),
        mode);

    std::shared_ptr<ReaderEvent> event = r->processSeInsertedTest();

    /* Assert */
    ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_INSERTED);
    ASSERT_EQ(event->getDefaultSelectionsResponse()->getSelectionSeResponses(), responses);
    ASSERT_EQ(event->getPluginName(), PLUGIN_NAME);
    ASSERT_EQ(event->getReaderName(), READER_NAME);
}

/* Selection is successful */
TEST(DefaultSelection, seMatched_MATCHED_ONLY)
{
    std::shared_ptr<AbstractObservableLocalReader> spy = getSpy(PLUGIN_NAME, READER_NAME);
    auto r = std::dynamic_pointer_cast<AbstractObservableLocalReaderMock>(spy);

    /* Configure parameters */
    std::vector<std::shared_ptr<SeRequest>> selections;
    MultiSeRequestProcessing multi = MultiSeRequestProcessing::PROCESS_ALL;
    ChannelControl channel = ChannelControl::CLOSE_AFTER;
    ObservableReader::NotificationMode mode = NotificationMode::MATCHED_ONLY;

    /* Mock return success selection */
    std::vector<std::shared_ptr<SeResponse>> responses =
        getMatchingResponses();

    EXPECT_CALL(*r, transmitSeRequests(selections, multi, channel))
        .Times(1)
        .WillOnce(Return(responses));

    /* Test */
    r->setDefaultSelectionRequest(
        std::make_shared<DefaultSelectionsRequest>(selections, multi, channel),
        mode);

    std::shared_ptr<ReaderEvent> event = r->processSeInsertedTest();

    ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_MATCHED);
    ASSERT_EQ(event->getDefaultSelectionsResponse()->getSelectionSeResponses(), responses);
    ASSERT_EQ(event->getPluginName(), PLUGIN_NAME);
    ASSERT_EQ(event->getReaderName(), READER_NAME);
}

/* Selection is not successful */
TEST(DefaultSelection, noEvent_MATCHED_ONLY)
{
    std::shared_ptr<AbstractObservableLocalReader> spy = getSpy(PLUGIN_NAME, READER_NAME);
    auto r = std::dynamic_pointer_cast<AbstractObservableLocalReaderMock>(spy);

    /* Configure parameters */
    std::vector<std::shared_ptr<SeRequest>> selections;
    MultiSeRequestProcessing multi = MultiSeRequestProcessing::PROCESS_ALL;
    ChannelControl channel = ChannelControl::CLOSE_AFTER;
    ObservableReader::NotificationMode mode = NotificationMode::MATCHED_ONLY;

    /* Mock return matching selection */
    EXPECT_CALL(*r, transmitSeRequests(selections, multi, channel))
        .Times(1)
        .WillOnce(Return(getNotMatchingResponses()));

    /* Test */
    r->setDefaultSelectionRequest(
        std::make_shared<DefaultSelectionsRequest>(selections, multi, channel),
        mode);

    std::shared_ptr<ReaderEvent> event = r->processSeInsertedTest();

    ASSERT_EQ(event, nullptr);
}

/*
 * Simulate an IOException while selecting Do not throw any event Nor an
 * exception
 */
TEST(DefaultSelection, noEvent_IOError)
{
    std::shared_ptr<AbstractObservableLocalReader> spy = getSpy(PLUGIN_NAME, READER_NAME);
    auto r = std::dynamic_pointer_cast<AbstractObservableLocalReaderMock>(spy);

    /* Configure parameters */
    std::vector<std::shared_ptr<SeRequest>> selections;
    MultiSeRequestProcessing multi = MultiSeRequestProcessing::PROCESS_ALL;
    ChannelControl channel = ChannelControl::CLOSE_AFTER;
    ObservableReader::NotificationMode mode =
        ObservableReader::NotificationMode::ALWAYS;

    /* Throw IO */
    EXPECT_CALL(*r, transmitSeRequests(selections, multi, channel))
        .Times(1)
        .WillRepeatedly(Throw(KeypleReaderIOException("io error when selecting")));


    /* Test */
    r->setDefaultSelectionRequest(
        std::make_shared<DefaultSelectionsRequest>(selections, multi, channel),
        mode);
    r->processSeInsertedTest();

    std::shared_ptr<ReaderEvent> event = r->processSeInsertedTest();

    ASSERT_EQ(event, nullptr);
}

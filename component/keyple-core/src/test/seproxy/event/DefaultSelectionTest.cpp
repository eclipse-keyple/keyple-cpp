/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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
#include "KeypleReaderIOException.h"

using namespace testing;

using namespace keyple::core::seproxy::event;

class AbstractObservableLocalReaderMock : public AbstractObservableLocalReader {
public:
    AbstractObservableLocalReaderMock(
      const std::string& pluginName, const std::string& readerName)
    : AbstractObservableLocalReader(pluginName, readerName) {}


};

/* ==== Card event ====================================== */
static const std::string PLUGIN_NAME = "AbsObservableLocalReaderTestP";
static const std::string READER_NAME = "AbsObservableLocalReaderTest";

static const std::vector<uint8_t> RESP_SUCCESS = {0x90, 0x00};
static const std::vector<uint8_t> RESP_FAIL = {0x00, 0x00};

static const int STATUS_CODE_1 = 1;
static const int STATUS_CODE_2 = 2;
static const std::vector<int> STATUS_CODE_LIST = {STATUS_CODE_1, STATUS_CODE_2};

static const std::vector<std::shared_ptr<ApduResponse>> empty;

static std::vector<std::shared_ptr<SeResponse>> getMatchingResponses()
{
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            nullptr,
            std::make_shared<ApduResponse>(RESP_SUCCESS, STATUS_CODE_LIST),
            false);

    std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(true, false, selectionStatus, empty);

    std::vector<std::shared_ptr<SeResponse> responses;
    responses.push_back(seResponse);

    return responses;
}

static std::vector<std::shared_ptr<SeResponse>> getNotMatchingResponses()
{
    std::shared_ptr<SelectionStatus> selectionStatus =
        std::make_shared<SelectionStatus>(
            nullptr,
            std::make_shared<ApduResponse>(RESP_FAIL, STATUS_CODE_LIST),
            false);

    std::shared_ptr<SeResponse> seResponse =
        std::make_shared<SeResponse>(false, false, selectionStatus, empty);

    std::vector<std::shared_ptr<SeResponse> responses;
    responses.push_back(seResponse);

    return responses;
}

/* No default selection */
TEST(DefaultSelection, seInserted)
{
    /* Empty reader */
    AbstractObservableLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    /* Test */
    std::shared_ptr<ReaderEvent> event = r.processSeInserted();

    ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_INSERTED);
    ASSERT_EQ(event->getDefaultSelectionsResponse(), nullptr);
    ASSERT_EQ(event->getPluginName(), PLUGIN_NAME);
    ASSERT_EQ(event->getReaderName(), READER_NAME);
}

/* Selection is not successful */
TEST(DefaultSelection, seInserted_ALWAYS)
{
    AbstractObservableLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    // configure parameters
    std::vector<std::shared_ptr<SeRequest>> selections;
    MultiSeRequestProcessing multi = MultiSeRequestProcessing::PROCESS_ALL;
    ChannelControl channel = ChannelControl::CLOSE_AFTER;
    ObservableReader::NotificationMode mode =
        ObservableReader::NotificationMode::ALWAYS;

    /* Mock return matching selection */
    std::vector<std::shared_ptr<SeResponse>> responses =
        getNotMatchingResponses();

    EXPECT_CALL(r, transmitSet(selections, multi, channel))
        .Times(1)
        .WillOnce(Return(responses));

    /* Test */
    r.setDefaultSelectionRequest(
        std::make_shared<DefaultSelectionsRequest>(selections, multi, channel),
        mode);

    std::shared_ptr<ReaderEvent> event = r.processSeInserted();

    /* Assert */
    ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_INSERTED);
    ASSERT_EQ(event->getDefaultSelectionsResponse()
              ->getSelectionSeResponseSet(), responses);
    ASSERT_EQ(event->getPluginName(), PLUGIN_NAME);
    ASSERT_EQ(event->getReaderName(), READER_NAME);
}

/* Selection is successful */
TEST(DefaultSelection, seMatched_MATCHED_ONLY()
{
    AbstractObservableLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    /* Configure parameters */
    std::vector<std::shared_ptr<SeRequest>> selections;
    MultiSeRequestProcessing multi = MultiSeRequestProcessing::PROCESS_ALL;
    ChannelControl channel = ChannelControl::CLOSE_AFTER;
    ObservableReader::NotificationMode mode =
        ObservableReader::NotificationMode::MATCHED_ONLY;

    /* Mock return success selection */
    std::vector<std::shared_ptr<SeResponse>> responses =
        getMatchingResponses();

    EXPECT_CALL(r, transmitSet(selections, multi, channel))
        .Times(1)
        .WillOnce(Return(responses));

    /* Test */
    r.setDefaultSelectionRequest(
        std::make_shared<DefaultSelectionsRequest>(selections, multi, channel),
        mode);

    std::shared_ptr<ReaderEvent> event = r.processSeInserted();

    ASSERT_EQ(event->getEventType(), ReaderEvent::EventType::SE_MATCHED);
    ASSERT_EQ(event->getDefaultSelectionsResponse()
              ->getSelectionSeResponseSet(), responses);
    ASSERT_EQ(event->getPluginName(), PLUGIN_NAME);
    ASSERT_EQ(event->getReaderName(), READER_NAME);
}

/* Selection is not successful */
TEST(DefaultSelection, noEvent_MATCHED_ONLY()
{
    AbstractObservableLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    /* Configure parameters */
    std::vector<std::shared_ptr<SeRequest>> selections;
    MultiSeRequestProcessing multi = MultiSeRequestProcessing::PROCESS_ALL;
    ChannelControl channel = ChannelControl::CLOSE_AFTER;
    ObservableReader::NotificationMode mode =
        ObservableReader::NotificationMode::MATCHED_ONLY;

    /* Mock return matching selection */
    EXPECT_CALL(r, transmitSet(selections, multi, channel))
        .Times(1)
        .WillOnce(Return(getNotMatchingResponses()));

    /* Test */
    r.setDefaultSelectionRequest(
        std::make_shared<DefaultSelectionsRequest>(selections, multi, channel),
        mode);

    std::shared_ptr<ReaderEvent> event = r.processSeInserted();

    ASSERT_EQ(event, nullptr);
}

/*
 * Simulate an IOException while selecting Do not throw any event Nor an
 * exception
 */
TEST(DefaultSelection, noEvent_IOError()
{
    AbstractObservableLocalReaderMock r(PLUGIN_NAME, READER_NAME);

    /* Configure parameters */
    std::vector<std::shared_ptr<SeRequest>> selections;
    MultiSeRequestProcessing multi = MultiSeRequestProcessing::PROCESS_ALL;
    ChannelControl channel = ChannelControl::CLOSE_AFTER;
    ObservableReader::NotificationMode mode =
        ObservableReader::NotificationMode::ALWAYS;

    /* Throw IO */
    EXPECT_CALL(r, transmitSet(selections, multi, channel))
        .Times(1)
        .WillRepeatedly(
            Throw(KeypleReaderIOException("io error when selecting")));


    /* Test */
    r.setDefaultSelectionRequest(
        std::make_shared<DefaultSelectionsRequest>(selections, multi, channel),
        mode);

    std::shared_ptr<ReaderEvent> event = r.processSeInserted();

    ASSERT_EQ(event, nullptr);
}

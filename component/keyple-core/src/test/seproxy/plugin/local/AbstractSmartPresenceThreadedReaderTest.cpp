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
#include "SmartInsertionReader.h"

#include "AbstractObservableState.h"
#include "CardAbsentPingMonitoringJob.h"
#include "InterruptedException.h"
#include "MonitoringPool.h"
#include "ObservableReaderStateService.h"
#include "SmartInsertionMonitoringJob.h"
#include "SmartInsertionReader.h"
#include "SmartRemovalMonitoringJob.h"
#include "SmartRemovalReader.h"
#include "Thread.h"
#include "WaitForStartDetect.h"
#include "WaitForSeInsertion.h"
#include "WaitForSeProcessing.h"
#include "WaitForSeRemoval.h"

using namespace testing;

using namespace keyple::core::seproxy::plugin::local;
using namespace keyple::core::seproxy::plugin::local::monitoring;
using namespace keyple::core::seproxy::plugin::local::state;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

class ASPTR_SmartPresenceTheadedReaderMock
: public AbstractObservableLocalReader, public SmartRemovalReader,
  public SmartInsertionReader
{
public:
    /**
     * Reader constructor
     * <p>
     * Force the definition of a name through the use of super method.
     * <p>
     *
     * @param pluginName the name of the plugin that instantiated the reader
     * @param readerName the name of the reader
     */
    ASPTR_SmartPresenceTheadedReaderMock(
      const std::string& pluginName, const std::string& readerName,
      int mockDetect)
    : AbstractObservableLocalReader(pluginName, readerName),
      mMockDetect(mockDetect) {}

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

    MOCK_METHOD(std::vector<uint8_t>,
                transmitApdu,
                (const std::vector<uint8_t>&),
                (override));

    const TransmissionMode& getTransmissionMode() const override
    {
        return mTransmissionMode;
    }

    const std::map<const std::string, const std::string>& getParameters() const
        override
    {
        return mParameters;
    }

    void setParameter(const std::string& key, const std::string& value) override
    {
        (void)key;
        (void)value;
    }

    bool waitForCardAbsentNative() override
    {
        if (!mRemovedOnlyOnce) {
            mRemovedOnlyOnce = true;
            return true;
        }

        return false;
    }

    void stopWaitForCardRemoval() override {}

    void stopWaitForCard() override {}

    bool waitForCardPresent() override
    {
        mDetectCount++;

        return mDetectCount <= mMockDetect;
    }

    std::shared_ptr<ObservableReaderStateService> initStateService() override
    {
        std::map<MonitoringState, std::shared_ptr<AbstractObservableState>>
            states;

        states.insert(
            {MonitoringState::WAIT_FOR_START_DETECTION,
             std::make_shared<WaitForStartDetect>(this)});

        states.insert(
            {MonitoringState::WAIT_FOR_SE_INSERTION,
                std::make_shared<WaitForSeInsertion>(
                    this, std::make_shared<SmartInsertionMonitoringJob>(this),
                    mExecutorService)});

        states.insert(
            {MonitoringState::WAIT_FOR_SE_PROCESSING,
                std::make_shared<WaitForSeProcessing>(
                    this, std::make_shared<SmartRemovalMonitoringJob>(this),
                    mExecutorService)});

        states.insert(
            {MonitoringState::WAIT_FOR_SE_REMOVAL,
             std::make_shared<WaitForSeRemoval>(
                 this, std::make_shared<SmartRemovalMonitoringJob>(this),
                 mExecutorService)});

        return std::make_shared<ObservableReaderStateService>(
                   this, states, MonitoringState::WAIT_FOR_START_DETECTION);
    }

    MOCK_METHOD(bool,
                isSePresentPing,
                (),
                ());

protected:
    std::shared_ptr<MonitoringPool> mExecutorService =
        std::make_shared<MonitoringPool>();

private:
    int mMockDetect;
    int mDetectCount = 0;
    volatile bool mRemovedOnlyOnce = false;
    std::vector<uint8_t> mAtr;
    TransmissionMode mTransmissionMode;
    std::map<const std::string, const std::string> mParameters;
};

class ASPTR_ReaderObserverMock : public ObservableReader::ReaderObserver {
public:
    void update(std::shared_ptr<ReaderEvent> event) override
    {
        (void)event;
    }
};

static const std::string PLUGIN_NAME = "AbsSmartPresenceTheadedReaderTestP";
static const std::string READER_NAME = "AbsSmartPresenceTheadedReaderTest";

ASPTR_SmartPresenceTheadedReaderMock r(PLUGIN_NAME, READER_NAME, 1);

static std::shared_ptr<ObservableReader::ReaderObserver> getObs()
{
    return std::shared_ptr<ASPTR_ReaderObserverMock>();
}

TEST(AbstractSmartPresenceThreadedReaderTest, startRemovalSequence)
{
    /* SE matched */
    //doReturn(true).when(r).processSeInserted();

    r.addObserver(getObs());
    Thread::sleep(100);

    r.startRemovalSequence();
    Thread::sleep(100);

    /* Does nothing */
    ASSERT_EQ(r.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_START_DETECTION);
}

TEST(AbstractSmartPresenceThreadedReaderTest, startRemovalSequence_CONTINUE)
{
    /* SE matched */
    //doReturn(true).when(r).processSeInserted();

    r.addObserver(getObs());
    r.startSeDetection(ObservableReader::PollingMode::REPEATING);
    Thread::sleep(100);

    r.startRemovalSequence();
    Thread::sleep(100);

    ASSERT_EQ(r.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_SE_INSERTION);
}

TEST(AbstractSmartPresenceThreadedReaderTest, startRemovalSequence_noping_STOP)
{
    /* SE matched */
    //doReturn(true).when(r).processSeInserted();
    EXPECT_CALL(r, isSePresentPing())
        .Times(1)
        .WillOnce(Return(false));

    r.addObserver(getObs());
    r.startSeDetection(ObservableReader::PollingMode::SINGLESHOT);
    Thread::sleep(100);

    r.startRemovalSequence();
    Thread::sleep(100);

    ASSERT_EQ(r.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_SE_INSERTION);
}

TEST(AbstractSmartPresenceThreadedReaderTest, startRemovalSequence_ping_STOP)
{
    /* SE matched */
    //doReturn(true).when(r).processSeInserted();
    // doReturn(true).when(r).isSePresentPing();
    //EXPECT_CALL(r, isSePresent())
    //    .Times(1)
    //    .WillOnce(Return(true));

    r.addObserver(getObs());
    r.startSeDetection(ObservableReader::PollingMode::SINGLESHOT);
    Thread::sleep(100);

    r.startRemovalSequence();
    Thread::sleep(100);

    ASSERT_EQ(r.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_START_DETECTION);
}

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
#include "SmartInsertionReader.h"

#include "AbstractObservableState.h"
#include "CardAbsentPingMonitoringJob.h"
#include "InterruptedException.h"
#include "ExecutorService.h"
#include "ObservableReaderStateService.h"
#include "SmartInsertionMonitoringJob.h"
#include "Thread.h"
#include "WaitForStartDetect.h"
#include "WaitForSeInsertion.h"
#include "WaitForSeProcessing.h"
#include "WaitForSeRemoval.h"

using namespace testing;

using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

class ASITR_SmartInsertionReaderMock
: public AbstractObservableLocalReader, public SmartInsertionReader {
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
    ASITR_SmartInsertionReaderMock(
      const std::string& pluginName, const std::string& readerName,
      int mockDetect)
    : AbstractObservableLocalReader(pluginName, readerName),
      mMockDetect(mockDetect)
    {
        mStateService = initStateService();
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
                std::make_shared<WaitForSeProcessing>(this)});

        states.insert(
            {MonitoringState::WAIT_FOR_SE_REMOVAL,
             std::make_shared<WaitForSeRemoval>(
                 this, std::make_shared<CardAbsentPingMonitoringJob>(this),
                 mExecutorService)});


        return std::make_shared<ObservableReaderStateService>(
                   this, states, MonitoringState::WAIT_FOR_START_DETECTION);
    }

/*
    private Runnable waitForCardPresentFuture() {
        return new Runnable() {
            @Override
            public void run() {
                logger.trace("[{}] Invoke waitForCardPresent asynchronously",
                        BlankSmartInsertionTheadedReader.this.getName());
                try {
                    if (BlankSmartInsertionTheadedReader.this.waitForCardPresent()) {
                        onEvent(AbstractObservableLocalReader.InternalEvent.SE_INSERTED);
                    }
                } catch (KeypleReaderIOException e) {
                    logger.trace(
                            "[{}] waitForCardPresent => Error while polling card with waitForCardPresent",
                            BlankSmartInsertionTheadedReader.this.getName());
                    onEvent(AbstractObservableLocalReader.InternalEvent.STOP_DETECT);
                }
            }
        };
    }
*/

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

    bool waitForCardPresent() override
    {
        mDetectCount++;

        try {
            Thread::sleep(10);
        } catch (const InterruptedException& e) {
            (void)e;
        }

        return mDetectCount <= mMockDetect;
    }

    void stopWaitForCard() override {}

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

    const std::string& getName() const override
    {
        return mName;
    }

private:

    int mMockDetect;
    int mDetectCount = 0;
    std::shared_ptr<ExecutorService> mExecutorService = std::make_shared<ExecutorService>();
    std::vector<uint8_t> mAtr;
    std::vector<uint8_t> mRApdu;
    std::map<const std::string, const std::string> mParameters;
    TransmissionMode mTransmissionMode = TransmissionMode::NONE;
    const std::string mName = "ASITR_SmartInsertionReaderMock";
};

class ASITR_ReaderObserverMock : public ObservableReader::ReaderObserver {
public:
    void update(std::shared_ptr<ReaderEvent> event) override
    {
        (void)event;
    }
};

const std::string PLUGIN_NAME = "AbsSmartInsertionTheadedReaderTestP";
const std::string READER_NAME = "AbsSmartInsertionTheadedReaderTest";

static std::shared_ptr<ObservableReader::ReaderObserver> getObs()
{
    return std::make_shared<ASITR_ReaderObserverMock>();
}

/* Observers management + Thread instantiation ============================== */

TEST(AbstractSmartInsertionThreadedReaderTest, addObserver)
{
    ASITR_SmartInsertionReaderMock r(PLUGIN_NAME, READER_NAME, 0);

    /* Add observer */
    r.addObserver(getObs());

    /* Should the thread start */
    ASSERT_EQ(r.countObservers(), 1);
    ASSERT_EQ(r.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_START_DETECTION);
}

TEST(AbstractSmartInsertionThreadedReaderTest, removeObserver)
{
    ASITR_SmartInsertionReaderMock r(PLUGIN_NAME, READER_NAME, 0);

    std::shared_ptr<ObservableReader::ReaderObserver> obs = getObs();

    /* Add and remove observer */
    r.addObserver(obs);
    r.removeObserver(obs);

    /* Should the thread start */
    ASSERT_EQ(r.countObservers(), 0);
    ASSERT_EQ(r.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_START_DETECTION);
}

TEST(AbstractSmartInsertionThreadedReaderTest, clearObservers)
{
    ASITR_SmartInsertionReaderMock r(PLUGIN_NAME, READER_NAME, 0);

    /* Add and remove observer */
    r.addObserver(getObs());
    r.clearObservers();

    /* Should the thread start and stop */
    ASSERT_EQ(r.countObservers(), 0);
    ASSERT_EQ(r.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_START_DETECTION);
}

/* SMART CARD DETECTION ===================================================== */

TEST(AbstractSmartInsertionThreadedReaderTest, startSeDetection)
{
    /* Do not present any card for this test */
    ASITR_SmartInsertionReaderMock r(PLUGIN_NAME, READER_NAME, 0);

    r.addObserver(getObs());
    r.startSeDetection(ObservableReader::PollingMode::SINGLESHOT);

    Thread::sleep(500);

    ASSERT_EQ(r.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_SE_INSERTION);

    r.stopSeDetection();
}

TEST(AbstractSmartInsertionThreadedReaderTest, stopSeDetection)
{
    /* Do not present any card for this test */
    ASITR_SmartInsertionReaderMock r(PLUGIN_NAME, READER_NAME, 0);

    r.addObserver(getObs());
    r.startSeDetection(ObservableReader::PollingMode::SINGLESHOT);
    r.stopSeDetection();

    ASSERT_EQ(r.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_START_DETECTION);
}

/* isSePresentPing ========================================================== */

TEST(AbstractSmartInsertionThreadedReaderTest, isSePresentPing_true)
{
    ASITR_SmartInsertionReaderMock r(PLUGIN_NAME, READER_NAME, 0);

    std::vector<uint8_t> rApdu = {0x00};

    EXPECT_CALL(r, transmitApdu(_))
        .Times(1)
        .WillOnce(Return(rApdu));

    ASSERT_TRUE(r.isSePresentPing());
}

TEST(AbstractSmartInsertionThreadedReaderTest, isSePresentPing_false)
{
    ASITR_SmartInsertionReaderMock r(PLUGIN_NAME, READER_NAME, 0);

    EXPECT_CALL(r, transmitApdu(_))
        .Times(1)
        .WillOnce(Throw(KeypleReaderIOException("ping failed")));

    ASSERT_FALSE(r.isSePresentPing());
}

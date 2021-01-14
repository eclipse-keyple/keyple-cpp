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

#include "ObservableReaderStateService.h"

#include "WaitForSeInsertion.h"
#include "WaitForSeProcessing.h"
#include "WaitForSeRemoval.h"
#include "WaitForStartDetect.h"

using namespace testing;

using namespace keyple::core::seproxy::plugin;

class AOSS_AbstractObservableLocalReader
: public AbstractObservableLocalReader {
public:
    AOSS_AbstractObservableLocalReader(
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

    MOCK_METHOD((std::shared_ptr<SelectionStatus>),
                openLogicalChannel,
                (std::shared_ptr<SeSelector> seSelector),
                (override));

    MOCK_METHOD(bool,
                protocolFlagMatches,
                (const std::shared_ptr<SeProtocol>),
                (override));

    MOCK_METHOD(std::vector<uint8_t>,
                transmitApdu,
                (const std::vector<uint8_t>&),
                (override));

    std::shared_ptr<ObservableReaderStateService> initStateService() override
    {
        mStates.insert(
            std::pair<MonitoringState,
                      std::shared_ptr<AbstractObservableState>>(
                MonitoringState::WAIT_FOR_START_DETECTION,
                std::make_shared<WaitForStartDetect>(this)));

        mStates.insert(
            std::pair<MonitoringState,
                      std::shared_ptr<AbstractObservableState>>(
                MonitoringState::WAIT_FOR_SE_INSERTION,
                std::make_shared<WaitForSeInsertion>(this)));

        mStates.insert(
            std::pair<MonitoringState,
                      std::shared_ptr<AbstractObservableState>>(
                MonitoringState::WAIT_FOR_SE_PROCESSING,
                std::make_shared<WaitForSeProcessing>(this)));

        mStates.insert(
            std::pair<MonitoringState,
                      std::shared_ptr<AbstractObservableState>>(
                MonitoringState::WAIT_FOR_SE_REMOVAL,
                std::make_shared<WaitForSeRemoval>(this)));

        return std::make_shared<ObservableReaderStateService>(
            this, mStates, MonitoringState::WAIT_FOR_START_DETECTION);
    }

    const std::map<MonitoringState, std::shared_ptr<AbstractObservableState>>
        getStates()
    {
        return mStates;
    }

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

    const std::string& getName() const override
    {
        return mName;
    }

    const std::map<std::shared_ptr<SeProtocol>, std::string>& getProtocolsMap() const
    {
        return AbstractLocalReader::getProtocolsMap();
    }

private:
    std::map<MonitoringState, std::shared_ptr<AbstractObservableState>> mStates;
    const std::string mName = "AOSS_AbstractObservableLocalReader";
};

TEST(ObservableReaderStateServiceTest, ObservableReaderStateService)
{
    AOSS_AbstractObservableLocalReader* reader =
        new AOSS_AbstractObservableLocalReader("plugin", "reader");

    ObservableReaderStateService service(
        reader, reader->getStates(), MonitoringState::WAIT_FOR_SE_INSERTION);

}

TEST(ObservableReaderStateServiceTest, getCurrentMonitoringState)
{
    AOSS_AbstractObservableLocalReader* reader =
        new AOSS_AbstractObservableLocalReader("plugin", "reader");

    ObservableReaderStateService service(
        reader, reader->getStates(), MonitoringState::WAIT_FOR_SE_INSERTION);

    ASSERT_EQ(service.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_SE_INSERTION);
}

TEST(ObservableReaderStateServiceTest, switchState)
{
    AOSS_AbstractObservableLocalReader* reader =
        new AOSS_AbstractObservableLocalReader("plugin", "reader");

    ObservableReaderStateService service(
        reader, reader->getStates(), MonitoringState::WAIT_FOR_SE_INSERTION);

    service.switchState(MonitoringState::WAIT_FOR_SE_PROCESSING);

    ASSERT_EQ(service.getCurrentMonitoringState(),
              MonitoringState::WAIT_FOR_SE_PROCESSING);
}

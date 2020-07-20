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

#include "AbstractObservableState.h"

#include "AbstractObservableLocalReader.h"
#include "ObservableReaderStateService.h"
#include "SeCommonProtocols.h"
#include "WaitForSeInsertion.h"
#include "WaitForSeProcessing.h"
#include "WaitForSeRemoval.h"
#include "WaitForStartDetect.h"

using namespace testing;

using namespace keyple::core::seproxy::plugin::local;
using namespace keyple::core::seproxy::plugin::local::state;

class AOS_AbstractObservableLocalReader : public AbstractObservableLocalReader {
public:
    AOS_AbstractObservableLocalReader(
      const std::string& pluginName, const std::string& readerName)
    : AbstractObservableLocalReader(pluginName, readerName)
    {
        this->stateService = initStateService();
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
};

class AOS_ObservableStateMock : public AbstractObservableState {
public:
    AOS_ObservableStateMock()
    : AbstractObservableState(
          MonitoringState::WAIT_FOR_START_DETECTION,
          new AOS_AbstractObservableLocalReader("plugin", "reader"))
    {
    }

    void onEvent(const InternalEvent event)
    {
        switch (event) {
        case InternalEvent::START_DETECT:
            switchState(MonitoringState::WAIT_FOR_SE_INSERTION);
            break;
        case InternalEvent::SE_INSERTED:
            switchState(MonitoringState::WAIT_FOR_SE_PROCESSING);
            break;
        case InternalEvent::SE_PROCESSED:
            switchState(MonitoringState::WAIT_FOR_SE_REMOVAL);
            break;
        case InternalEvent::SE_REMOVED:
            switchState(MonitoringState::WAIT_FOR_START_DETECTION);
            break;
        case InternalEvent::STOP_DETECT:
            switchState(MonitoringState::WAIT_FOR_START_DETECTION);
            break;
        case InternalEvent::TIME_OUT:
            switchState(MonitoringState::WAIT_FOR_START_DETECTION);
            break;
        }
    }
};

TEST(AbstractObservableStateTest, AbstractObservableState)
{
    AOS_ObservableStateMock state;
}

TEST(AbstractObservableStateTest, onEvent)
{
     AOS_ObservableStateMock state;

     state.onEvent(InternalEvent::START_DETECT);

     ASSERT_EQ(state.getMonitoringState(),
               MonitoringState::WAIT_FOR_START_DETECTION);
}

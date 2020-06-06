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

class ObservableStateMock : public AbstractObservableState {
public:
    ObservableStateMock()
    : AbstractObservableState(MonitoringState::WAIT_FOR_START_DETECTION,
                              new ObservableLocalReaderMock("plugin", "reader"))
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
    ObservableStateMock state;
}

TEST(AbstractObservableStateTest, onEvent)
{
     ObservableStateMock state;

     state.onEvent(InternalEvent::START_DETECT);

     ASSERT_EQ(state.getMonitoringState(),
               MonitoringState::WAIT_FOR_START_DETECTION);
}

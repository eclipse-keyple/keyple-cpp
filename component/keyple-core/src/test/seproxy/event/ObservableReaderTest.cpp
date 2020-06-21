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

#include <algorithm>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ObservableReader.h"

using namespace testing;

using namespace keyple::core::seproxy::event;

class ObservableReaderMock : public ObservableReader {
public:
    ObservableReaderMock(const TransmissionMode& transmissionMode)
    : ObservableReader(), transmissionMode(transmissionMode)
    {
    }

    const std::string& getName() const override
    {
        return name;
    }

    const std::map<const std::string, const std::string> getParameters() const
        override
    {
        return mPparameters;
    }

    void setParameter(const std::string& key, const std::string& value) override
    {
        (void)key;
        (void)value;
    }

    void setParameters(
        const std::map<const std::string, const std::string>& parameters)
        override
    {
        (void)parameters;
    }

    void addObserver(const std::shared_ptr<ReaderObserver> observer) override
    {
        observers.push_back(observer);
    }

    void removeObserver(const std::shared_ptr<ReaderObserver> observer) override
    {
        observers.erase(std::remove(observers.begin(), observers.end(),
	                            observer), observers.end());
    }

    void notifyObservers(const std::shared_ptr<ReaderEvent> event) override
    {
        (void)event;
    }

    void clearObservers() override
    {
        observers.clear();
    }

    int countObservers() const override
    {
        return observers.size();
    }

    void startSeDetection(PollingMode pollingMode) override
    {
        (void)pollingMode;
    }

    void stopSeDetection() override
    {
    }

    bool isSePresent() override
    {
        return true;
    }

    void addSeProtocolSetting(std::shared_ptr<SeProtocol> seProtocol,
                              const std::string& protocolRule) override
    {
        (void)seProtocol;
        (void)protocolRule;
    }

    void setSeProtocolSetting(const std::map<std::shared_ptr<SeProtocol>,
                              std::string>& protocolSetting) override
    {
        (void)protocolSetting;
    }

    void setDefaultSelectionRequest(
        std::shared_ptr<AbstractDefaultSelectionsRequest>
            defaultSelectionsRequest,
        NotificationMode notificationMode,
        PollingMode pollingMode) override
    {
        (void)defaultSelectionsRequest;
        (void)notificationMode;
        (void)pollingMode;
    }

    void notifySeProcessed() override
    {
    }

    const TransmissionMode& getTransmissionMode() const override
    {
        return transmissionMode;
    }

    void setDefaultSelectionRequest(
        std::shared_ptr<AbstractDefaultSelectionsRequest>
            defaultSelectionsRequest,
        NotificationMode notificationMode) override
    {
        (void)defaultSelectionsRequest;
        (void)notificationMode;
    }

private:
    std::string name;

    std::map<const std::string, const std::string> mPparameters;

    std::vector<std::shared_ptr<ReaderObserver>> observers;

    const TransmissionMode transmissionMode;
};

TEST(ObservableReaderTest, ObservableReader)
{
    ObservableReaderMock reader(TransmissionMode::CONTACTLESS);
}



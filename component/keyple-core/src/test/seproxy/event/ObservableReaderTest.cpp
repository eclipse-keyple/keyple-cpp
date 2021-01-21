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

#include <algorithm>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ObservableReader.h"

using namespace testing;

using namespace keyple::core::seproxy::event;

class ObservableReaderMock : public ObservableReader {
public:
    MOCK_METHOD((const std::string&),
                getName,
                (),
                (const, override));

    MOCK_METHOD((const std::map<const std::string, const std::string>&),
                getParameters,
                (),
                (const, override));

    MOCK_METHOD(void,
                setParameter,
                (const std::string&, const std::string&),
                (override));

    MOCK_METHOD(void,
                setParameters,
                ((const std::map<const std::string, const std::string>&)),
                (override));

    MOCK_METHOD(void,
                addObserver,
                (const std::shared_ptr<ReaderObserver>),
                (override));

    MOCK_METHOD(void,
                removeObserver,
                (const std::shared_ptr<ReaderObserver>),
                (override));

    MOCK_METHOD(void,
                clearObservers,
                (),
                (override));

    MOCK_METHOD(int,
                countObservers,
                (),
                (const, override));

    MOCK_METHOD(void,
                startSeDetection,
                (PollingMode pollingMode),
                (override));

    MOCK_METHOD(void,
                stopSeDetection,
                (),
                (override));

    MOCK_METHOD(bool,
                isSePresent,
                (),
                (override));

    MOCK_METHOD(void,
                addSeProtocolSetting,
                (std::shared_ptr<SeProtocol>, const std::string&),
                (override));

    MOCK_METHOD(void,
                setSeProtocolSetting,
                ((const std::map<std::shared_ptr<SeProtocol>, std::string>&)),
                (override));

    MOCK_METHOD(void,
                setDefaultSelectionRequest,
                (std::shared_ptr<AbstractDefaultSelectionsRequest>,
                 const NotificationMode&,
                 PollingMode),
                (override));

    MOCK_METHOD(const TransmissionMode&,
                getTransmissionMode,
                (),
                (const, override));

    MOCK_METHOD(void,
                setDefaultSelectionRequest,
                (std::shared_ptr<AbstractDefaultSelectionsRequest>, const NotificationMode&),
                (override));

    MOCK_METHOD(void,
                finalizeSeProcessing,
                (),
                (override));
};

TEST(ObservableReaderTest, ObservableReader)
{
    ObservableReaderMock reader;
}



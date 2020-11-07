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

#include "ObservablePlugin.h"

using namespace testing;

using namespace keyple::core::seproxy::event;

class ObservablePluginMock : public ObservablePlugin {
public:
    MOCK_METHOD(void,
                addObserver,
                (const std::shared_ptr<PluginObserver>),
                (override));

    MOCK_METHOD(void,
                removeObserver,
                (const std::shared_ptr<PluginObserver>),
                (override));

    MOCK_METHOD(void,
                clearObservers,
                (),
                (override));

    MOCK_METHOD(int,
                countObservers,
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

    MOCK_METHOD((const std::set<std::string>),
                getReaderNames,
                (),
                (const, override));

    MOCK_METHOD((ConcurrentMap<const std::string, std::shared_ptr<SeReader>>&),
                getReaders,
                (),
                (override));

    MOCK_METHOD((const std::shared_ptr<SeReader>),
                getReader,
                (const std::string&),
                (override));

    MOCK_METHOD((const std::string&),
                getName,
                (),
                (const, override));
};

TEST(ObservablePluginTest, ObservablePlugin)
{
    ObservablePluginMock plugin;
}

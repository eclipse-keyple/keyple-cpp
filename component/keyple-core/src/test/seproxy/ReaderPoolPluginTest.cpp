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

#include "ReaderPoolPlugin.h"

using namespace testing;

using namespace keyple::core::seproxy;

class RPP_ReaderPoolPluginMock : public ReaderPoolPlugin {
public:
    MOCK_METHOD((std::set<std::string>), getReaderGroupReferences, (),
                (override));

    MOCK_METHOD((std::shared_ptr<SeReader>), allocateReader,
                (const std::string& groupReference), (override));

    MOCK_METHOD(void, releaseReader, (std::shared_ptr<SeReader> seReader),
                (override));
};

TEST(ReaderPoolPluginTest, ReaderPoolPlugin)
{
    RPP_ReaderPoolPluginMock plugin;
}

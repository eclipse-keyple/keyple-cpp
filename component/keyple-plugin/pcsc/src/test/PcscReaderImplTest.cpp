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

#include "PcscReaderImpl.h"

#include "IllegalArgumentException.h"

using namespace testing;

using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::plugin::pcsc;

TEST(PcscReaderImplTest, PcscReaderImpl)
{
    const std::vector<std::string>& list = PcscTerminal::listTerminals();

    PcscTerminal t(list[0]);

    PcscReaderImpl r("plugin", t);
}

TEST(PcscReaderImplTest, setParameter)
{
    const std::vector<std::string>& list = PcscTerminal::listTerminals();

    PcscTerminal t(list[0]);

    PcscReaderImpl r("plugin", t);

    r.setParameter("protocol", "T0");
}

TEST(PcscReaderImplTest, setParameter_badMde)
{
    const std::vector<std::string>& list = PcscTerminal::listTerminals();

    PcscTerminal t(list[0]);

    PcscReaderImpl r("plugin", t);

    EXPECT_THROW(r.setParameter("transmission_mode", "xxx"), IllegalArgumentException);
}

TEST(PcscReaderImplTest, getParameters)
{
    const std::vector<std::string>& list = PcscTerminal::listTerminals();

    PcscTerminal t(list[0]);

    PcscReaderImpl r("plugin", t);

    r.setParameter(PcscReader::SETTING_KEY_PROTOCOL,
                   PcscReader::SETTING_PROTOCOL_T0);

    const std::map<const std::string, const std::string>& map1 =
        r.getParameters();

    ASSERT_EQ(static_cast<int>(map1.size()), 1);
    ASSERT_EQ(map1.find(PcscReader::SETTING_KEY_PROTOCOL)->second, "T=0");


    r.setParameter(PcscReader::SETTING_KEY_MODE,
                   PcscReader::SETTING_MODE_SHARED);

    const std::map<const std::string, const std::string>& map2 =
        r.getParameters();

    ASSERT_EQ(static_cast<int>(map2.size()), 2);
    ASSERT_EQ(map2.find(PcscReader::SETTING_KEY_MODE)->second,
              PcscReader::SETTING_MODE_SHARED);
}

TEST(PcscReaderImplTest, getTransmissionMode)
{
    const std::vector<std::string>& list = PcscTerminal::listTerminals();

    PcscTerminal t(list[0]);

    PcscReaderImpl r("plugin", t);

    ASSERT_EQ(r.getTransmissionMode(), TransmissionMode::CONTACTS);

    r.setParameter(PcscReader::SETTING_KEY_PROTOCOL,
                   PcscReader::SETTING_PROTOCOL_T1);

    ASSERT_EQ(r.getTransmissionMode(), TransmissionMode::CONTACTLESS);
}

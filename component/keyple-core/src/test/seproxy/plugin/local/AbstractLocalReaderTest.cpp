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

#include "AbstractLocalReader.h"

#include "SeCommonProtocols.h"

using namespace testing;

using namespace keyple::core::seproxy::plugin::local;

class ALR_AbstractLocalReaderMock : public AbstractLocalReader {
public:
    ALR_AbstractLocalReaderMock(
      const std::string& pluginName, const std::string& readerName)
    : AbstractLocalReader(pluginName, readerName)
    {
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
};

TEST(AbstractLocalReaderTest, AbstractLocalReader)
{
    ALR_AbstractLocalReaderMock reader("pluginName", "readerName");
}

TEST(AbstractLocalReaderTest, isSePresent)
{
    ALR_AbstractLocalReaderMock reader("pluginName", "readerName");

    EXPECT_CALL(reader, checkSePresence())
        .Times(1)
        .WillOnce(Return(true));

    ASSERT_TRUE(reader.isSePresent());
}

TEST(AbstractLocalReaderTest, isLogicalChannelOpen)
{
    ALR_AbstractLocalReaderMock reader("pluginName", "readerName");

    ASSERT_FALSE(reader.isLogicalChannelOpen());
}

TEST(AbstractLocalReaderTest, addSeProtocolSetting)
{
    ALR_AbstractLocalReaderMock reader("pluginName", "readerName");

    reader.addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO14443_4, "rule");
}

TEST(AbstractLocalReaderTest, setSeProtocolSetting)
{
    ALR_AbstractLocalReaderMock reader("pluginName", "readerName");

    const std::map<std::shared_ptr<SeProtocol>, std::string> setting;

    reader.setSeProtocolSetting(setting);
}

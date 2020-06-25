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

#include "AbstractLocalReader.h"

#include "SeCommonProtocols.h"

using namespace testing;

using namespace keyple::core::seproxy::plugin::local;

class LocalReaderMock : public AbstractLocalReader {
public:
    LocalReaderMock(
      const std::string& pluginName, const std::string& readerName)
    : AbstractLocalReader(pluginName, readerName)
    {
    }

    const std::map<const std::string, const std::string> getParameters() const
        override
    {
        return parameters;
    }

    void setParameter(const std::string& key, const std::string& value) override
    {
        parameters.insert(
            std::pair<const std::string, const std::string>(key, value));
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

private:
    std::map<const std::string, const std::string> parameters;

    const TransmissionMode transmissionMode = TransmissionMode::CONTACTLESS;

    const std::vector<uint8_t> atr = {0x11, 0x22, 0x33, 0x44, 0x55};
};

TEST(AbstractLocalReaderTest, AbstractLocalReader)
{
    LocalReaderMock reader("pluginName", "readerName");
}

TEST(AbstractLocalReaderTest, isSePresent)
{
    LocalReaderMock reader("pluginName", "readerName");

    ASSERT_TRUE(reader.isSePresent());
}

TEST(AbstractLocalReaderTest, isLogicalChannelOpen)
{
    LocalReaderMock reader("pluginName", "readerName");

    ASSERT_FALSE(reader.isLogicalChannelOpen());
}

TEST(AbstractLocalReaderTest, addSeProtocolSetting)
{
    LocalReaderMock reader("pluginName", "readerName");

    reader.addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO14443_4, "rule");
}

TEST(AbstractLocalReaderTest, setSeProtocolSetting)
{
    LocalReaderMock reader("pluginName", "readerName");

    const std::map<std::shared_ptr<SeProtocol>, std::string> setting;

    reader.setSeProtocolSetting(setting);
}

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

#include "SeProtocol.h"

#include "TransmissionMode.h"

using namespace keyple::core::seproxy::protocol;

using namespace testing;

class SeProtocolMock : public SeProtocol {
public:
    SeProtocolMock(const std::string name)
    : SeProtocol(), mode(TransmissionMode::CONTACTLESS), name(name)
    {
    }

    const TransmissionMode& getTransmissionMode() const
    {
        return mode;
    }

    const std::string& getName() const
    {
        return name;
    }

private:
    const TransmissionMode mode;
    const std::string name;
};

TEST(SeProtocolTest, SeProtocol)
{
    SeProtocolMock protocol("protocol");
}

TEST(SeProtocolTest, getName)
{
    SeProtocolMock protocol("protocol");

    ASSERT_EQ(protocol.getName(), "protocol");
}

TEST(SeProtocolTest, getTransmissionMode)
{
    SeProtocolMock protocol("protocol");

    ASSERT_EQ(protocol.getTransmissionMode(), TransmissionMode::CONTACTLESS);
}

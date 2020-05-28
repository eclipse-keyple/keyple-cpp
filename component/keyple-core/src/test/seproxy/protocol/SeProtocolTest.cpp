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
    SeProtocolMock() : SeProtocol(), mode(TransmissionMode::CONTACTLESS)
    {
    }

    SeProtocolMock(const SeProtocolMock& o)
    : SeProtocol(o), mode(TransmissionMode::CONTACTLESS)
    {
    }

    TransmissionMode getTransmissionMode() const
    {
        return mode;
    }

private:
    const TransmissionMode mode;
};

TEST(SeProtocolTest, SeProtocol)
{
    SeProtocolMock protocol1;
    SeProtocolMock protocol2(protocol1);
}

TEST(SeProtocolTest, getName)
{
    SeProtocolMock protocol1;

    ASSERT_EQ(protocol1.getName(), "");

    SeProtocolMock protocol2(protocol1);

    ASSERT_EQ(protocol2.getName(), "");
}

TEST(SeProtocolTest, getTransmissionMode)
{
    SeProtocolMock protocol1;

    ASSERT_EQ(protocol1.getTransmissionMode(), TransmissionMode::CONTACTLESS);

    SeProtocolMock protocol2(protocol1);

    ASSERT_EQ(protocol2.getTransmissionMode(), TransmissionMode::CONTACTLESS);
}

TEST(SeProtocolTest, operators)
{
    SeProtocolMock protocol1;
    SeProtocolMock protocol2(protocol1);
    SeProtocolMock protocol3;

    ASSERT_TRUE(protocol1 == protocol2);
    ASSERT_TRUE(protocol1 != protocol3);

    ASSERT_FALSE(protocol1 != protocol2);
    ASSERT_FALSE(protocol1 == protocol3);

    ASSERT_TRUE(protocol1 < protocol3);
    ASSERT_TRUE(protocol2 < protocol3);

    ASSERT_FALSE(protocol1 < protocol2);
    ASSERT_FALSE(protocol2 < protocol1);
    ASSERT_FALSE(protocol3 < protocol2);
    ASSERT_FALSE(protocol3 < protocol1);
}
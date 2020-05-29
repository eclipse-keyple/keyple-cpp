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

#include "SeCommonProtocols.h"

#include "TransmissionMode.h"

using namespace testing;

using namespace keyple::core::seproxy::protocol;

TEST(SeCommonProtocols, SeCommonProtocols)
{
    SeCommonProtocols protocol1("name", TransmissionMode::CONTACTLESS);

    SeCommonProtocols protocol2(protocol1);

    ASSERT_EQ(protocol1.values.size(), 15);
    ASSERT_EQ(protocol2.values.size(), 15);
}

TEST(SeCommonProtocols, getName)
{
    SeCommonProtocols protocol1("name", TransmissionMode::CONTACTLESS);

    SeCommonProtocols protocol2(protocol1);

    ASSERT_EQ(protocol1.getName(), "name");
    ASSERT_EQ(protocol2.getName(), "name");
}

TEST(SeCommonProtocols, getTransmissionMode)
{
    SeCommonProtocols protocol1("name", TransmissionMode::CONTACTLESS);
    SeCommonProtocols protocol2(protocol1);
    SeCommonProtocols protocol3("name", TransmissionMode::CONTACTS);

    ASSERT_EQ(protocol1.getTransmissionMode(), TransmissionMode::CONTACTLESS);
    ASSERT_EQ(protocol2.getTransmissionMode(), TransmissionMode::CONTACTLESS);
    ASSERT_EQ(protocol3.getTransmissionMode(), TransmissionMode::CONTACTS);
}

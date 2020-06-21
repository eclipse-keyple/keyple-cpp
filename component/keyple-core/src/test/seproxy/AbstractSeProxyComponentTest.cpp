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

#include "AbstractSeProxyComponent.h"

using namespace testing;

using namespace keyple::core::seproxy;

class ASPC_AbstractSeProxyComponentMock : public AbstractSeProxyComponent {
public:
    ASPC_AbstractSeProxyComponentMock(const std::string& name)
    : AbstractSeProxyComponent(name) {}

    MOCK_METHOD((const std::map<const std::string, const std::string>),
                getParameters, (), (const, override));

    MOCK_METHOD(void, setParameter,
                (const std::string& key, const std::string& value), (override));
};

TEST(AbstractSeProxyComponentTest, AbstractSeProxyComponent)
{
    ASPC_AbstractSeProxyComponentMock proxy("proxy");
}

TEST(AbstractSeProxyComponentTest, getName)
{
    ASPC_AbstractSeProxyComponentMock proxy("proxy");

    ASSERT_EQ(proxy.getName(), "proxy");
}

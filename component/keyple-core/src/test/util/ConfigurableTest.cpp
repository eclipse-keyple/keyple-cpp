
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

#include "ConfigurableTest.h"

using namespace keyple::core::util;

namespace keyple {
namespace core {
namespace util {

using namespace testing;

void ConfigurableTest::sample(std::string szKey, std::string szValue)
{
    //std::shared_ptr<Configurable> pLocal = std::make_shared<Configurable>();
    //pLocal->setParameter( szKey, szValue );
    szKey   = "S";
    szValue = "Z";
}
}
}
}

TEST(ConfigurableTest, testA)
{
    std::shared_ptr<ConfigurableTest> LocalTest =
        std::make_shared<ConfigurableTest>();
    //std::shared_ptr<Configurable> Local = std::make_shared<Configurable>();
    int local = 4;

    LocalTest->sample("TEST", "yes");
    LocalTest->sample("ON", "1");
    LocalTest->sample("OFF", "0");

    ASSERT_NE(local, 0);
    ASSERT_EQ(local, 4);
}

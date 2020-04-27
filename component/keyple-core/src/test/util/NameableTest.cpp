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

#include "NameableTest.h"

using namespace keyple::core::util;

namespace keyple {
namespace core {
namespace util {

using namespace testing;

int NameableTest::sample()
{
    //std::shared_ptr<NameableTest> pLocal =
    //    std::make_shared<NameableTest>("Testing");
    //std::string szName = pLocal->getName();
    std::string szName = "TEST";

    return szName.length();
}
}
}
}

TEST(NameableTest, testA)
{
    //std::shared_ptr<NameableTest> LocalTest = std::make_shared<NameableTest>();
    int local = 4; //LocalTest->sample();

    ASSERT_NE(local, 0);
    ASSERT_EQ(local, 4);
}

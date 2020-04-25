
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

#include "CountDownLatchTest.h"

using namespace keyple::core::util;

namespace keyple {
namespace core {
namespace util {

using namespace testing;

int CountDownLatchTest::sample(unsigned int iValue)
{
    int iResult;

    std::shared_ptr<CountDownLatch> pLocal =
        std::make_shared<CountDownLatch>(iValue);
    pLocal->countDown();
    iResult = pLocal->getCount();

    return iResult;
}
}
}
}

TEST(CountDownLatchTest, testA)
{
    std::shared_ptr<CountDownLatchTest> LocalTest =
        std::make_shared<CountDownLatchTest>();
    int local = LocalTest->sample(5);

    ASSERT_NE(local, 5);
    ASSERT_EQ(local, 4);
}

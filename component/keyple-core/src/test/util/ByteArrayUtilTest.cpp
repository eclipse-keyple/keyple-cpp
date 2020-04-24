
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

#include "ByteArrayUtilTest.h"

using namespace keyple::core::util;

namespace keyple {
namespace core {
namespace util {

using namespace testing;

int ByteArrayUtilTest::sample()
{
    std::vector<char> local = {'A', 'B', 'C', 'D'};
    std::string szIn        = "012345";
    int iOffset             = 4;

    ByteArrayUtil pLocal = ByteArrayUtil();
    local                = pLocal.fromHex(szIn);

    return iOffset;
}
}
}
}

TEST(ByteArrayUtilTest, testA)
{
    std::shared_ptr<ByteArrayUtilTest> LocalTest =
        std::make_shared<ByteArrayUtilTest>();
    int local = LocalTest->sample();

    ASSERT_NE(local, 0);
    ASSERT_EQ(local, 4);
}

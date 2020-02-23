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

#include "SendableInSessionTest.h"

using namespace keyple::calypso::command;

namespace keyple {
namespace calypso {
namespace command {
using namespace testing;

void SendableInSessionTest::sample()
{
    //std::shared_ptr<SendableInSession> pLocal =
    //    std::make_shared<SendableInSession>();
}
}
}
}

TEST(SendableInSessionTest, testA)
{
    std::shared_ptr<SendableInSessionTest> LocalTest =
        std::make_shared<SendableInSessionTest>();
    LocalTest->sample();
}

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

#include "AbstractPoCommandBuilderTest.h"
#include "AbstractPoCommandBuilder.h"

using namespace keyple::calypso::command::po;

namespace keyple {
namespace calypso {
namespace command {
namespace po {

void AbstractPoCommandBuilderTest::sample()
{
    //std::shared_ptr<AbstractPoCommandBuilder<AbstractIso7816CommandBuilder>>
    //    pLocal = std::make_shared<AbstractPoCommandBuilder<
    //                                  AbstractIso7816CommandBuilder>>();
    //std::string local = pLocal->getName();

    //ASSERT_NE( local, "");
}
}
}
}
}

TEST(AbstractPoCommandBuilderTest, testA)
{
    std::shared_ptr<AbstractPoCommandBuilderTest> LocalTest =
        std::make_shared<AbstractPoCommandBuilderTest>();
    LocalTest->sample();
}

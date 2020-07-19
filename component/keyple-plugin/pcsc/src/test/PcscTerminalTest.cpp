/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include "PcscTerminal.h"

using namespace testing;

using namespace keyple::plugin::pcsc;

TEST(PcscTerminalTest, PcscTerminal)
{
    PcscTerminal t("terminal");
}

TEST(PcscTerminalTest, listTerminals)
{
    const std::vector<std::string>& list = PcscTerminal::listTerminals();

    std::cout << "             " << list.size() << " terminal available:"
              << std::endl;

    for (const auto &r : list)
        std::cout << "              - " << r << std::endl;

    ASSERT_NE(static_cast<int>(list.size()), 0);
}

TEST(PcscTerminalTest, getName)
{
    PcscTerminal t("terminal");

    ASSERT_EQ(t.getName(), "terminal");
}

TEST(PcscTerminalTest, isCardPresent)
{
    const std::vector<std::string>& list = PcscTerminal::listTerminals();

    PcscTerminal t(list[0]);

    ASSERT_FALSE(t.isCardPresent(false));
}

TEST(PcscTerminalTest, waitForCardPresent)
{
    const std::vector<std::string>& list = PcscTerminal::listTerminals();

    PcscTerminal t(list[0]);

    ASSERT_FALSE(t.waitForCardPresent(100));
}

TEST(PcscTerminalTest, waitForCardAbsent)
{
    const std::vector<std::string>& list = PcscTerminal::listTerminals();

    PcscTerminal t(list[0]);

    ASSERT_TRUE(t.waitForCardAbsent(100));
}


/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License version 2.0 which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 */

#include "SmartCardIOReaderTest.hpp"

TEST_F(SmartCardIOReaderTest, assertEqual)
{
    ASSERT_NE(reader, nullptr);
}

TEST_F(SmartCardIOReaderTest, testIsSEPresent) // throws CardException, NoStackTraceThrowable
{
    // this.reader = new PcscReader(terminal, readerName);
    while(!terminal->isCardPresent());
    ASSERT_TRUE(reader->isSePresent());
    while(terminal->isCardPresent());
    ASSERT_FALSE(reader->isSePresent());
}
/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#include "gtest/gtest.h"

#include "Logger.h"

using namespace testing;

using namespace keyple::common;

int main(int argc, char **argv)
{
    /* Initialize GTest */
    ::testing::InitGoogleTest(&argc, argv);

    Logger::setLoggerLevel(Logger::Level::logError);

    /* Run */
    return RUN_ALL_TESTS();
}

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

#pragma once

#include <stdexcept>
#include <memory>
#include <typeinfo>
#include <chrono>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

/* Common */
#include "LoggerFactory.h"

#include "StubPluginImpl.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace keyple { namespace plugin { namespace stub { class StubPlugin; } } }
namespace keyple { namespace plugin { namespace stub { class StubReader; } } }
namespace keyple { namespace seproxy { namespace exception {
    class KeypleReaderException; } } }
namespace keyple { namespace seproxy { namespace exception {
    class KeyplePluginException; } } }
namespace testing { namespace gtest {} }

namespace keyple {
namespace plugin {
namespace stub {

using Logger                = keyple::common::Logger;
using LoggerFactory         = keyple::common::LoggerFactory;
//using namespace ::testing;

class BaseStubTest : public std::enable_shared_from_this<BaseStubTest>
//, public testing::Test
{
public:
    /**
     *
     */
    StubPluginImpl* stubPlugin;

    /**
     *
     */
    static const std::string PLUGIN_NAME;

private:
    static const std::shared_ptr<Logger> logger;

public:
    std::shared_ptr<BaseStubTest> name = std::make_shared<BaseStubTest>();

    virtual void setUp();

    virtual void tearDown();

    void TestBoby();
};
}
}
}

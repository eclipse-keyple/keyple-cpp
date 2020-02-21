#pragma once

#include <memory>

/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/
namespace org {
namespace eclipse {
namespace keyple {

using org::junit::rules::TestName;

class CoreBaseTest : public std::enable_shared_from_this<CoreBaseTest> {

public:
    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Rule public org.junit.rules.TestName name = new org.junit.rules.TestName();
    std::shared_ptr<TestName> name = std::make_shared<TestName>();
};

}
}
}

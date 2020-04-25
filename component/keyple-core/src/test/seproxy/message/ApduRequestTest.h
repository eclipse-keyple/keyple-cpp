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

#include <string>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <memory>

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {
class ApduRequest;
}
}
}
}
}

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {

class ApduRequestTest : public std::enable_shared_from_this<ApduRequestTest> {
public:
    virtual void setUp() throw(std::runtime_error);

    virtual void testSimpleAPDURequest();

    virtual void testAPDURequest();

    /*
     * HELPERS
     */

    static std::shared_ptr<ApduRequest> getApduSample();

    static std::vector<char> getACommand();

    static std::shared_ptr<std::set<Integer>> getASuccessFulStatusCode();

    static std::string getAName();
};

}
}
}
}
}

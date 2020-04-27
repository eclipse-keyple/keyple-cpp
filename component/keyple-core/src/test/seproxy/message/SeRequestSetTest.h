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

#include <unordered_set>
#include <stdexcept>
#include <memory>

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {
class SeRequestSet;
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
class SeRequest;
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

class SeRequestSetTest : public std::enable_shared_from_this<SeRequestSetTest> {

    // object to test
public:
    std::shared_ptr<SeRequestSet> seRequestSet;

    // attributes
    std::shared_ptr<std::set<std::shared_ptr<SeRequest>>> sortedRequests;

    std::shared_ptr<SeRequest> firstRequest;

    virtual void setUp() throw(std::runtime_error);

    virtual void getRequests();

    virtual void getSingleRequest();

    virtual void getSingleRequestFail();

    virtual void toStringNull();
};

}
}
}
}
}

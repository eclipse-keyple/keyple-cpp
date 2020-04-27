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

#include <vector>
#include <stdexcept>
#include <memory>

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {

class SeResponseSetTest
: public std::enable_shared_from_this<SeResponseSetTest> {
public:
    virtual void getSingleResponse() throw(std::runtime_error);

    virtual void getSingleResponseFail() throw(std::runtime_error);

    virtual void getResponses() throw(std::runtime_error);

    virtual void toStringTest() throw(std::runtime_error);
};

}
}
}
}
}

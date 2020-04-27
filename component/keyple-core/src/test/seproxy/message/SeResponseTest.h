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

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {
class SeResponse;
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

class SeResponseTest : public std::enable_shared_from_this<SeResponseTest> {
public:
    virtual void
    constructorSuccessfullResponseMatch();

    virtual void
    constructorSuccessfullResponseNoMatch();

    virtual void constructorATRNull();

    virtual void constructorFCINull();

    virtual void constructorFCIAndATRNull();

    virtual void testEquals();

    virtual void testThisEquals();

    virtual void testNotEquals();

    virtual void testNotEqualsNull();

    virtual void hashcode();

    virtual void hashcodeNull();

    /*
     * HELPERS
     */

    static std::shared_ptr<SeResponse>
    getASeResponse();
};

}
}
}
}
}

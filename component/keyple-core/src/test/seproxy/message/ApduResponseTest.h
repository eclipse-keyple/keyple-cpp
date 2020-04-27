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
#include <vector>
#include <memory>

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {
class AnswerToReset;
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
class ApduResponse;
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

class ApduResponseTest : public std::enable_shared_from_this<ApduResponseTest> {
public:
    virtual void setUp();

    virtual void constructorSuccessFullResponse();

    virtual void constructorSuccessFullResponseWithCustomCode();

    virtual void constructorFailResponse();

    virtual void constructorFailResponseWithCustomCode();

    virtual void isEqualsTest();

    virtual void isThisEquals();

    virtual void isNotEquals();

    virtual void isNotEqualsNull();

    virtual void hashcodeTest();
    virtual void hashcodeNull();

    virtual void testToStringNull();

    /*
     * HELPERS
     */

    static std::shared_ptr<std::set<Integer>> getA9005CustomCode();

    static std::shared_ptr<AnswerToReset> getAAtr();

    static std::shared_ptr<ApduResponse> getAFCI();

    static std::shared_ptr<ApduResponse> getSuccessfullResponse();

    static std::vector<std::shared_ptr<ApduResponse>> getAListOfAPDUs();
};

}
}
}
}
}

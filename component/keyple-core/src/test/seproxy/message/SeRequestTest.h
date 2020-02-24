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

#include "ChannelState.h"
#include <vector>
#include <memory>

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
namespace protocol {
class SeProtocol;
}
}
}
}
}

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
class SeSelector;
}
}
}
}

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {

using ChannelState = keyple::core::seproxy::ChannelState;
using SeSelector   = keyple::core::seproxy::SeSelector;
using SeProtocol   = keyple::core::seproxy::protocol::SeProtocol;

class SeRequestTest : public std::enable_shared_from_this<SeRequestTest> {

    // object to test
public:
    std::shared_ptr<SeRequest> seRequest;

    virtual std::vector<std::shared_ptr<ApduRequest>> getApdus();

    // attributes
    std::vector<std::shared_ptr<ApduRequest>> apdus;
    ChannelState channelState = static_cast<ChannelState>(0);
    std::shared_ptr<SeProtocol> seProtocol;
    std::shared_ptr<std::set<Integer>> selectionStatusCode;
    std::shared_ptr<SeSelector> selector;

    virtual void setUp();

    virtual void testSERequest();

    virtual void getSelector();

    virtual void getApduRequests();

    virtual void isKeepChannelOpen();

    virtual void getProtocolFlag();

    virtual void getSuccessfulSelectionStatusCodes();

    virtual void toStringNull();

    /*
     * Constructors
     */
    virtual void constructor1();

    virtual void constructor2();

    virtual void constructor2b();
    virtual void constructor3();

    /*
     * HELPERS FOR OTHERS TESTS SUITE
     */

    static std::shared_ptr<SeRequest> getSeRequestSample();

    static std::vector<std::shared_ptr<ApduRequest>> getAapduLists();

    static std::shared_ptr<SeProtocol> getASeProtocol();

    static std::shared_ptr<SeSelector>
    getSelector(std::shared_ptr<std::set<Integer>> selectionStatusCode);
};

}
}
}
}
}

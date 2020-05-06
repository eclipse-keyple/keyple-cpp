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
#include <memory>

#include "ChannelControl.h"
#include "SeRequest.h"
#include "ApduRequest.h"
#include "SeSelector.h"
#include "ApduRequestTest.h"
#include "ByteArrayUtil.h"
#include "SeProtocol.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using ChannelControl = keyple::core::seproxy::ChannelControl;
using SeSelector   = keyple::core::seproxy::SeSelector;
using SeProtocol   = keyple::core::seproxy::protocol::SeProtocol;

class SeRequestTest : public std::enable_shared_from_this<SeRequestTest> {

    // object to test
public:
    std::shared_ptr<SeRequest> seRequest;

    virtual std::vector<std::shared_ptr<ApduRequest>> getApdus();

    // attributes
    std::vector<std::shared_ptr<ApduRequest>> apdus;
    ChannelControl channelState = static_cast<ChannelControl>(0);
    std::shared_ptr<SeProtocol> seProtocol;
    std::shared_ptr<std::set<int>> selectionStatusCode;
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
    getSelector(std::shared_ptr<std::set<int>> selectionStatusCode);
};

}
}
}
}

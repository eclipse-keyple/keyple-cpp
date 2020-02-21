#pragma once

#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include <vector>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
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
namespace seproxy {
namespace message {

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                    import static org.junit.Assert.*;
using ChannelState = keyple::core::seproxy::ChannelState;
using SeSelector   = keyple::core::seproxy::SeSelector;
using SeProtocol   = keyple::core::seproxy::protocol::SeProtocol;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("PMD.SignatureDeclareThrowsException") @RunWith(MockitoJUnitRunner.class) public class SeRequestTest
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

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Before public void setUp()
    virtual void setUp();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void testSERequest()
    virtual void testSERequest();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void getSelector()
    virtual void getSelector();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void getApduRequests()
    virtual void getApduRequests();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void isKeepChannelOpen()
    virtual void isKeepChannelOpen();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void getProtocolFlag()
    virtual void getProtocolFlag();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void getSuccessfulSelectionStatusCodes()
    virtual void getSuccessfulSelectionStatusCodes();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void toStringNull()
    virtual void toStringNull();

    /*
                         * Constructors
                         */
    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void constructor1()
    virtual void constructor1();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void constructor2()
    virtual void constructor2();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void constructor2b()
    virtual void constructor2b();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void constructor3()
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

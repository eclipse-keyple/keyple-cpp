#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <regex>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
class SeProxyService;
}
}
}
}
namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
class ReaderPlugin;
}
}
}
}
namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace exception {
class KeyplePluginNotFoundException;
}
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

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                import static org.junit.Assert.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                import static org.mockito.Mockito.when;
using KeyplePluginNotFoundException =
    org::eclipse::keyple::seproxy::exception::KeyplePluginNotFoundException;
using org::slf4j::Logger;
using org::slf4j::LoggerFactory;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("PMD.SignatureDeclareThrowsException") @RunWith(MockitoJUnitRunner.class) public class SeProxyServiceTest
class SeProxyServiceTest
: public std::enable_shared_from_this<SeProxyServiceTest> {

private:
    static const std::shared_ptr<Logger> logger;

    // class to test
public:
    std::shared_ptr<SeProxyService> proxyService;

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Mock ReaderPlugin plugin1;
    std::shared_ptr<ReaderPlugin> plugin1;

    static std::string PLUGIN_NAME;

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Before public void setupBeforeEach()
    virtual void setupBeforeEach();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void testGetInstance()
    virtual void testGetInstance();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void testGetVersion()
    virtual void testGetVersion();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void testGetSetPlugins()
    virtual void testGetSetPlugins();

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test public void testGetPlugin() throws org.eclipse.keyple.seproxy.exception.KeyplePluginNotFoundException
    virtual void testGetPlugin() throw(KeyplePluginNotFoundException);

    //JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
    //ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeyplePluginNotFoundException.class) public void testGetPluginFail() throws Exception
    virtual void testGetPluginFail() throw(std::runtime_error);

    /*
                     * HELPERS
                     */

private:
    std::shared_ptr<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>
    getPluginList();
};

}
}
}
}

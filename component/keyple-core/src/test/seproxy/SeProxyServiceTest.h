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
#include <iostream>
#include <stdexcept>
#include <regex>
#include <memory>


namespace keyple {
namespace core {
namespace seproxy {
class SeProxyService;
}
}
}


namespace keyple {
namespace core {
namespace seproxy {
class ReaderPlugin;
}
}
}


namespace keyple {
namespace core {
namespace seproxy {
namespace exception {
class KeyplePluginNotFoundException;
}
}
}
}



namespace keyple {
namespace core {
namespace seproxy {

using KeyplePluginNotFoundException =
    keyple::core::seproxy::exception::KeyplePluginNotFoundException;

class SeProxyServiceTest
: public std::enable_shared_from_this<SeProxyServiceTest> {

private:
    static const std::shared_ptr<Logger> logger;

    // class to test
public:
    std::shared_ptr<SeProxyService> proxyService;

    std::shared_ptr<ReaderPlugin> plugin1;

    static std::string PLUGIN_NAME;

    virtual void setupBeforeEach();

    virtual void testGetInstance();

    virtual void testGetVersion();

    virtual void testGetSetPlugins();

    virtual void testGetPlugin() throw(KeyplePluginNotFoundException);

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

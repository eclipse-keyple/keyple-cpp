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
#include <memory>
#include <mutex>
#include <set>

#include "exceptionhelper.h"

/* Common */
#include "Export.h"
#include "LoggerFactory.h"
#include "Object.h"

/* Core */
#include "AbstractPluginFactory.h"
#include "KeyplePluginNotFoundException.h"

/* Forward class declarations */
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

using namespace keyple::core::seproxy::exception;

/**
 * The Class SeProxyService. This singleton is the entry point of the SE Proxy
 * Service, its instance has to be called by a ticketing application in order to
 * establish a link with a SE’s application.
 *
 */
class EXPORT SeProxyService final
: public std::enable_shared_from_this<SeProxyService> {
public:
    /**
     * Gets the single instance of SeProxyService.
     *
     * @return single instance of SeProxyService
     */
    static SeProxyService& getInstance()
    {
        static SeProxyService uniqueInstance;
        return uniqueInstance;
    }

    /**
     * Register a new plugin to be available in the platform if not registered
     * yet
     *
     * @param pluginFactory : plugin factory to instantiate plugin to be added
     * @throws KeyplePluginInstantiationException if instantiation failed
     */
    void registerPlugin(AbstractPluginFactory* pluginFactory);

    /**
     * Unregister plugin from platform
     *
     * @param pluginName : plugin name
     * @return true if the plugin was successfully unregistered
     */
    bool unregisterPlugin(const std::string& pluginName);

    /**
     * Check weither a plugin is already registered to the platform or not
     *
     * @param pluginName : name of the plugin to be checked
     * @return true if a plugin with matching name has been registered
     */
    bool isRegistered(const std::string& pluginName);

    /**
     * Gets the plugins.
     *
     * @return the plugins the list of interfaced reader’s plugins.
     */
    std::set<ReaderPlugin*>& getPlugins();

    /**
     * Gets the plugin whose name is provided as an argument.
     *
     * @param name the plugin name
     * @return the plugin
     * @throws KeyplePluginNotFoundException if the wanted plugin is not found
     */
    ReaderPlugin* getPlugin(const std::string& name);

    /**
     * Gets the version API, (the version of the sdk).
     *
     * @return the version
     */
    std::string getVersion();

private:
    /**
     * The list of readers’ plugins interfaced with the SE Proxy Service
     */
    std::set<ReaderPlugin*> plugins;

    /**
     * Instantiates a new SeProxyService.
     */
    SeProxyService();

    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(SeProxyService));

    /**
     * This is the object we will be synchronizing on ("the monitor")
     */
    std::mutex MONITOR;
};

}
}
}

/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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
#include <map>

/* Common */
#include "LoggerFactory.h"

/* Core */
#include "PluginFactory.h"
#include "KeypleCoreExport.h"
#include "KeyplePluginNotFoundException.h"

namespace keyple {
namespace core {
namespace seproxy {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;

/**
 * The Class SeProxyService. This singleton is the entry point of the SE Proxy
 * Service, its instance has to be called by a ticketing application in order to
 * establish a link with a SE’s application.
 *
 */
class KEYPLECORE_API SeProxyService final
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
     * @throw KeyplePluginInstantiationException if instantiation failed
     * @return ReaderPlugin : registered reader plugin
     */
    std::shared_ptr<ReaderPlugin> registerPlugin(
        std::shared_ptr<PluginFactory> pluginFactory);

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
     * @return the plugin names and plugin instances map of interfaced reader’s
     *         plugins.
     */
    const std::map<const std::string, std::shared_ptr<ReaderPlugin>>&
        getPlugins() const;

    /**
     * Gets the plugin whose name is provided as an argument.
     *
     * @param name the plugin name
     * @return the plugin
     * @throw KeyplePluginNotFoundException if the wanted plugin is not found
     */
    std::shared_ptr<ReaderPlugin> getPlugin(const std::string& name);

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
    std::map<const std::string, std::shared_ptr<ReaderPlugin>> mPlugins;

    /**
     * Instantiates a new SeProxyService.
     */
    SeProxyService();

    /**
     *
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(SeProxyService));

    /**
     * This is the object we will be synchronizing on ("the monitor")
     */
    std::mutex MONITOR;
};

}
}
}

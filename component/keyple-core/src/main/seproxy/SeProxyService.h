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

#pragma once

#include <string>
#include <memory>
#include <set>

#include "exceptionhelper.h"

/* Common */
#include "Export.h"

/* Core */
#include "KeyplePluginNotFoundException.h"

/* Forward class declarations */
namespace keyple { namespace core { namespace seproxy { class ReaderPlugin; } } }
namespace keyple { namespace core { namespace seproxy { namespace exception { class KeyplePluginNotFoundException; } } } }

namespace keyple {
namespace core {
namespace seproxy {

using namespace keyple::core::seproxy::exception;

/**
 * The Class SeProxyService. This singleton is the entry point of the SE Proxy Service, its instance
 * has to be called by a ticketing application in order to establish a link with a SE’s application.
 *
 */
class EXPORT SeProxyService final : public std::enable_shared_from_this<SeProxyService> {
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
     * Sets the plugins.
     *
     * @param plugins the new plugins
     */
    void setPlugins(std::set<std::shared_ptr<ReaderPlugin>>& plugins);

    /**
     * Adds a single plugin to the plugin list.
     *
     * @param plugin the plugin to add.
     */
    void addPlugin(std::shared_ptr<ReaderPlugin> plugin);

    /**
     * Gets the plugins.
     *
     * @return the plugins the list of interfaced reader’s plugins.
     */
    std::set<std::shared_ptr<ReaderPlugin>>& getPlugins();

    /**
     * Gets the plugin whose name is provided as an argument.
     *
     * @param name the plugin name
     * @return the plugin
     * @throws KeyplePluginNotFoundException if the wanted plugin is not found
     */
    std::shared_ptr<ReaderPlugin> getPlugin(const std::string &name);

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
    std::set<std::shared_ptr<ReaderPlugin>> plugins = std::set<std::shared_ptr<ReaderPlugin>>();

    /**
     * Instantiates a new SeProxyService.
     */
    SeProxyService();
};

}
}
}

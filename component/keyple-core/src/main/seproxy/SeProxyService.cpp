/********************************************************************************
* Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#include "SeProxyService.h"
#include "ReaderPlugin.h"
#include "KeyplePluginNotFoundException.h"

namespace keyple {
namespace core {
namespace seproxy {

using namespace keyple::core::seproxy::exception;

SeProxyService::SeProxyService()
{
}

void SeProxyService::setPlugins(std::set<std::shared_ptr<ReaderPlugin>> &plugins)
{
    this->plugins = plugins;
}

void SeProxyService::addPlugin(std::shared_ptr<ReaderPlugin> plugin)
{
    this->plugins.insert(plugin);
}

std::set<std::shared_ptr<ReaderPlugin>>& SeProxyService::getPlugins()
{
    return plugins;
}

std::shared_ptr<ReaderPlugin> SeProxyService::getPlugin(const std::string &name)
{
    for (auto plugin : plugins)
    {
        if (plugin->getName() == name)
        {
            return plugin;
        }
    }
    throw std::make_shared<KeyplePluginNotFoundException>(name);
}

std::string SeProxyService::getVersion()
{
    /*
        * Alex
        *
    try {
        // load keyple core property file
        std::shared_ptr<InputStream> propertiesIs = this->getClass().getClassLoader().getResourceAsStream("META-INF/keyple-core.properties");
        std::shared_ptr<Properties> prop = std::make_shared<Properties>();
        prop->load(propertiesIs);
        std::string version = prop->getProperty("version");
        if (version != "") {
            return version;
        }
    }
    catch (const IOException &e) {
        e->printStackTrace();
    }
    */

    return "no-version-found";
}

}
}
}

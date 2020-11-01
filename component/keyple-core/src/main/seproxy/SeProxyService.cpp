/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/


#include "SeProxyService.h"

/* Common */
#include "IllegalArgumentException.h"

/* Core */
#include "ReaderPlugin.h"
#include "KeyplePluginInstantiationException.h"
#include "KeyplePluginNotFoundException.h"

namespace keyple {
namespace core {
namespace seproxy {

using namespace keyple::core::seproxy::exception;

SeProxyService::SeProxyService() {}

std::shared_ptr<ReaderPlugin> SeProxyService::registerPlugin(
    std::shared_ptr<PluginFactory> pluginFactory)
{
    if (pluginFactory == nullptr)
        throw IllegalArgumentException("Factory must not be null");

    std::lock_guard<std::mutex> guard(MONITOR);

    const std::string& pluginName = pluginFactory->getPluginName();

    if (mPlugins.count(pluginName)) {
        mLogger->warn("Plugin has already been registered to the platform : %\n", pluginName);
        return mPlugins.find(pluginName)->second;
    } else {
        std::shared_ptr<ReaderPlugin> pluginInstance = pluginFactory->getPlugin();
        mLogger->info("Registering a new Plugin to the platform : %\n", pluginName);
        mPlugins.insert({pluginName, pluginInstance});
        return pluginInstance;
    }
}

bool SeProxyService::unregisterPlugin(const std::string& pluginName)
{
    std::lock_guard<std::mutex> guard(MONITOR);

    if (mPlugins.count(pluginName)) {
        mPlugins.erase(pluginName);
        mLogger->info("Unregistering a plugin from the platform : %\n", pluginName);
        return true;
    } else {
        mLogger->warn("Plugin is not registered to the platform : %\n", pluginName);
        return false;
    }
}

bool SeProxyService::isRegistered(const std::string& pluginName)
{
    std::lock_guard<std::mutex> guard(MONITOR);

    return mPlugins.count(pluginName) ? true : false;
}

const std::map<const std::string, std::shared_ptr<ReaderPlugin>>& SeProxyService::getPlugins() const
{
    return mPlugins;
}

std::shared_ptr<ReaderPlugin> SeProxyService::getPlugin(const std::string& name)
{
    std::lock_guard<std::mutex> guard(MONITOR);

    ConcurrentMap<const std::string, std::shared_ptr<ReaderPlugin>>
        ::const_iterator it;

    if ((it = mPlugins.find(name)) != mPlugins.end())
        return it->second;
    else
        throw KeyplePluginNotFoundException(name);
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

    return "0.8.2";
}

}
}
}

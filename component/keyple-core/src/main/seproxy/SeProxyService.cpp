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

#include "SeProxyService.h"
#include "ReaderPlugin.h"
#include "KeyplePluginInstantiationException.h"
#include "KeyplePluginNotFoundException.h"

namespace keyple {
namespace core {
namespace seproxy {

using namespace keyple::core::seproxy::exception;

SeProxyService::SeProxyService()
{
}

void SeProxyService::registerPlugin(AbstractPluginFactory* pluginFactory)
{
    if (pluginFactory == nullptr)
        throw IllegalArgumentException("Factory must not be null");

    if (!isRegistered(pluginFactory->getPluginName())) {
        std::lock_guard<std::mutex> guard(MONITOR);
        logger->info("Registering a new Plugin to the platform : %s\n",
                     pluginFactory->getPluginName().c_str());
        ReaderPlugin& newPlugin = pluginFactory->getPluginInstance();
        this->plugins.insert(&newPlugin);

    } else {
        logger->warn("Plugin has already been registered to the platform "
                     ": %s\n",
                     pluginFactory->getPluginName().c_str());
    }
}

bool SeProxyService::unregisterPlugin(const std::string& pluginName)
{
    bool ret = false;
    ReaderPlugin* readerPlugin = nullptr;

    try {

        std::lock_guard<std::mutex> guard(MONITOR);
        readerPlugin = this->getPlugin(pluginName);
        logger->info("Unregistering a plugin from the platform : %s\n",
                     readerPlugin->getName().c_str());
        ret = plugins.erase(readerPlugin);

    } catch (KeyplePluginNotFoundException& e) {
        logger->info("Plugin is not registered to the platform : %s. %s\n",
                     pluginName.c_str(), e.getMessage().c_str());
    }

    return ret;
}

bool SeProxyService::isRegistered(const std::string& pluginName)
{
    bool ret = false;

    std::lock_guard<std::mutex> guard(MONITOR);

    for (ReaderPlugin* registeredPlugin : plugins) {
        if (registeredPlugin->getName().compare(pluginName)) {
            ret = true;
            break;
        }
    }

    return ret;
}

std::set<ReaderPlugin*>& SeProxyService::getPlugins()
{
    return plugins;
}

ReaderPlugin* SeProxyService::getPlugin(const std::string& name)
{
    for (auto plugin : plugins) {
        if (plugin->getName() == name) {
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

#pragma once

#include <string>
#include "exceptionhelper.h"
#include <memory>
#include <set>

#include "KeyplePluginNotFoundException.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class ReaderPlugin; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeyplePluginNotFoundException; } } } } }

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

                using KeyplePluginNotFoundException = org::eclipse::keyple::seproxy::exception::KeyplePluginNotFoundException;

                /**
                 * The Class SeProxyService. This singleton is the entry point of the SE Proxy Service, its instance
                 * has to be called by a ticketing application in order to establish a link with a SE’s application.
                 *
                 */
                class SeProxyService final : public std::enable_shared_from_this<SeProxyService> {

                    /** singleton instance of SeProxyService */
                private:
                    static std::shared_ptr<SeProxyService> uniqueInstance;

                    /** the list of readers’ plugins interfaced with the SE Proxy Service */
                    std::shared_ptr<std::set<std::shared_ptr<ReaderPlugin>>> plugins = std::make_shared<std::set<std::shared_ptr<ReaderPlugin>>>();

                    /**
                     * Instantiates a new SeProxyService.
                     */
                    SeProxyService();

                    /**
                     * Gets the single instance of SeProxyService.
                     *
                     * @return single instance of SeProxyService
                     */
                public:
                    static std::shared_ptr<SeProxyService> getInstance();

                    /**
                     * Sets the plugins.
                     *
                     * @param plugins the new plugins
                     */
                    void setPlugins(std::shared_ptr<std::set<std::shared_ptr<ReaderPlugin>>> plugins);

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
                    std::shared_ptr<std::set<std::shared_ptr<ReaderPlugin>>> getPlugins();

                    /**
                     * Gets the plugin whose name is provided as an argument.
                     *
                     * @param name the plugin name
                     * @return the plugin
                     * @throws KeyplePluginNotFoundException if the wanted plugin is not found
                     */
                    std::shared_ptr<ReaderPlugin> getPlugin(const std::string &name) throw(KeyplePluginNotFoundException);

                    /**
                     * Gets the version API, (the version of the sdk).
                     *
                     * @return the version
                     */
                    std::string getVersion();
                };

            }
        }
    }
}

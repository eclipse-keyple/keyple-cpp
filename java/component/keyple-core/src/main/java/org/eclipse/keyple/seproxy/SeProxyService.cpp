#include "SeProxyService.h"
#include "ReaderPlugin.h"
#include "exception/KeyplePluginNotFoundException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                using KeyplePluginNotFoundException =
                    org::eclipse::keyple::seproxy::exception::KeyplePluginNotFoundException;

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
            } // namespace seproxy
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org

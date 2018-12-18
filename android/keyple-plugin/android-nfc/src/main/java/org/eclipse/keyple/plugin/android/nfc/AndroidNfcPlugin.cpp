#include "AndroidNfcPlugin.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractObservableReader.h"
#include "AndroidNfcReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace android {
                    namespace nfc {
                        using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                        using AbstractStaticPlugin = org::eclipse::keyple::seproxy::plugin::AbstractStaticPlugin;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> AndroidNfcPlugin::LOG = org::slf4j::LoggerFactory::getLogger(AndroidNfcPlugin::typeid);
const std::shared_ptr<AndroidNfcPlugin> AndroidNfcPlugin::uniqueInstance = std::make_shared<AndroidNfcPlugin>();
const std::string AndroidNfcPlugin::PLUGIN_NAME = "AndroidNFCPlugin";

                        AndroidNfcPlugin::AndroidNfcPlugin() : org::eclipse::keyple::seproxy::plugin::AbstractStaticPlugin("AndroidNFCPlugin") {
                        }

                        std::shared_ptr<AndroidNfcPlugin> AndroidNfcPlugin::getInstance() {
                            return uniqueInstance;
                        }

                        std::unordered_map<std::string, std::string> AndroidNfcPlugin::getParameters() {
                            LOG->warn("Android NFC Plugin does not support parameters, see AndroidNfcReader instead");
                            return parameters;
                        }

                        void AndroidNfcPlugin::setParameter(const std::string &key, const std::string &value) {
                            LOG->warn("Android NFC Plugin does not support parameters, see AndroidNfcReader instead");
                            parameters.emplace(key, value);
                        }

                        std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> AndroidNfcPlugin::getNativeReaders() {
                            // return the only one reader in a list
                            std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> readers = std::make_shared<ConcurrentSkipListSet<std::shared_ptr<AbstractObservableReader>>>();
                            readers->add(AndroidNfcReader::getInstance());
                            return readers;
                        }

                        std::shared_ptr<AbstractObservableReader> AndroidNfcPlugin::getNativeReader(const std::string &name) {
                            return readers->first();
                        }
                    }
                }
            }
        }
    }
}

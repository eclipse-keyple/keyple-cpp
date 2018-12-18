#include "AndroidOmapiPlugin.h"
#include "ISeServiceFactory.h"
#include "SeServiceFactoryImpl.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractObservableReader.h"
#include "AndroidOmapiReader.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace android {
                    namespace omapi {
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                        using AbstractStaticPlugin = org::eclipse::keyple::seproxy::plugin::AbstractStaticPlugin;
                        using org::simalliance::openmobileapi::Reader;
                        using org::simalliance::openmobileapi::SEService;
                        using android::util::Log;
const std::string AndroidOmapiPlugin::TAG = AndroidOmapiPlugin::typeid->getSimpleName();
const std::string AndroidOmapiPlugin::PLUGIN_NAME = "AndroidOmapiPlugin";
std::shared_ptr<AndroidOmapiPlugin> AndroidOmapiPlugin::uniqueInstance = nullptr;

                        std::shared_ptr<ISeServiceFactory> AndroidOmapiPlugin::getSeServiceFactory() {
                            return std::make_shared<SeServiceFactoryImpl>();
                        };

                        AndroidOmapiPlugin::AndroidOmapiPlugin() : org::eclipse::keyple::seproxy::plugin::AbstractStaticPlugin(PLUGIN_NAME) {
                            seServiceFactory = AndroidOmapiPlugin::getSeServiceFactory();
//JAVA TO C++ CONVERTER TODO TASK: You cannot use 'shared_from_this' in a constructor:
                            seService = seServiceFactory->connectToSe(shared_from_this());
                        }

                        std::shared_ptr<AndroidOmapiPlugin> AndroidOmapiPlugin::getInstance() {
                            if (uniqueInstance == nullptr) {
                                uniqueInstance = std::make_shared<AndroidOmapiPlugin>();
                            }
                            return uniqueInstance;


                        }

                        std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> AndroidOmapiPlugin::getNativeReaders() {

                            std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> readers = std::set<std::shared_ptr<AbstractObservableReader>>();

                            if (seService != nullptr && seService->isConnected()) {
                                std::vector<std::shared_ptr<Reader>> omapiReaders = seService->getReaders();

                                // no readers found in the environment, don't return any readers for keyple
                                if (omapiReaders.empty()) {
                                    Log::w(TAG, "No readers found");
                                    return readers; // empty list
                                }

                                // Build a keyple reader for each readers found by the OMAPI
                                for (auto omapiReader : omapiReaders) {
                                    Log::d(TAG, "Reader available name : " + omapiReader->getName());
                                    Log::d(TAG, "Reader available isSePresent : " + omapiReader->isSecureElementPresent());

                                    // http://seek-for-android.github.io/javadoc/V4.0.0/org/simalliance/openmobileapi/Reader.html
                                    std::shared_ptr<AbstractObservableReader> seReader = std::make_shared<AndroidOmapiReader>(PLUGIN_NAME, omapiReader, omapiReader->getName());
                                    readers->add(seReader);
                                }

                                return readers;

                            }
                            else {
                                Log::w(TAG, "OMAPI SeService is not connected yet");
                                return readers; // empty list
                            }

                        }

                        std::shared_ptr<AbstractObservableReader> AndroidOmapiPlugin::getNativeReader(const std::string &name) throw(KeypleReaderException) {
                            for (auto aReader : readers) {
                                if (aReader->getName() == name) {
                                    return aReader;
                                }
                            }
                            return nullptr;

                        }

                        void AndroidOmapiPlugin::serviceConnected(std::shared_ptr<SEService> seService) {

                            Log::i(TAG, "Retrieve available readers...");

                            // init readers
                            readers = getNativeReaders();
                        }

                        std::unordered_map<std::string, std::string> AndroidOmapiPlugin::getParameters() {
                            Log::w(TAG, "Android OMAPI Plugin does not support parameters, see OMAPINfcReader instead");
                            return parameters;
                        }

                        void AndroidOmapiPlugin::setParameter(const std::string &key, const std::string &value) {
                            Log::w(TAG, "Android OMAPI  Plugin does not support parameters, see OMAPINfcReader instead");
                            parameters.emplace(key, value);
                        }
                    }
                }
            }
        }
    }
}

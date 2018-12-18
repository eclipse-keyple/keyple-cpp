#include "StubPlugin.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractObservableReader.h"
#include "StubReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace stub {
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                    using AbstractThreadedObservablePlugin = org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;
const std::shared_ptr<StubPlugin> StubPlugin::uniqueInstance = std::make_shared<StubPlugin>();
const std::shared_ptr<org::slf4j::Logger> StubPlugin::logger = org::slf4j::LoggerFactory::getLogger(StubPlugin::typeid);
std::shared_ptr<java::util::SortedSet<std::string>> StubPlugin::nativeStubReadersNames = std::make_shared<java::util::concurrent::ConcurrentSkipListSet<std::string>>();

                    StubPlugin::StubPlugin() : org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin("StubPlugin") {

                        /*
                         * Monitoring is not handled by a lower layer (as in PC/SC), reduce the threading period to
                         * 50 ms to speed up responsiveness.
                         */
                        threadWaitTimeout = 50;
                    }

                    std::shared_ptr<StubPlugin> StubPlugin::getInstance() {
                        return uniqueInstance;
                    }

                    std::unordered_map<std::string, std::string> StubPlugin::getParameters() {
                        return parameters;
                    }

                    void StubPlugin::setParameter(const std::string &key, const std::string &value) {
                        parameters.emplace(key, value);
                    }

                    std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> StubPlugin::getNativeReaders() throw(KeypleReaderException) {
                        /* init Stub Readers list */
                        std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> nativeReaders = std::make_shared<ConcurrentSkipListSet<std::shared_ptr<AbstractObservableReader>>>();

                        /*
                         * parse the current readers list to create the ProxyReader(s) associated with new reader(s)
                         */
                        if (nativeStubReadersNames != nullptr && nativeStubReadersNames->size() > 0) {
                            for (auto name : nativeStubReadersNames) {
                                nativeReaders->add(std::make_shared<StubReader>(name));
                            }
                        }
                        return nativeReaders;
                    }

                    std::shared_ptr<AbstractObservableReader> StubPlugin::getNativeReader(const std::string &name) {
                        for (auto reader : readers) {
                            if (reader->getName() == name) {
                                return reader;
                            }
                        }
                        std::shared_ptr<AbstractObservableReader> reader = nullptr;
                        if (nativeStubReadersNames->contains(name)) {
                            reader = std::make_shared<StubReader>(name);
                        }
                        return reader;
                    }

                    void StubPlugin::plugStubReader(const std::string &name) {
                        if (!nativeStubReadersNames->contains(name)) {
                            logger->info("Plugging a new reader with name " + name);
                            /* add the native reader to the native readers list */
                            nativeStubReadersNames->add(name);
                            /* add the reader as a new reader to the readers list */
                            std::shared_ptr<StubReader> stubReader = std::make_shared<StubReader>(name);
                            readers->add(std::static_pointer_cast<AbstractObservableReader>(stubReader));
                        }
                        else {
                            logger->error("Reader with name " + name + " was already plugged");
                        }
                    }

                    void StubPlugin::unplugReader(const std::string &name) throw(KeypleReaderException) {

                        if (!nativeStubReadersNames->contains(name)) {
                            logger->warn("No reader found with name " + name);
                        }
                        else {
                            /* remove the reader from the readers list */
                            readers->remove(getNativeReader(name));
                            /* remove the native reader from the native readers list */
                            nativeStubReadersNames->remove(name);
                            logger->info("Unplugged reader with name " + name);
                        }
                    }

                    std::shared_ptr<SortedSet<std::string>> StubPlugin::getNativeReadersNames() {
                        if (nativeStubReadersNames->isEmpty()) {
                            logger->trace("No reader available.");
                        }
                        return nativeStubReadersNames;
                    }
                }
            }
        }
    }
}

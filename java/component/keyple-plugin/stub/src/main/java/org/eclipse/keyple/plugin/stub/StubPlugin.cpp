/* Plugin - Stub */
#include "StubPlugin.h"
#include "StubReader.h"

/* Core */
#include "KeypleReaderException.h"
#include "AbstractObservableReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace stub {
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                    using AbstractThreadedObservablePlugin = org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin;
                    

                    
                    std::shared_ptr<std::set<std::string>> StubPlugin::nativeStubReadersNames = std::make_shared<std::set<std::string>>();

                    StubPlugin::StubPlugin() : org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin("StubPlugin")
                    {
                        logger->debug("constructor\n");

                        /*
                         * Monitoring is not handled by a lower layer (as in PC/SC), reduce the threading period to
                         * 50 ms to speed up responsiveness.
                         */
                        threadWaitTimeout = 50;
                    }

                    std::shared_ptr<StubPlugin> StubPlugin::getInstance()
                    {
                        static StubPlugin uniqueInstance;

                        return std::shared_ptr<StubPlugin>(&uniqueInstance);
                    }

                    std::unordered_map<std::string, std::string> StubPlugin::getParameters()
                    {
                        return parameters;
                    }

                    void StubPlugin::setParameter(const std::string &key, const std::string &value)
                    {
                        parameters.emplace(key, value);
                    }

                    std::shared_ptr<std::set<std::shared_ptr<SeReader>>> StubPlugin::initNativeReaders() throw(KeypleReaderException) {
                        /* init Stub Readers list */
                        logger->debug("creating new list\n");
                        std::shared_ptr<std::set<std::shared_ptr<SeReader>>> nativeReaders =
                            std::shared_ptr<std::set<std::shared_ptr<SeReader>>>(
                                new std::set<std::shared_ptr<SeReader>>());

                        /*
                         * parse the current readers list to create the ProxyReader(s) associated with new reader(s)
                         */
                        if (nativeStubReadersNames != nullptr && nativeStubReadersNames->size() > 0) {
                            for (auto name : *nativeStubReadersNames) {
                                nativeReaders->insert(std::make_shared<StubReader>(name));
                            }
                        }
                        return nativeReaders;
                    }

                    std::shared_ptr<SeReader> StubPlugin::getNativeReader(const std::string &name)
                    {
                        for (auto reader : *readers) {
                            if (reader->getName() == name) {
                                return std::dynamic_pointer_cast < AbstractObservableReader>(reader);
                            }
                        }
                        std::shared_ptr<AbstractObservableReader> reader = nullptr;
                        if (nativeStubReadersNames->find(name) != nativeStubReadersNames->end())
                        {
                            reader = std::shared_ptr<StubReader>(new StubReader(name));
                        }
                        return reader;
                    }

                    void StubPlugin::plugStubReader(const std::string &name) {
                        if (nativeStubReadersNames->find(name) == nativeStubReadersNames->end())
                        {
                            logger->info("Plugging a new reader with name %s\n", name);
                            /* add the native reader to the native readers list */
                            nativeStubReadersNames->insert(name);
                            /* add the reader as a new reader to the readers list */
                            std::shared_ptr<StubReader> stubReader = std::shared_ptr<StubReader>(new StubReader(name));
                            readers->insert(std::static_pointer_cast<AbstractObservableReader>(stubReader));
                        }
                        else {
                            logger->error("Reader with name %s was already plugged\n", name);
                        }
                    }

                    void StubPlugin::unplugReader(const std::string &name) throw(KeypleReaderException) {

                        if (nativeStubReadersNames->find(name) == nativeStubReadersNames->end())
                        {
                            logger->warn("No reader found with name %s", name);
                        }
                        else {
                            /* remove the reader from the readers list */
                            readers->erase(getNativeReader(name));
                            /* remove the native reader from the native readers list */
                            nativeStubReadersNames->erase(name);
                            logger->info("Unplugged reader with name %s\n", name);
                        }
                    }

                    std::shared_ptr<std::set<std::string>> StubPlugin::getNativeReadersNames() {
                        if (nativeStubReadersNames->empty()) {
                            logger->trace("No reader available\n");
                        }
                        return nativeStubReadersNames;
                    }
                }
            }
        }
    }
}

/* Plugin - Stub */
#include "StubPlugin.h"
#include "StubReader.h"

/* Core */
#include "KeypleReaderException.h"
#include "AbstractObservableReader.h"
#include "KeypleReaderNotFoundException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace stub {
                    using KeypleReaderException            = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using KeypleReaderNotFoundException    = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                    using AbstractObservableReader         = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                    using AbstractThreadedObservablePlugin = org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin;

                    std::shared_ptr<std::set<std::string>> StubPlugin::connectedStubNames = std::make_shared<std::set<std::string>>();
                    std::shared_ptr<std::set<std::string>> StubPlugin::nativeStubReadersNames = std::make_shared<std::set<std::string>>();

                    StubPlugin::StubPlugin()
                        : org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin(
                              "StubPlugin")
                    {
                        logger->debug("constructor\n");

                        /*
                         * Monitoring is not handled by a lower layer (as in PC/SC), reduce the threading period to
                         * 50 ms to speed up responsiveness.
                         */
                        threadWaitTimeout = 50;
                    }

                    StubPlugin::~StubPlugin()
                    {
                        logger->debug("destructor\n");
                    }

                    StubPlugin& StubPlugin::getInstance()
                    {
                        static StubPlugin uniqueInstance;
                        return uniqueInstance;
                    }

                    std::unordered_map<std::string, std::string> StubPlugin::getParameters()
                    {
                        return parameters;
                    }

                    void StubPlugin::setParameter(const std::string &key, const std::string &value)
                    {
                        parameters.emplace(key, value);
                    }

                    std::shared_ptr<std::set<std::shared_ptr<SeReader>>>
                    StubPlugin::initNativeReaders() throw(KeypleReaderException)
                    {
                        /* init Stub Readers list */
                        logger->debug("creating new list\n");
                        std::shared_ptr<std::set<std::shared_ptr<SeReader>>> nativeReaders =
                            std::shared_ptr<std::set<std::shared_ptr<SeReader>>>(
                                new std::set<std::shared_ptr<SeReader>>());

                        /*
                         * parse the current readers list to create the ProxyReader(s) associated with new reader(s)
                         */
                        if (nativeStubReadersNames != nullptr && nativeStubReadersNames->size() > 0)
                        {
                            for (auto name : *nativeStubReadersNames)
                            {
                                nativeReaders->insert(std::make_shared<StubReader>(name));
                            }
                        }
                        return nativeReaders;
                    }

                    void StubPlugin::plugStubReader(const std::string &name, bool synchronous) {

                        logger->info("Plugging a new reader with name %s\n", name);
                        /* add the native reader to the native readers list */
                        bool exist = connectedStubNames->find(name) != connectedStubNames->end();

                        if (!exist && synchronous) {
                            /* add the reader as a new reader to the readers list */
                            readers->insert(std::make_shared<StubReader>(name));
                        }

                        connectedStubNames->insert(name);

                        if (exist) {
                            logger->error("Reader with name %s was already plugged\n", name);
                        }

                    }

                    void StubPlugin::plugStubReaders(std::shared_ptr<std::set<std::string>> names, bool synchronous) {
                        logger->debug("Plugging %s readers ..\n", names->size());

                        /* plug stub readers that were not plugged already */
                        // duplicate names
                        std::set<std::string> newNames(*names);
                        // remove already connected stubNames
                        for (auto name : *connectedStubNames)
                            newNames.erase(name);

                        logger->debug("New readers to be created #%s\n", newNames.size());


                        /*
                         * Add new names to the connectedStubNames
                         */

                        if (newNames.size() > 0) {
                            if (synchronous) {
                                std::vector<std::shared_ptr<StubReader>> newReaders;
                                for (auto name : newNames) {
                                    newReaders.push_back(std::make_shared<StubReader>(name));
                                }
                                readers->insert(newReaders.begin(), newReaders.end());
                            }

                            connectedStubNames->insert(names->begin(), names->end());

                        }
                        else {
                            logger->error("All %d readers were already plugged\n", names->size());

                        }


                    }

                    void StubPlugin::unplugStubReader(const std::string &name, bool synchronous) throw(KeypleReaderException, InterruptedException) {

                        if (connectedStubNames->find(name) == connectedStubNames->end()) {
                            logger->warn("unplugStubReader() No reader found with name %s\n", name);
                            }
                        else {
                            /* remove the reader from the readers list */
                            if (synchronous) {
                                connectedStubNames->erase(name);
                                readers->erase(getReader(name));
                        }
                            else {
                                connectedStubNames->erase(name);
                            }
                            /* remove the native reader from the native readers list */
                            logger->info("Unplugged reader with name %s, connectedStubNames size %s\n", name, connectedStubNames->size());
                        }
                    }

                    void StubPlugin::unplugStubReaders(std::shared_ptr<std::set<std::string>> names, bool synchronous) {
                        logger->info("Unplug %d stub readers\n", names->size());
                        logger->debug("Unplug stub readers.. %s\n", names);
                        std::vector<std::shared_ptr<StubReader>> readersToDelete;
                        for (auto name : *names) {
                            try {
                                readersToDelete.push_back(std::dynamic_pointer_cast<StubReader>(getReader(name)));
                        }
                            catch (const KeypleReaderNotFoundException &e) {
                                logger->warn("unplugStubReaders() No reader found with name %s\n", name);
                            }
                        }
                        for (auto name : *names)
                        	connectedStubNames->erase(name);
                        if (synchronous) {
                            for (auto name : readersToDelete)
                                readers->erase(name);
                        }
                    }

                    std::shared_ptr<std::set<std::string>> StubPlugin::fetchNativeReadersNames() {
                        if (connectedStubNames->empty()) {
                            logger->trace("No reader available\n");
                        }
                        return connectedStubNames;
                    }

                    std::shared_ptr<AbstractObservableReader> StubPlugin::fetchNativeReader(const std::string &name) {
                        for (auto reader : *readers) {
                            if (reader->getName() == name) {
                                return std::dynamic_pointer_cast<AbstractObservableReader>(reader);
                            }
                        }
                        std::shared_ptr<AbstractObservableReader> reader = nullptr;
                        if (connectedStubNames->find(name) != connectedStubNames->end()) {
                            reader = std::make_shared<StubReader>(name);
                        }
                        return reader;
                    }
                }
            }
        }
    }
}

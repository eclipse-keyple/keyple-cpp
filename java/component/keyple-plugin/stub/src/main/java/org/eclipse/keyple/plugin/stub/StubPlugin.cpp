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
                    using KeypleReaderException =
                        org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using AbstractObservableReader =
                        org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                    using AbstractThreadedObservablePlugin =
                        org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin;

                    std::shared_ptr<std::set<std::string>> StubPlugin::nativeStubReadersNames =
                        std::make_shared<std::set<std::string>>();

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

                    void StubPlugin::plugStubReader(const std::string &name, Boolean synchronous) {

                        logger->info("Plugging a new reader with name " + name);
                        /* add the native reader to the native readers list */
                        Boolean exist = connectedStubNames->contains(name);

                        if (!exist && synchronous) {
                            /* add the reader as a new reader to the readers list */
                            readers->add(std::make_shared<StubReader>(name));
                        }

                        connectedStubNames->add(name);

                        if (exist) {
                            logger->error("Reader with name " + name + " was already plugged");
                        }

                    }

                    void StubPlugin::plugStubReaders(std::shared_ptr<Set<std::string>> names, Boolean synchronous) {
                        logger->debug("Plugging {} readers ..", names->size());

                        /* plug stub readers that were not plugged already */
                        // duplicate names
                        std::shared_ptr<Set<std::string>> newNames = std::unordered_set<std::string>(names);
                        // remove already connected stubNames
                        newNames->removeAll(connectedStubNames);

                        logger->debug("New readers to be created #{}", newNames->size());


                        /*
                         * Add new names to the connectedStubNames
                         */

                        if (newNames->size() > 0) {
                            if (synchronous) {
                                std::vector<std::shared_ptr<StubReader>> newReaders;
                                for (auto name : newNames) {
                                    newReaders.push_back(std::make_shared<StubReader>(name));
                                }
                                readers->addAll(newReaders);
                            }

                            connectedStubNames->addAll(names);

                        }
                        else {
                            logger->error("All {} readers were already plugged", names->size());

                        }


                    }

                    void StubPlugin::unplugStubReader(const std::string &name, Boolean synchronous) throw(KeypleReaderException, InterruptedException) {

                        if (!connectedStubNames->contains(name)) {
                            logger->warn("unplugStubReader() No reader found with name {}", name);
                            }
                        else {
                            /* remove the reader from the readers list */
                            if (synchronous) {
                                connectedStubNames->remove(name);
                                readers->remove(getReader(name));
                        }
                            else {
                                connectedStubNames->remove(name);
                            }
                            /* remove the native reader from the native readers list */
                            logger->info("Unplugged reader with name {}, connectedStubNames size {}", name, connectedStubNames->size());
                        }
                    }

                    void StubPlugin::unplugStubReaders(std::shared_ptr<Set<std::string>> names, Boolean synchronous) {
                        logger->info("Unplug {} stub readers", names->size());
                        logger->debug("Unplug stub readers.. {}", names);
                        std::vector<std::shared_ptr<StubReader>> readersToDelete;
                        for (auto name : names) {
                            try {
                                readersToDelete.push_back(std::static_pointer_cast<StubReader>(getReader(name)));
                        }
                            catch (const KeypleReaderNotFoundException &e) {
                                logger->warn("unplugStubReaders() No reader found with name {}", name);
                            }
                        }
                        connectedStubNames->removeAll(names);
                        if (synchronous) {
                            readers->removeAll(readersToDelete);
                        }
                    }

                    std::shared_ptr<SortedSet<std::string>> StubPlugin::fetchNativeReadersNames() {
                        if (connectedStubNames->isEmpty()) {
                            logger->trace("No reader available.");
                        }
                        return connectedStubNames;
                        }

                    std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> StubPlugin::initNativeReaders() throw(KeypleReaderException) {
                        /* init Stub Readers response object */
                        std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> newNativeReaders = std::make_shared<ConcurrentSkipListSet<std::shared_ptr<AbstractObservableReader>>>();

                        /*
                         * parse the current readers list to create the ProxyReader(s) associated with new reader(s)
                         * if (connectedStubNames != null && connectedStubNames.size() > 0) { for (String name :
                         * connectedStubNames) { newNativeReaders.add(new StubReader(name)); } }
                         */
                        return newNativeReaders;
                    }

                    std::shared_ptr<AbstractObservableReader> StubPlugin::fetchNativeReader(const std::string &name) {
                        for (auto reader : readers) {
                            if (reader->getName() == name) {
                                return reader;
                            }
                        }
                        std::shared_ptr<AbstractObservableReader> reader = nullptr;
                        if (connectedStubNames->contains(name)) {
                            reader = std::make_shared<StubReader>(name);
                        }
                        return reader;
                    }
                }
            }
        }
    }
}

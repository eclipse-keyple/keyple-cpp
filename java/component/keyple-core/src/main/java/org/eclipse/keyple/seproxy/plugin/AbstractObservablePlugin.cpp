#include "AbstractObservablePlugin.h"
#include "../event/PluginEvent.h"
#include "AbstractObservableReader.h"
#include "../exception/KeypleReaderException.h"
#include "../event/ObservablePlugin.h"
#include "../exception/KeypleReaderNotFoundException.h"
#include "../message/ProxyReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {
                    using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                    using ObservablePlugin = org::eclipse::keyple::seproxy::event::ObservablePlugin;
                    using PluginEvent = org::eclipse::keyple::seproxy::event::PluginEvent;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                    using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
//                    using org::slf4j::Logger;
//                    using org::slf4j::LoggerFactory;
//                    const std::shared_ptr<org::slf4j::Logger> AbstractObservablePlugin::logger = org::slf4j::LoggerFactory::getLogger(AbstractObservablePlugin::typeid);

                    AbstractObservablePlugin::AbstractObservablePlugin(const std::string &name) : AbstractLoggedObservable<org::eclipse::keyple::seproxy::event_Renamed::PluginEvent>(name) {
                        if (readers == nullptr) {
                            try {
                                readers = getNativeReaders();
                            }
                            catch (const KeypleReaderException &e) {
                                e->printStackTrace();
                            }
                        }
                    }

                    std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> AbstractObservablePlugin::getReaders() throw(KeypleReaderException) {
                        if (readers == nullptr) {
                            throw std::make_shared<KeypleReaderException>("List of readers has not been initialized");
                        }
                        return readers;
                    }

                    void AbstractObservablePlugin::startObservation() {
                    };

                    void AbstractObservablePlugin::stopObservation() {
                    };

                    void AbstractObservablePlugin::addObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer) {
                        AbstractLoggedObservable<PluginEvent>::addObserver(observer);
                        if (AbstractLoggedObservable<PluginEvent>::countObservers() == 1) {
//                            logger->debug("Start the plugin monitoring.");
                            startObservation();
                        }
                    }

                    void AbstractObservablePlugin::removeObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer) {
                        AbstractLoggedObservable<PluginEvent>::removeObserver(observer);
                        if (AbstractLoggedObservable<PluginEvent>::countObservers() == 0) {
//                            logger->debug("Stop the plugin monitoring.");
                            stopObservation();
                        }
                    }

                    int AbstractObservablePlugin::compareTo(std::shared_ptr<ReaderPlugin> plugin) {
                        return this->getName().compare(plugin->getName());
                    }

                    std::shared_ptr<ProxyReader> AbstractObservablePlugin::getReader(const std::string &name) throw(KeypleReaderNotFoundException) {
                        for (auto reader : readers) {
                            if (reader->getName() == name) {
                                return reader;
                            }
                        }
                        throw std::make_shared<KeypleReaderNotFoundException>(name);
                    }
                }
            }
        }
    }
}

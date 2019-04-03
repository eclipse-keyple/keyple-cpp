#include "AbstractObservablePlugin.h"
#include "../event/PluginEvent.h"
#include "AbstractObservableReader.h"
#include "../exception/KeypleReaderException.h"
#include "../event/ObservablePlugin.h"
#include "../exception/KeypleReaderNotFoundException.h"
#include "../message/ProxyReader.h"

#include "LoggerFactory.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {

                    using ReaderPlugin     = org::eclipse::keyple::seproxy::ReaderPlugin;
                    using ObservablePlugin = org::eclipse::keyple::seproxy::event::ObservablePlugin;
                    using PluginEvent      = org::eclipse::keyple::seproxy::event::PluginEvent;
                    using KeypleReaderException =
                        org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using KeypleReaderNotFoundException =
                        org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                    using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;

                    AbstractObservablePlugin::AbstractObservablePlugin(const std::string &name)
                        : AbstractLoggedObservable<std::shared_ptr<PluginEvent>>(name)
                    {
                        logger->debug("constructor (name: %s)\n", name);
                    }

                    void AbstractObservablePlugin::initReaders()
                    {
                        logger->debug("readers: %p\n", readers);
                        if (readers == nullptr)
                        {
                            try
                            {
                                logger->debug("initializing native readers\n");
                                readers = initNativeReaders();
                                logger->debug("readers: %p\n", readers);
                            } catch (KeypleReaderException &e)
                            {
                                logger->debug("KeypleReaderException\n");
                                e.printStackTrace();
                            }
                        }
                    }

                    std::shared_ptr<std::set<std::shared_ptr<SeReader>>> AbstractObservablePlugin::getReaders()
                    {
                        if (readers == nullptr) {
                            throw std::make_shared<KeypleReaderException>("List of readers has not been initialized");
                        }
                        return readers;
                    }

                    std::shared_ptr<std::set<std::string>> AbstractObservablePlugin::getReaderNames() {
                        std::shared_ptr<std::set<std::string>> readerNames = std::make_shared<std::set<std::string>>();
                        for (auto reader : *readers) {
                            readerNames->insert(reader->getName());
                        }
                        return readerNames;
                    }

                    void AbstractObservablePlugin::addObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer) {
                        AbstractLoggedObservable<std::shared_ptr<PluginEvent>>::addObserver(
                            std::dynamic_pointer_cast<org::eclipse::keyple::util::Observer<std::shared_ptr<PluginEvent>>>(observer));
                        if (AbstractLoggedObservable<std::shared_ptr<PluginEvent>>::countObservers() == 1) {
                            logger->debug("Start the plugin monitoring.");
                            startObservation();
                        }
                    }

                    void AbstractObservablePlugin::removeObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer) {
                        AbstractLoggedObservable<std::shared_ptr<PluginEvent>>::removeObserver(
                            std::dynamic_pointer_cast<org::eclipse::keyple::util::Observer<std::shared_ptr<PluginEvent>>>(observer));
                        if (AbstractLoggedObservable<std::shared_ptr<PluginEvent>>::countObservers() == 0) {
                            logger->debug("Stop the plugin monitoring.");
                            stopObservation();
                        }
                    }

                    int AbstractObservablePlugin::compareTo(std::shared_ptr<ReaderPlugin> plugin)
                    {
                        return this->AbstractLoggedObservable::getName().compare(plugin->getName());
                    }

                    /*
                     * Alex: consider note in header comment (covariant return type).
                     */
                    std::shared_ptr<SeReader> AbstractObservablePlugin::getReader(const std::string &name)
                    {
                        for (auto reader : *readers)
                        {
                            if (reader->getName() == name)
                            {
                                return std::shared_ptr<SeReader>(
                                    std::dynamic_pointer_cast<SeReader>(reader));
                            }
                        }
                        throw std::make_shared<KeypleReaderNotFoundException>(name);
                    }

                } // namespace plugin
            }     // namespace seproxy
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org

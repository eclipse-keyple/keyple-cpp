/* Core */
#include "AbstractThreadedObservablePlugin.h"
#include "PluginEvent.h"
#include "AbstractObservableReader.h"
#include "KeypleReaderException.h"
#include "InterruptedException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {

                    using ObservablePlugin = org::eclipse::keyple::seproxy::event::ObservablePlugin;
                    using PluginEvent      = org::eclipse::keyple::seproxy::event::PluginEvent;
                    using KeypleReaderException =
                        org::eclipse::keyple::seproxy::exception::KeypleReaderException;

                    std::set<std::string> _set;
                    std::shared_ptr<std::set<std::string>> nativeReadersNames =
                        std::make_shared<std::set<std::string>>(_set);

                    AbstractThreadedObservablePlugin::AbstractThreadedObservablePlugin(
                        const std::string &name)
                        : AbstractObservablePlugin(name)
                    {
                        logger->debug("constructor (name: %s)\n", name);
                    }

                    AbstractThreadedObservablePlugin::~AbstractThreadedObservablePlugin()
                    {
                        logger->debug("destructor (name: %s)\n", name);
                    }

                    void AbstractThreadedObservablePlugin::startObservation() {
                        logger->debug("\n");
                        thread = std::make_shared<AbstractThreadedObservablePlugin::EventThread>(shared_from_this(), this->getName());
                        thread->start();
                    }

                    void AbstractThreadedObservablePlugin::stopObservation() {
                        logger->debug("\n");
                        if (thread != nullptr) {
                            thread->end();
                        }
                    }

                    AbstractThreadedObservablePlugin::EventThread::EventThread(
                        std::shared_ptr<AbstractThreadedObservablePlugin> outerInstance,
                        const std::string &pluginName)
                        : pluginName(pluginName), outerInstance(outerInstance)
                    {
                        outerInstance->logger->debug("constructor with outerInstance: %p, pluginName: %s\n",
                                                     outerInstance, pluginName);
                    }

                    void AbstractThreadedObservablePlugin::EventThread::end()
                    {
                        outerInstance->logger->debug("\n");

                        running = false;
                        this->interrupt();
                    }

                    void *AbstractThreadedObservablePlugin::EventThread::run()
                    {
                        try
                        {
                            while (running)
                            {
                                /* retrieves the current readers names list */
                                std::shared_ptr<std::set<std::string>> actualNativeReadersNames =
                                    outerInstance->fetchNativeReadersNames();

                                /* Checks if it has changed this algorithm favors cases where nothing change */
                                if (outerInstance->nativeReadersNames != actualNativeReadersNames)
                                {
                                    /*
                                     * parse the current readers list, notify for disappeared readers, update
                                     * readers list
                                     */
                                    /* build changed reader names list */
                                    changedReaderNames->clear();
                                    for (auto reader : outerInstance->readers) {
                                        if (!actualNativeReadersNames->contains(reader->getName())) {
                                            changedReaderNames->add(reader->getName());
                                        }
                                    }
                                    /* notify disconnections if any and update the reader list */
                                    if (changedReaderNames->size() > 0) {
                                        /* grouped notification */
                                        outerInstance->notifyObservers(std::make_shared<PluginEvent>(this->pluginName, changedReaderNames, PluginEvent::EventType::READER_DISCONNECTED));
                                        /* list update */
                                        for (auto reader : outerInstance->readers) {
                                            if (!actualNativeReadersNames->contains(reader->getName())) {
                                                outerInstance->readers->remove(reader);
                                                logger->trace("[{}][{}] Plugin thread => Remove unplugged reader from readers list.", this->pluginName, reader->getName());
                                                /* remove reader name from the current list */
                                                outerInstance->nativeReadersNames->remove(reader->getName());
                                        }
                                    }
                                        /* clean the list for a possible connection notification */
                                        changedReaderNames->clear();
                                    }
                                    /*
                                     * parse the new readers list, notify for readers appearance, update readers
                                     * list
                                     */
                                    for (auto readerName : actualNativeReadersNames) {
                                        if (!outerInstance->nativeReadersNames->contains(readerName)) {
                                            std::shared_ptr<AbstractObservableReader> reader = outerInstance->fetchNativeReader(readerName);
                                            outerInstance->readers->add(reader);
                                            /* add to the notification list */
                                            changedReaderNames->add(readerName);
                                            logger->trace("[{}][{}] Plugin thread => Add plugged reader to readers list.", this->pluginName, reader->getName());
                                            /* add reader name to the current list */
                                            outerInstance->nativeReadersNames->add(readerName);
                                        }
                                    }
                                    /* notify connections if any */
                                    if (changedReaderNames->size() > 0) {
                                        outerInstance->notifyObservers(std::make_shared<PluginEvent>(this->pluginName, changedReaderNames, PluginEvent::EventType::READER_CONNECTED));
                                    }
                                }
                                /* sleep for a while. */
                                delay(outerInstance->threadWaitTimeout);
                            }
                        }
                        catch (const InterruptedException &e) {
                            e->printStackTrace();
                            logger->warn("[{}] An exception occurred while monitoring plugin: {}, cause {}", this->pluginName, e->getMessage(), e->getCause());
                        }
                        catch (const KeypleReaderException &e) {
                            e->printStackTrace();
                            logger->warn("[{}] An exception occurred while monitoring plugin: {}, cause {}", this->pluginName, e->what(), e->getCause());
                                }
                            }

                        thread->end();
                        thread.reset();
                        logger->trace("[%s] observable Plugin thread ended.", this->getName());
                        AbstractObservablePlugin::finalize();
                    }

                } // namespace plugin
            }     // namespace seproxy
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org

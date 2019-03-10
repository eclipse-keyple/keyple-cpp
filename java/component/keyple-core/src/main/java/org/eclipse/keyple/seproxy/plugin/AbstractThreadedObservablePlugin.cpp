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

                    void AbstractThreadedObservablePlugin::startObservation()
                    {
                        logger->debug("\n");

                        thread = std::shared_ptr<EventThread>(
                            new EventThread(shared_from_this(), this->getName()));
                        thread->start();
                    }

                    void AbstractThreadedObservablePlugin::stopObservation()
                    {
                        logger->debug("\n");

                        if (thread != nullptr)
                        {
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
                                    outerInstance->getNativeReadersNames();

                                /* Checks if it has changed this algorithm favors cases where nothing change */
                                if (outerInstance->nativeReadersNames != actualNativeReadersNames)
                                {
                                    /*
                                     * parse the current readers list, notify for disappeared readers, update
                                     * readers list
                                     */
                                    outerInstance->logger->debug("updating current list (removal)\n");
                                    for (auto it = (*outerInstance->readers).begin();
                                         it != (*outerInstance->readers).end();)
                                    {
                                        if (actualNativeReadersNames->find((*it)->getName()) ==
                                            actualNativeReadersNames->end())
                                        {
                                            outerInstance->AbstractLoggedObservable::notifyObservers(
                                                std::make_shared<PluginEvent>(
                                                    this->pluginName, (*it)->getName(),
                                                    PluginEvent::EventType::READER_DISCONNECTED));
                                            outerInstance->logger->trace(
                                                "plugin thread (%s) => remove unplugged reader from readers "
                                                "list: %s\n",
                                                this->pluginName, (*it)->getName());

                                            /* remove reader name from the current lists */
                                            outerInstance->nativeReadersNames->erase((*it)->getName());
                                            it = outerInstance->readers->erase(it);
                                        }
                                        else
                                        {
                                            it++;
                                        }
                                    }

                                    /*
                                     * parse the new readers list, notify for readers appearance, update readers
                                     * list
                                     */
                                    outerInstance->logger->debug("updating current list (addition)\n");
                                    for (auto readerName : *actualNativeReadersNames)
                                    {
                                        if (outerInstance->nativeReadersNames->find(readerName) ==
                                            outerInstance->nativeReadersNames->end())
                                        {
                                            std::shared_ptr<SeReader> reader =
                                                outerInstance->getNativeReader(readerName);
                                            outerInstance->readers->insert(reader);
                                            outerInstance->AbstractLoggedObservable::notifyObservers(
                                                std::make_shared<PluginEvent>(
                                                    this->pluginName, reader->getName(),
                                                    PluginEvent::EventType::READER_CONNECTED));
                                            outerInstance->logger->trace("plugin thread (%s) => add plugged "
                                                                         "reader to readers list: %s",
                                                                         this->pluginName, reader->getName());
                                            /* Add reader name to the current list */
                                            outerInstance->nativeReadersNames->insert(readerName);
                                        }
                                    }
                                }

                                /* Sleep for a while */
                                Thread::sleep(outerInstance->threadWaitTimeout);
                            }
                        } catch (InterruptedException &e)
                        {
                            e.printStackTrace();
                            outerInstance->logger->warn(
                                "an exception occurred while monitoring plugin: %s, message: %s, cause %s",
                                this->pluginName, e.getMessage(), e.getCause().what());
                        } catch (KeypleReaderException &e)
                        {
                            e.printStackTrace();
                            outerInstance->logger->warn(
                                "an exception occurred while monitoring plugin: %s, message: %s, cause %s",
                                this->pluginName, e.getMessage(), e.getCause().what());
                        }

                        return NULL;
                    }

                    void AbstractThreadedObservablePlugin::finalize() throw(std::runtime_error)
                    {
                        logger->debug("finalizing\n");

                        thread->end();
                        thread.reset();
                        logger->trace("[%s] observable Plugin thread ended.", this->getName());

                        /*
                         * Alex...
                         */
                        //AbstractObservablePlugin::finalize();
                    }

                } // namespace plugin
            }     // namespace seproxy
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org

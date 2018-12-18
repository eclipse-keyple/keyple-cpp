#include "AbstractThreadedObservablePlugin.h"
#include "../event/PluginEvent.h"
#include "AbstractObservableReader.h"
#include "../exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {
                    using ObservablePlugin = org::eclipse::keyple::seproxy::event_Renamed::ObservablePlugin;
                    using PluginEvent = org::eclipse::keyple::seproxy::event_Renamed::PluginEvent;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> AbstractThreadedObservablePlugin::logger = org::slf4j::LoggerFactory::getLogger(AbstractThreadedObservablePlugin::typeid);
std::shared_ptr<java::util::SortedSet<std::string>> AbstractThreadedObservablePlugin::nativeReadersNames = std::make_shared<java::util::concurrent::ConcurrentSkipListSet<std::string>>();

                    AbstractThreadedObservablePlugin::AbstractThreadedObservablePlugin(const std::string &name) : AbstractObservablePlugin(name) {
                    }

                    void AbstractThreadedObservablePlugin::startObservation() {
                        thread = std::make_shared<AbstractThreadedObservablePlugin::EventThread>(shared_from_this(), this->getName());
                        thread->start();
                    }

                    void AbstractThreadedObservablePlugin::stopObservation() {
                        thread->end();
                    }

                    AbstractThreadedObservablePlugin::EventThread::EventThread(std::shared_ptr<AbstractThreadedObservablePlugin> outerInstance, const std::string &pluginName) : pluginName(pluginName), outerInstance(outerInstance) {
                    }

                    void AbstractThreadedObservablePlugin::EventThread::end() {
                        running = false;
                        this->interrupt();
                    }

                    void AbstractThreadedObservablePlugin::EventThread::run() {
                        try {
                            while (running) {
                                /* retrieves the current readers names list */
                                std::shared_ptr<SortedSet<std::string>> actualNativeReadersNames = outerInstance->getNativeReadersNames();
                                /*
                                 * checks if it has changed this algorithm favors cases where nothing change
                                 */
                                if (!nativeReadersNames->equals(actualNativeReadersNames)) {
                                    /*
                                     * parse the current readers list, notify for disappeared readers, update
                                     * readers list
                                     */
                                    for (auto reader : outerInstance->readers) {
                                        if (!actualNativeReadersNames->contains(reader->getName())) {
                                            outerInstance->notifyObservers(std::make_shared<PluginEvent>(this->pluginName, reader->getName(), PluginEvent::EventType::READER_DISCONNECTED));
                                            outerInstance->readers->remove(reader);
                                            logger->trace("[{}][{}] Plugin thread => Remove unplugged reader from readers list.", this->pluginName, reader->getName());
                                            /* remove reader name from the current list */
                                            nativeReadersNames->remove(reader->getName());
                                            reader.reset();
                                        }
                                    }
                                    /*
                                     * parse the new readers list, notify for readers appearance, update readers
                                     * list
                                     */
                                    for (auto readerName : actualNativeReadersNames) {
                                        if (!nativeReadersNames->contains(readerName)) {
                                            std::shared_ptr<AbstractObservableReader> reader = outerInstance->getNativeReader(readerName);
                                            outerInstance->readers->add(reader);
                                            outerInstance->notifyObservers(std::make_shared<PluginEvent>(this->pluginName, reader->getName(), PluginEvent::EventType::READER_CONNECTED));
                                            logger->trace("[{}][{}] Plugin thread => Add plugged reader to readers list.", this->pluginName, reader->getName());
                                            /* add reader name to the current list */
                                            nativeReadersNames->add(readerName);
                                        }
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

//JAVA TO C++ CONVERTER WARNING: Unlike Java, there is no automatic call to this finalizer method in native C++:
                    void AbstractThreadedObservablePlugin::finalize() throw(std::runtime_error) {
                        thread->end();
                        thread.reset();
                        logger->trace("[{}] Observable Plugin thread ended.", this->getName());
                        AbstractObservablePlugin::finalize();
                    }
                }
            }
        }
    }
}

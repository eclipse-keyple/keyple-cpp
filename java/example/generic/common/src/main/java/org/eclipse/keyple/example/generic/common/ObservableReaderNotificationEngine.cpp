#include "ObservableReaderNotificationEngine.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/PluginEvent.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeyplePluginNotFoundException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace common {
                        using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using ObservablePlugin = org::eclipse::keyple::seproxy::event_Renamed::ObservablePlugin;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using PluginEvent = org::eclipse::keyple::seproxy::event_Renamed::PluginEvent;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeyplePluginNotFoundException = org::eclipse::keyple::seproxy::exception::KeyplePluginNotFoundException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> ObservableReaderNotificationEngine::logger = org::slf4j::LoggerFactory::getLogger(ObservableReaderNotificationEngine::typeid);

                        ObservableReaderNotificationEngine::ObservableReaderNotificationEngine() {
                            /* initializes observers */
//JAVA TO C++ CONVERTER TODO TASK: You cannot use 'shared_from_this' in a constructor:
                            std::shared_ptr<SpecificReaderObserver> readerObserver = std::make_shared<SpecificReaderObserver>(shared_from_this());
//JAVA TO C++ CONVERTER TODO TASK: You cannot use 'shared_from_this' in a constructor:
                            pluginObserver = std::make_shared<SpecificPluginObserver>(shared_from_this(), readerObserver);
                        }

                        void ObservableReaderNotificationEngine::setPluginObserver() {

                            /*
                             * We add an observer to each plugin (only one in this example) the readers observers will
                             * be added dynamically upon plugin notification (see SpecificPluginObserver.update)
                             */
                            for (auto plugin : SeProxyService::getInstance()->getPlugins()) {

                                if (std::dynamic_pointer_cast<ObservablePlugin>(plugin) != nullptr) {
                                    logger->info("Add observer PLUGINNAME = {}", plugin->getName());
                                    (std::static_pointer_cast<ObservablePlugin>(plugin))->addObserver(this->pluginObserver);
                                }
                                else {
                                    logger->info("PLUGINNAME = {} isn't observable", plugin->getName());
                                }
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: No base class can be determined:
                        ObservableReaderNotificationEngine::SpecificReaderObserver::SpecificReaderObserver(std::shared_ptr<ObservableReaderNotificationEngine> outerInstance) : outerInstance(outerInstance) {
                        }

                        void ObservableReaderNotificationEngine::SpecificReaderObserver::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                            /* just log the event */
                            logger->info("Event: PLUGINNAME = {}, READERNAME = {}, EVENT = {}", event_Renamed->getPluginName(), event_Renamed->getReaderName(), event_Renamed->getEventType().getName());
                        }

                        ObservableReaderNotificationEngine::SpecificPluginObserver::SpecificPluginObserver(std::shared_ptr<ObservableReaderNotificationEngine> outerInstance, std::shared_ptr<SpecificReaderObserver> readerObserver) : outerInstance(outerInstance) {
                            this->readerObserver = readerObserver;
                        }

                        void ObservableReaderNotificationEngine::SpecificPluginObserver::update(std::shared_ptr<PluginEvent> event_Renamed) {
                            std::shared_ptr<SeReader> reader = nullptr;
                            logger->info("PluginEvent: PLUGINNAME = {}, READERNAME = {}, EVENTTYPE = {}", event_Renamed->getPluginName(), event_Renamed->getReaderName(), event_Renamed->getEventType());

                            /* We retrieve the reader object from its name. */
                            try {
                                reader = SeProxyService::getInstance()->getPlugin(event_Renamed->getPluginName())->getReader(event_Renamed->getReaderName());
                            }
                            catch (const KeyplePluginNotFoundException &e) {
                                e->printStackTrace();
                            }
                            catch (const KeypleReaderNotFoundException &e) {
                                e->printStackTrace();
                            }
                            switch (event_Renamed->getEventType()) {
                                case READER_CONNECTED:
                                    logger->info("New reader! READERNAME = {}", reader->getName());

                                    /*
                                     * We are informed here of a disconnection of a reader.
                                     * 
                                     * We add an observer to this reader if this is possible.
                                     */
                                    if (std::dynamic_pointer_cast<ObservableReader>(reader) != nullptr) {
                                        if (readerObserver != nullptr) {
                                            logger->info("Add observer READERNAME = {}", reader->getName());
                                            (std::static_pointer_cast<ObservableReader>(reader))->addObserver(readerObserver);
                                        }
                                        else {
                                            logger->info("No observer to add READERNAME = {}", reader->getName());
                                        }
                                    }
                                    break;
                                case READER_DISCONNECTED:
                                    /*
                                     * We are informed here of a disconnection of a reader.
                                     *
                                     * The reader object still exists but will be removed from the reader list right
                                     * after. Thus, we can properly remove the observer attached to this reader
                                     * before the list update.
                                     */
                                    logger->info("Reader removed. READERNAME = {}", event_Renamed->getReaderName());
                                    if (std::dynamic_pointer_cast<ObservableReader>(reader) != nullptr) {
                                        if (readerObserver != nullptr) {
                                            logger->info("Remove observer READERNAME = {}", event_Renamed->getReaderName());
                                            (std::static_pointer_cast<ObservableReader>(reader))->removeObserver(readerObserver);
                                        }
                                        else {
                                            logger->info("Unplugged reader READERNAME = {} wasn't observed.", event_Renamed->getReaderName());
                                        }
                                    }
                                    break;
                                default:
                                    logger->info("Unexpected reader event. EVENT = {}", event_Renamed->getEventType().getName());
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}

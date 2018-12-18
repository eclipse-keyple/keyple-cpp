#include "AbstractReaderObserverEngine.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SelectionResponse.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace common {
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using SelectionResponse = org::eclipse::keyple::transaction::SelectionResponse;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
std::shared_ptr<org::slf4j::Logger> AbstractReaderObserverEngine::logger = org::slf4j::LoggerFactory::getLogger(AbstractReaderObserverEngine::typeid);

                        void AbstractReaderObserverEngine::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                            if (event_Renamed->getEventType() != ReaderEvent::EventType::SE_INSERTED && logger->isInfoEnabled()) {
                                logger->info(event_Renamed->getReaderName());
                                logger->info("Start the processing of the SE...");
                            }

                            switch (event_Renamed->getEventType()) {

                                case SE_INSERTED:
                                    currentlyProcessingSe = true;
                                    processSeInsertion(); // optional, to process alternative AID selection
                                    currentlyProcessingSe = false;
                                    break;

                                case SE_MATCHED:
                                    currentlyProcessingSe = true;
                                    processSeMatch(event_Renamed->getDefaultSelectionResponse()); // to process the selected
                                    // application
                                    currentlyProcessingSe = false;
                                    break;

                                case SE_REMOVAL:
                                    if (currentlyProcessingSe) {
                                        processUnexpectedSeRemoval(); // to clean current SE processing
                                        logger->error("Unexpected SE Removal");
                                    }
                                    else {
                                        processSeRemoval();
                                        if (logger->isInfoEnabled()) {
                                            logger->info("Waiting for a SE...");
                                        }
                                    }
                                    currentlyProcessingSe = false;
                                    break;
                                default:

                                    logger->error("IO Error");
                            }
                        }
                    }
                }
            }
        }
    }
}

/* Examples */
#include "AbstractReaderObserverEngine.h"

/* Core */
#include "ReaderEvent_Import.h"
#include "SelectionResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic {
                    namespace common {

                        using ObservableReader  = org::eclipse::keyple::seproxy::event::ObservableReader;
                        using ReaderEvent       = org::eclipse::keyple::seproxy::event::ReaderEvent;
                        using SelectionResponse = org::eclipse::keyple::seproxy::event::SelectionResponse;

                        void AbstractReaderObserverEngine::update(std::shared_ptr<ReaderEvent> event)
                        {
                            if (event->getEventType() != ReaderEvent::EventType::SE_INSERTED &&
                                logger->isInfoEnabled())
                            {
                                logger->info("%s\n", event->getReaderName());
                                logger->info("Start the processing of the SE...\n");
                            }

                            ReaderEvent::EventType eventType = event->getEventType();
                            if (eventType == ReaderEvent::EventType::SE_INSERTED)
                            {
                                currentlyProcessingSe = true;
                                processSeInsertion(); // optional, to process alternative AID selection
                                currentlyProcessingSe = false;
                            }
                            else if (eventType == ReaderEvent::EventType::SE_MATCHED)
                            {
                                currentlyProcessingSe = true;
                                processSeMatch(
                                    event->getDefaultSelectionResponse()); // to process the selected
                                // application
                                currentlyProcessingSe = false;
                            }
                            else if (eventType == ReaderEvent::EventType::SE_REMOVAL)
                            {
                                if (currentlyProcessingSe)
                                {
                                    processUnexpectedSeRemoval(); // to clean current SE processing
                                    logger->error("Unexpected SE Removal\n");
                                }
                                else
                                {
                                    processSeRemoval();
                                    if (logger->isInfoEnabled())
                                    {
                                        logger->info("Waiting for a SE...");
                                    }
                                }
                                currentlyProcessingSe = false;
                            }
                            else
                            {

                                logger->error("IO Error");
                            }
                        }
                    } // namespace common
                }     // namespace generic
            }         // namespace example
        }             // namespace keyple
    }                 // namespace eclipse
} // namespace org

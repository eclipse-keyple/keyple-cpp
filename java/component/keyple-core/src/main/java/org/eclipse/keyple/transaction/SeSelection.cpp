#include "KeypleReaderException.h"
#include "MatchingSe.h"
#include "ProxyReader.h"
#include "SeRequest.h"
#include "SeReader.h"
#include "SelectionResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "SeRequestSet.h"
#include "SeSelection.h"
#include "SeSelector.h"
#include "SeSelectionRequest.h"
#include "LoggerFactory.h"

namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeSelectionRequest; }}}}

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace transaction {
                using SeReader = org::eclipse::keyple::seproxy::SeReader;
                using DefaultSelectionRequest = org::eclipse::keyple::seproxy::event::DefaultSelectionRequest;
                using SelectionResponse = org::eclipse::keyple::seproxy::event::SelectionResponse;
                using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;

                SeSelection::SeSelection(std::shared_ptr<SeReader> seReader) : seReader(seReader) {
                }

                std::shared_ptr<MatchingSe> SeSelection::prepareSelection(std::shared_ptr<SeSelectionRequest> seSelectionRequest) {
                    if (logger->isTraceEnabled()) {
                        logger->trace("SELECTORREQUEST = %s, EXTRAINFO = %s", seSelectionRequest->getSelectionRequest(), seSelectionRequest->getSeSelector()->getExtraInfo());
                    }
                    selectionRequestSet->add(seSelectionRequest->getSelectionRequest());
                    std::shared_ptr<MatchingSe> matchingSe = nullptr;
                    try {
                        /*
                         * Alex: problem, C++ cannot instanciate objects based on runtime-known types.
                         */
                        //std::shared_ptr<Constructor> constructor = seSelectionRequest->getMatchingClass().getConstructor(seSelectionRequest->getSelectionClass());
                        //matchingSe = std::static_pointer_cast<MatchingSe>(constructor->newInstance(seSelectionRequest));
                        //matchingSeList.push_back(matchingSe);
                    }
                    catch (const NoSuchMethodException &e) {
                        //e.printStackTrace();
                    }
                    catch (const InstantiationException &e) {
                        //e.printStackTrace();
                    }
                    catch (const IllegalAccessException &e) {
                        //e.printStackTrace();
                    }
                    catch (const InvocationTargetException &e) {
                        //e.printStackTrace();
                    }
                    return matchingSe;
                }

                bool SeSelection::processSelection(std::shared_ptr<SelectionResponse> selectionResponse) {
                    bool selectionSuccessful = false;

                    /* null pointer exception protection */
                    if (selectionResponse == nullptr) {
                        logger->error("selectionResponse shouldn't be null in processSelection.");
                        return false;
                    }

                    /* resets MatchingSe previous data */
                    for (auto matchingSe : matchingSeList) {
                        matchingSe->reset();
                    }
                    /* Check SeResponses */
                    std::vector<std::shared_ptr<MatchingSe>>::const_iterator matchingSeIterator = matchingSeList.begin();
                    for (auto seResponse : selectionResponse->getSelectionSeResponseSet()->getResponses()) {
                        if (seResponse != nullptr) {
                            /* test if the selection is successful: we should have either a FCI or an ATR */
                            if (seResponse->getSelectionStatus() != nullptr && seResponse->getSelectionStatus()->hasMatched()) {
                                /* at least one is successful */
                                selectionSuccessful = true;
                                /* update the matchingSe list */
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                if (matchingSeIterator != matchingSeList.end()) {
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                    std::shared_ptr<MatchingSe> matchingSe = *(++matchingSeIterator);
                                    matchingSe->setSelectionResponse(seResponse);
                                    if (matchingSe->isSelectable()) {
                                        selectedSe = matchingSe;
                                    }
                                }
                                else {
                                    throw std::make_shared<IllegalStateException>("The number of selection responses exceeds the number of prepared selectors.");
                                }
                            }
                            else {

//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                if (matchingSeIterator == matchingSeList.end()) {
                                    throw std::make_shared<IllegalStateException>("The number of selection responses exceeds the number of prepared selectors.");
                                }

//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                matchingSeIterator++;
                            }
                        }
                        else {
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                            if (matchingSeIterator != matchingSeList.end()) {
                                /* skip not matching response */
//JAVA TO C++ CONVERTER TODO TASK: Java iterators are only converted within the context of 'while' and 'for' loops:
                                matchingSeIterator++;
                            }
                            else {
                                throw std::make_shared<IllegalStateException>("The number of selection responses exceeds the number of prepared selectors.");
                            }
                        }
                    }
                    return selectionSuccessful;
                }

                bool SeSelection::processDefaultSelection(std::shared_ptr<SelectionResponse> selectionResponse) {
                    if (logger->isTraceEnabled()) {
                        logger->trace("Process default SELECTIONRESPONSE ({} response(s))", selectionResponse->getSelectionSeResponseSet()->getResponses().size());
                    }

                    return processSelection(selectionResponse);
                }

                bool SeSelection::processExplicitSelection() throw(KeypleReaderException) {
                    if (logger->isTraceEnabled()) {
                        logger->trace("Transmit SELECTIONREQUEST ({} request(s))", selectionRequestSet->getRequests()->size());
                    }

                    /* Communicate with the SE to do the selection */
                    std::shared_ptr<SeResponseSet> seResponseSet = (std::dynamic_pointer_cast<ProxyReader>(seReader))->transmitSet(selectionRequestSet);

                    return processSelection(std::make_shared<SelectionResponse>(seResponseSet));
                }

                std::shared_ptr<MatchingSe> SeSelection::getSelectedSe() {
                    return selectedSe;
                }

                std::vector<std::shared_ptr<MatchingSe>> SeSelection::getMatchingSeList() {
                    return matchingSeList;
                }

                std::shared_ptr<DefaultSelectionRequest> SeSelection::getSelectionOperation() {
                    return std::make_shared<DefaultSelectionRequest>(selectionRequestSet);
                }
            }
        }
    }
}

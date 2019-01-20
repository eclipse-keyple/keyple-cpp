#include "KeypleReaderException.h"
#include "MatchingSe.h"
#include "ProxyReader.h"
#include "SeRequest.h"
#include "SeReader.h"
#include "SelectionResponse.h"
#include "SelectionRequest.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "SeRequestSet.h"
#include "SeSelection.h"
#include "SeSelector.h"

#include "LoggerFactory.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace transaction {
                using SeReader = org::eclipse::keyple::seproxy::SeReader;
                using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;

                const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(SeSelection));

                SeSelection::SeSelection(std::shared_ptr<SeReader> seReader) : proxyReader(std::dynamic_pointer_cast<ProxyReader>(seReader)) {
                }

                std::shared_ptr<MatchingSe> SeSelection::prepareSelection(std::shared_ptr<SeSelector> seSelector) {
                    if (logger->isTraceEnabled()) {
                        logger->trace("SELECTORREQUEST = {}, EXTRAINFO = {}", seSelector->getSelectorRequest(), seSelector->getExtraInfo());
                    }
                    selectionRequestSet->insert(seSelector->getSelectorRequest());
                    std::shared_ptr<MatchingSe> matchingSe = nullptr;
/*
 * Alex: problem!
                    try {
                        std::shared_ptr<Constructor> constructor = seSelector->getMatchingClass().getConstructor(seSelector->getSelectorClass());
                        matchingSe = std::static_pointer_cast<MatchingSe>(constructor->newInstance(seSelector));
                        matchingSeList.push_back(matchingSe);
                    }
                    catch (NoSuchMethodException &e) {
                        e.printStackTrace();
                    }
                    catch (InstantiationException &e) {
                        e.printStackTrace();
                    }
                    catch (IllegalAccessException &e) {
                        e.printStackTrace();
                    }
                    catch (InvocationTargetException &e) {
                        e.printStackTrace();
                    }
*/
                    return matchingSe;
                }

                bool SeSelection::processSelection(std::shared_ptr<SelectionResponse> selectionResponse) {
                    bool selectionSuccessful = false;
                    /* Check SeResponses */
                    std::vector<std::shared_ptr<MatchingSe>>::const_iterator matchingSeIterator = matchingSeList.begin();
                    for (auto seResponse : selectionResponse->getSelectionSeResponseSet()->getResponses()) {
                        if (seResponse != nullptr) {
                            /* test if the selection is successful: we should have either a FCI or an ATR */
                            if (seResponse->getSelectionStatus()->hasMatched()) {
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
                        logger->trace("Transmit SELECTIONREQUEST ({} request(s))", selectionRequestSet->size());
                    }

                    /* Communicate with the SE to do the selection */
                    std::shared_ptr<SeResponseSet> seResponseSet = proxyReader->transmitSet(std::make_shared<SeRequestSet>(selectionRequestSet));

                    return processSelection(std::make_shared<SelectionResponse>(seResponseSet));
                }

                std::shared_ptr<MatchingSe> SeSelection::getSelectedSe() {
                    return selectedSe;
                }

                std::vector<std::shared_ptr<MatchingSe>> SeSelection::getMatchingSeList() {
                    return matchingSeList;
                }

                std::shared_ptr<SelectionRequest> SeSelection::getSelectionOperation() {
                    return std::make_shared<SelectionRequest>(std::make_shared<SeRequestSet>(selectionRequestSet));
                }
            }
        }
    }
}

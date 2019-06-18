#include <typeinfo>
#include <typeindex>

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

namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { class SeSelectionRequest; }}}}}

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace transaction {

using SeReader                = org::eclipse::keyple::core::seproxy::SeReader;
using DefaultSelectionRequest = org::eclipse::keyple::core::seproxy::event::DefaultSelectionRequest;
using SelectionResponse       = org::eclipse::keyple::core::seproxy::event::SelectionResponse;
using KeypleReaderException   = org::eclipse::keyple::core::seproxy::exception::KeypleReaderException;
using ProxyReader             = org::eclipse::keyple::core::seproxy::message::ProxyReader;
using SeRequest               = org::eclipse::keyple::core::seproxy::message::SeRequest;
using SeRequestSet            = org::eclipse::keyple::core::seproxy::message::SeRequestSet;
using SeResponse              = org::eclipse::keyple::core::seproxy::message::SeResponse;
using SeResponseSet           = org::eclipse::keyple::core::seproxy::message::SeResponseSet;

SeSelection::SeSelection(std::shared_ptr<SeReader> seReader) : seReader(seReader) {
}

std::shared_ptr<MatchingSe> SeSelection::prepareSelection(std::shared_ptr<SeSelectionRequest> seSelectionRequest)
{
    /*
     * Removed 'if (logger-isTraceEnabled())', that check will be done in the
     * trace function already.
     */
    logger->trace("SELECTORREQUEST = %s, EXTRAINFO = %s\n",
                  seSelectionRequest->getSelectionRequest(),
                  seSelectionRequest->getSeSelector()->getExtraInfo());

    selectionRequestSet->add(seSelectionRequest->getSelectionRequest());
    std::shared_ptr<MatchingSe> matchingSe = nullptr;

    try {
        const std::string matching = Logger::demangle(seSelectionRequest->getMatchingClass().name());
        const std::string selection = Logger::demangle(seSelectionRequest->getSelectionClass().name());
        logger->debug("matching class: %s\n", matching);
        logger->debug("selection class: %s\n", selection);

        /*
         * Problem, C++ cannot instanciate objects based on runtime-known
         * types. Let's go the hard way with exhaustive if/else until a better
         * option.
         *
         * Original code:
         * 
         * std::shared_ptr<Constructor> constructor = 
         *     seSelectionRequest->getMatchingClass().getConstructor(
         *                            seSelectionRequest->getSelectionClass());
         * matchingSe = std::static_pointer_cast<MatchingSe>(
         *                       constructor->newInstance(seSelectionRequest));
         */
        if (!matching.compare("org::eclipse::keyple::transaction::MatchingSe") &&
            !selection.compare("org::eclipse::keyple::seproxy::SeSelector")) {
            matchingSe = std::make_shared<MatchingSe>(seSelectionRequest);
        } else {
            throw std::runtime_error("matching/selector combination not handled yet");
        }

        matchingSeList.push_back(matchingSe);
    }
    catch (const NoSuchMethodException &e) {
        (void)e;
        //e.printStackTrace();
    }
    catch (const InstantiationException &e) {
        (void)e;
        //e.printStackTrace();
    }
    catch (const IllegalAccessException &e) {
        (void)e;
        //e.printStackTrace();
    }
    catch (const InvocationTargetException &e) {
        (void)e;
        //e.printStackTrace();
    }

    return matchingSe;
}

bool SeSelection::processSelection(std::shared_ptr<SelectionResponse> selectionResponse) {
    bool selectionSuccessful = false;

    /* null pointer exception protection */
    if (selectionResponse == nullptr) {
        logger->error("selectionResponse shouldn't be null in processSelection.\n");
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

bool SeSelection::processDefaultSelection(std::shared_ptr<SelectionResponse> selectionResponse)
{
    /*
     * Removed 'if (logger-isTraceEnabled())', that check will be done in the
     * trace function already.
     */
    logger->trace("Process default SELECTIONRESPONSE (%d response(s))\n",
                  selectionResponse->getSelectionSeResponseSet()->getResponses().size());

    return processSelection(selectionResponse);
}

bool SeSelection::processExplicitSelection()
{
    /*
     * Removed 'if (logger-isTraceEnabled())', that check will be done in the
     * trace function already.
     */
    logger->trace("Transmit SELECTIONREQUEST (%d request(s))\n",
                  selectionRequestSet->getRequests()->size());

    /* Communicate with the SE to do the selection */
    std::shared_ptr<SeResponseSet> seResponseSet = 
        (std::dynamic_pointer_cast<ProxyReader>(seReader))->transmitSet(selectionRequestSet);

    return processSelection(std::make_shared<SelectionResponse>(seResponseSet));
}

std::shared_ptr<MatchingSe> SeSelection::getSelectedSe()
{
    return selectedSe;
}

std::vector<std::shared_ptr<MatchingSe>> SeSelection::getMatchingSeList()
{
    return matchingSeList;
}

std::shared_ptr<DefaultSelectionRequest> SeSelection::getSelectionOperation()
{
    return std::make_shared<DefaultSelectionRequest>(selectionRequestSet);
}

}
}
}
}
}

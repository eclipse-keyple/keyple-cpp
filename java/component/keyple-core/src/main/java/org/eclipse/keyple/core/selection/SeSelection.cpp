#include "AbstractDefaultSelectionsResponse.h"
#include "AbstractDefaultSelectionsRequest.h"
#include "AbstractMatchingSe.h"
#include "AbstractSeSelectionRequest.h"
#include "DefaultSelectionsRequest.h"
#include "DefaultSelectionsResponse.h"
#include "KeypleReaderException.h"
#include "MatchingSelection.h"
#include "ProxyReader.h"
#include "SeSelection.h"
#include "SeRequest.h"
#include "SeRequestSet.h"
#include "SelectionsResult.h"
#include "SelectionStatus.h"
#include "SeReader.h"
#include "SeResponseSet.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace selection {

using SeReader                          = org::eclipse::keyple::core::seproxy::SeReader;
using AbstractDefaultSelectionsRequest  = org::eclipse::keyple::core::seproxy::event::AbstractDefaultSelectionsRequest;
using AbstractDefaultSelectionsResponse = org::eclipse::keyple::core::seproxy::event::AbstractDefaultSelectionsResponse;
using KeypleReaderException             = org::eclipse::keyple::core::seproxy::exception::KeypleReaderException;
using DefaultSelectionsRequest          = org::eclipse::keyple::core::seproxy::message::DefaultSelectionsRequest;
using DefaultSelectionsResponse         = org::eclipse::keyple::core::seproxy::message::DefaultSelectionsResponse;
using ProxyReader                       = org::eclipse::keyple::core::seproxy::message::ProxyReader;
using SelectionStatus                   = org::eclipse::keyple::core::seproxy::message::SelectionStatus;
using SeRequest                         = org::eclipse::keyple::core::seproxy::message::SeRequest;
using SeRequestSet                      = org::eclipse::keyple::core::seproxy::message::SeRequestSet;
using SeResponse                        = org::eclipse::keyple::core::seproxy::message::SeResponse;
using SeResponseSet                     = org::eclipse::keyple::core::seproxy::message::SeResponseSet;

SeSelection::SeSelection() {
    selectionIndex = 0;
}

int SeSelection::prepareSelection(std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest)
{

    logger->trace("SELECTORREQUEST = %s, EXTRAINFO = %s", seSelectionRequest->getSelectionRequest(),
                  seSelectionRequest->getSeSelector()->getExtraInfo());

    /* build the SeRequest set transmitted to the SE */
    selectionRequestSet->add(seSelectionRequest->getSelectionRequest());
    /* keep the selection request */
    seSelectionRequestList.push_back(seSelectionRequest);
    /* return and post increment the selection index */
    return selectionIndex++;
}

std::shared_ptr<SelectionsResult> SeSelection::processSelection(std::shared_ptr<DefaultSelectionsResponse> defaultSelectionsResponse)
{
    std::shared_ptr<SelectionsResult> selectionsResult = std::make_shared<SelectionsResult>();

    /* null pointer exception protection */
    if (defaultSelectionsResponse == nullptr) {
        logger->error("defaultSelectionsResponse shouldn't be null in processSelection.");
        return nullptr;
    }
    int selectionIndex = 0;

    /* Check SeResponses */
    for (auto seResponse : defaultSelectionsResponse->getSelectionSeResponseSet()->getResponses()) {
        if (seResponse != nullptr) {
            /* test if the selection is successful: we should have either a FCI or an ATR */
            if (seResponse->getSelectionStatus() != nullptr && seResponse->getSelectionStatus()->hasMatched()) {
                /*
                    * create a AbstractMatchingSe with the class deduced from the selection request
                    * during the selection preparation
                    */
                std::shared_ptr<AbstractMatchingSe> matchingSe = seSelectionRequestList[selectionIndex]->parse(seResponse);

                selectionsResult->addMatchingSelection(std::make_shared<MatchingSelection>(selectionIndex,
                                                       seSelectionRequestList[selectionIndex], matchingSe, seResponse));
            }
        }
        selectionIndex++;
    }
    return selectionsResult;
}

std::shared_ptr<SelectionsResult>
SeSelection::processDefaultSelection(std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse)
{
    if (logger->isTraceEnabled()) {
        logger->trace("Process default SELECTIONRESPONSE (%s response(s))",
                      (std::static_pointer_cast<DefaultSelectionsResponse>(defaultSelectionsResponse))->
                         getSelectionSeResponseSet()->getResponses().size());
    }

    return processSelection(std::static_pointer_cast<DefaultSelectionsResponse>(defaultSelectionsResponse));
}

std::shared_ptr<SelectionsResult> SeSelection::processExplicitSelection(std::shared_ptr<SeReader> seReader)
{
    /*
     * Removed 'if (logger-isTraceEnabled())', that check will be done in the
     * trace function already.
     */
    logger->trace("Transmit SELECTIONREQUEST (%d request(s))\n", selectionRequestSet->getRequests()->size());

    /* Communicate with the SE to do the selection */
    std::shared_ptr<SeResponseSet> seResponseSet = 
        (std::dynamic_pointer_cast<ProxyReader>(seReader))->transmitSet(selectionRequestSet);

    return processSelection(std::make_shared<DefaultSelectionsResponse>(seResponseSet));
}

std::shared_ptr<AbstractDefaultSelectionsRequest> SeSelection::getSelectionOperation()
{
    return std::static_pointer_cast<AbstractDefaultSelectionsRequest>(std::make_shared<DefaultSelectionsRequest>(selectionRequestSet));
}

}
}
}
}
}

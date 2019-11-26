/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include "AbstractDefaultSelectionsResponse.h"
#include "AbstractDefaultSelectionsRequest.h"
#include "AbstractMatchingSe.h"
#include "AbstractSeSelectionRequest.h"
#include "DefaultSelectionsRequest.h"
#include "DefaultSelectionsResponse.h"
#include "KeypleReaderException.h"
#include "KeypleIOReaderException.h"
#include "MatchingSelection.h"
#include "ProxyReader.h"
#include "SeSelection.h"
#include "SeRequest.h"
#include "SeRequestSet.h"
#include "SelectionsResult.h"
#include "SelectionStatus.h"
#include "SeReader.h"
#include "SeResponseSet.h"
#include "SeSelector_Import.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;

SeSelection::SeSelection()
{
    selectionIndex = 0;
}

int SeSelection::prepareSelection(
    std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest)
{

    logger->trace("SELECTORREQUEST = %s, EXTRAINFO = %s\n",
                  seSelectionRequest->getSelectionRequest()->toString().c_str(),
                  seSelectionRequest->getSeSelector()->getExtraInfo().c_str());

    /* build the SeRequest set transmitted to the SE */
    selectionRequestSet->add(seSelectionRequest->getSelectionRequest());

    /* keep the selection request */
    seSelectionRequestList.push_back(seSelectionRequest);

    /* return and post increment the selection index */
    return selectionIndex++;
}

std::shared_ptr<SelectionsResult> SeSelection::processSelection(
    std::shared_ptr<DefaultSelectionsResponse> defaultSelectionsResponse)
{

    int selectionIndex = 0;
    std::shared_ptr<SelectionsResult> selectionsResult =
        std::make_shared<SelectionsResult>();

    logger->debug("processSelection\n");

    /* null pointer exception protection */
    if (defaultSelectionsResponse == nullptr) {
        logger->error("defaultSelectionsResponse shouldn't be null in " \
                      "processSelection\n");
        return nullptr;
    }

    /* Check SeResponses */
    for (auto seResponse : defaultSelectionsResponse
                               ->getSelectionSeResponseSet()->getResponses()) {
        if (seResponse != nullptr) {
            /*
             * Test if the selection is successful: we should have either a FCI
             * or an ATR
             */
            if (seResponse->getSelectionStatus() != nullptr &&
                seResponse->getSelectionStatus()->hasMatched()) {
                /*
                 * Create a AbstractMatchingSe with the class deduced from the
                 * selection request during the selection preparation
                 */
                std::shared_ptr<AbstractMatchingSe> matchingSe =
                    seSelectionRequestList[selectionIndex]->parse(seResponse);

                selectionsResult->addMatchingSelection(
                    std::make_shared<MatchingSelection>(
                        selectionIndex, seSelectionRequestList[selectionIndex],
                        matchingSe, seResponse));
            }
        }
        selectionIndex++;
    }

    return selectionsResult;
}

std::shared_ptr<SelectionsResult> SeSelection::processDefaultSelection(
  std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse)
{
    logger->trace("Process default SELECTIONRESPONSE (%d response(s))\n",
                  (std::static_pointer_cast<DefaultSelectionsResponse>(
                      defaultSelectionsResponse))->
                          getSelectionSeResponseSet()->getResponses().size());

    return processSelection(
               std::static_pointer_cast<DefaultSelectionsResponse>(
                   defaultSelectionsResponse));
}

std::shared_ptr<SelectionsResult>
SeSelection::processExplicitSelection(std::shared_ptr<SeReader> seReader)
{
    /*
     * Removed 'if (logger-isTraceEnabled())', that check will be done in the
     * trace function already.
     */
    logger->trace("Transmit SELECTIONREQUEST (%d request(s))\n",
                  selectionRequestSet->getRequests()->size());

    if (!seReader) {
        logger->error("processExplicitSelection - seReader is null\n");
        return nullptr;
    }

    /* Communicate with the SE to do the selection */
    try {
        std::shared_ptr<ProxyReader> proxy =
                std::dynamic_pointer_cast<ProxyReader>(seReader);
        if (!proxy) {
            logger->error("processExplicitSelection - error retrieving " \
                          "ProxyReader\n");
            return nullptr;
        }

        std::shared_ptr<SeResponseSet> seResponseSet =
            proxy->transmitSet(selectionRequestSet);
        if (!seResponseSet) {
            logger->error("processExplicitSelection - error retrieving " \
                          "seResponseSet\n");
            return nullptr;
        }

        std::shared_ptr<DefaultSelectionsResponse> defaultSelectionResponse =
            std::make_shared<DefaultSelectionsResponse>(seResponseSet);
        if (!defaultSelectionResponse) {
            logger->error("processExplicitSelection - error casting "\
                          "seResponseSet into defaultSelectionResponse\n");
            return nullptr;
        }

        return processSelection(defaultSelectionResponse);

    } catch (const KeypleIOReaderException &ex) {
            throw ex;
    }
}

std::shared_ptr<AbstractDefaultSelectionsRequest>
SeSelection::getSelectionOperation()
{
    return std::static_pointer_cast<AbstractDefaultSelectionsRequest>(
               std::make_shared<DefaultSelectionsRequest>(selectionRequestSet));
}

}
}
}

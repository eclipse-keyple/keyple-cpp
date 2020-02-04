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
#include "SelectionsResult.h"
#include "SelectionStatus.h"
#include "SeReader.h"
#include "SeSelector_Import.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;

SeSelection::SeSelection(
  const MultiSeRequestProcessing multiSeRequestProcessing,
  const ChannelControl channelControl)
: selectionIndex(0), multiSeRequestProcessing(multiSeRequestProcessing),
  channelControl(channelControl)
{
}

SeSelection::SeSelection()
: SeSelection(MultiSeRequestProcessing::FIRST_MATCH, ChannelControl::KEEP_OPEN)
{
}

int SeSelection::prepareSelection(
    std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest)
{
    logger->trace("SELECTORREQUEST = %s, EXTRAINFO = %s\n",
                  seSelectionRequest->getSelectionRequest()->toString().c_str(),
                  seSelectionRequest->getSeSelector()->getExtraInfo().c_str());

    /* build the SeRequest set transmitted to the SE */
    selectionRequestSet.insert(seSelectionRequest->getSelectionRequest());

    /* keep the selection request */
    seSelectionRequestList.push_back(seSelectionRequest);

    /* return and post increment the selection index */
    return selectionIndex++;
}

std::shared_ptr<SelectionsResult> SeSelection::processSelection(
    std::shared_ptr<AbstractDefaultSelectionsResponse>
        defaultSelectionsResponse)
{
    int index = 0;

    std::shared_ptr<SelectionsResult> selectionsResult =
        std::make_shared<SelectionsResult>();

    /* Check SeResponses */
    for (auto seResponse :
             (std::dynamic_pointer_cast<DefaultSelectionsResponse>(
                 defaultSelectionsResponse))->getSelectionSeResponseSet()) {
        /*
         * Test if the selection is successful: we should have either a FCI
         * or an ATR
         */
        if (seResponse != nullptr &&
            seResponse->getSelectionStatus() != nullptr &&
            seResponse->getSelectionStatus()->hasMatched()) {
            /*
             * Create a AbstractMatchingSe with the class deduced from the
             * selection request during the selection preparation
             */
            std::shared_ptr<AbstractMatchingSe> matchingSe =
                seSelectionRequestList[index]->parse(seResponse);

            selectionsResult->addMatchingSelection(
                std::make_shared<MatchingSelection>(
                    index, seSelectionRequestList[index],
                    matchingSe, seResponse));
        }

        index++;
    }

    return selectionsResult;
}

std::shared_ptr<SelectionsResult> SeSelection::processDefaultSelection(
  std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse)
{
    /* Null pointer exception protection */
    if (defaultSelectionsResponse == nullptr) {
        logger->error("defaultSelectionsResponse shouldn't be null in " \
                      "processSelection\n");
        return nullptr;
    }

    logger->trace("Process default SELECTIONRESPONSE (%d response(s))\n",
                  (std::static_pointer_cast<DefaultSelectionsResponse>(
                      defaultSelectionsResponse))->
                          getSelectionSeResponseSet().size());

    return processSelection(defaultSelectionsResponse);
}

std::shared_ptr<SelectionsResult>
SeSelection::processExplicitSelection(std::shared_ptr<SeReader> seReader)
{
    /*
     * Removed 'if (logger-isTraceEnabled())', that check will be done in the
     * trace function already.
     */
    logger->trace("Transmit SELECTIONREQUEST (%d request(s))\n",
                  selectionRequestSet.size());

    if (!seReader) {
        logger->error("processExplicitSelection - seReader is null\n");
        return nullptr;
    }

    /* Communicate with the SE to do the selection */
    std::list<std::shared_ptr<SeResponse>> seResponseList =
        (std::dynamic_pointer_cast<ProxyReader>(seReader))
            ->transmitSet(selectionRequestSet, multiSeRequestProcessing,
                channelControl);

    return processSelection(
               std::make_shared<DefaultSelectionsResponse>(seResponseList));
}

std::shared_ptr<AbstractDefaultSelectionsRequest>
SeSelection::getSelectionOperation()
{
    return std::make_shared<DefaultSelectionsRequest>(
               selectionRequestSet, multiSeRequestProcessing, channelControl);
}

}
}
}

/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "SeSelection.h"

#include "AbstractDefaultSelectionsResponse.h"
#include "AbstractDefaultSelectionsRequest.h"
#include "AbstractMatchingSe.h"
#include "AbstractSeSelectionRequest.h"
#include "DefaultSelectionsRequest.h"
#include "DefaultSelectionsResponse.h"
#include "KeypleReaderException.h"
#include "KeypleReaderIOException.h"
#include "ProxyReader.h"
#include "SeRequest.h"
#include "SelectionsResult.h"
#include "SelectionStatus.h"
#include "SeReader.h"
#include "SeSelector.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;

SeSelection::SeSelection(MultiSeRequestProcessing multiSeRequestProcessing)
: mMultiSeRequestProcessing(multiSeRequestProcessing) {}

SeSelection::SeSelection() : SeSelection(MultiSeRequestProcessing::FIRST_MATCH) {}

int SeSelection::prepareSelection(
    std::shared_ptr<AbstractSeSelectionRequest<AbstractApduCommandBuilder>> seSelectionRequest)
{
    logger->trace("SELECTORREQUEST = %\n", seSelectionRequest->getSelectionRequest());

    /* keep the selection request */
    mSeSelectionRequests.push_back(seSelectionRequest);

    /* return and post increment the selection index (starting at 0) */
    return mSeSelectionRequests.size() - 1;
}

void SeSelection::prepareReleaseSeChannel()
{
    mChannelControl = ChannelControl::CLOSE_AFTER;
}

std::shared_ptr<SelectionsResult> SeSelection::processSelection(
   std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse)
{
    int index = 0;

    std::shared_ptr<SelectionsResult> selectionsResult =
        std::make_shared<SelectionsResult>();

    /* Check SeResponses */
    for (const auto& seResponse :
         (std::dynamic_pointer_cast<DefaultSelectionsResponse>(
              defaultSelectionsResponse))
             ->getSelectionSeResponses()) {
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
            const std::shared_ptr<AbstractMatchingSe> matchingSe =
                mSeSelectionRequests[index]->parse(seResponse);

            /* Determine if the current matching SE is selected */
            std::shared_ptr<SelectionStatus> selectionStatus =
                seResponse->getSelectionStatus();
            bool isSelected;
            if (selectionStatus)
                isSelected = selectionStatus->hasMatched() &&
                             seResponse->isLogicalChannelOpen();
            else
                isSelected = false;

            selectionsResult->addMatchingSe(index, matchingSe, isSelected);
        }

        index++;
    }

    return selectionsResult;
}

std::shared_ptr<SelectionsResult> SeSelection::processDefaultSelection(
    const std::shared_ptr<AbstractDefaultSelectionsResponse>
        defaultSelectionsResponse)
{
    /* Null pointer exception protection */
    if (defaultSelectionsResponse == nullptr) {
        return nullptr;
    }

    logger->trace("Process default SELECTIONRESPONSE (% response(s))\n",
                  (std::static_pointer_cast<DefaultSelectionsResponse>(
                       defaultSelectionsResponse))
                      ->getSelectionSeResponses().size());

    return processSelection(defaultSelectionsResponse);
}

std::shared_ptr<SelectionsResult> SeSelection::processExplicitSelection(
    std::shared_ptr<SeReader> seReader)
{
    std::vector<std::shared_ptr<SeRequest>> selectionRequests;

    for (const auto& seSelectionRequest : mSeSelectionRequests)
        selectionRequests.push_back(seSelectionRequest->getSelectionRequest());

    logger->trace("Transmit SELECTIONREQUEST (% request(s))\n",
                  selectionRequests.size());

    if (!seReader) {
        logger->error("processExplicitSelection - seReader is null\n");
        return nullptr;
    }

    /* Communicate with the SE to do the selection */
    auto proxyReader = std::dynamic_pointer_cast<ProxyReader>(seReader);
    std::vector<std::shared_ptr<SeResponse>> seResponses =
        proxyReader->transmitSeRequests(selectionRequests,
                                        mMultiSeRequestProcessing,
                                        mChannelControl);

    return processSelection(
        std::make_shared<DefaultSelectionsResponse>(seResponses));
}

std::shared_ptr<AbstractDefaultSelectionsRequest>
SeSelection::getSelectionOperation()
{
    std::vector<std::shared_ptr<SeRequest>> selectionRequests;

    for (const auto& seSelectionRequest : mSeSelectionRequests)
        selectionRequests.push_back(seSelectionRequest->getSelectionRequest());

    return std::make_shared<DefaultSelectionsRequest>(
               selectionRequests, mMultiSeRequestProcessing,
               mChannelControl);
}

std::ostream& operator<<(std::ostream& os, const SeSelection& ss)
{
    (void)ss;

    os << "SESELECTION: {"
       << "TODO"
       << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::shared_ptr<SeSelection>& ss)
{
    if (!ss)
        os << "SESELECTION = null";
    else
        os << *ss.get();

    return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::unique_ptr<SeSelection>& ss)
{
    if (!ss)
        os << "SESELECTION = null";
    else
        os << *ss.get();

    return os;
}

}
}
}

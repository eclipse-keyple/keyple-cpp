/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <vector>
#include <memory>

#include "AbstractDefaultSelectionsRequest.h"
#include "AbstractDefaultSelectionsResponse.h"
#include "AbstractSeSelectionRequest.h"
#include "DefaultSelectionsResponse.h"
#include "KeypleReaderException.h"
#include "SelectionsResult.h"
#include "SeReader.h"
#include "SeRequest.h"
#include "SeRequestSet.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace selection {

using SeReader                          = org::eclipse::keyple::core::seproxy::SeReader;
using AbstractDefaultSelectionsRequest  = org::eclipse::keyple::core::seproxy::event::AbstractDefaultSelectionsRequest;
using AbstractDefaultSelectionsResponse = org::eclipse::keyple::core::seproxy::event::AbstractDefaultSelectionsResponse;
using KeypleReaderException             = org::eclipse::keyple::core::seproxy::exception::KeypleReaderException;
using DefaultSelectionsResponse         = org::eclipse::keyple::core::seproxy::message::DefaultSelectionsResponse;
using SeRequest                         = org::eclipse::keyple::core::seproxy::message::SeRequest;
using SeRequestSet                      = org::eclipse::keyple::core::seproxy::message::SeRequestSet;
using Logger                            = org::eclipse::keyple::common::Logger;
using LoggerFactory                     = org::eclipse::keyple::common::LoggerFactory;

/**
    * The SeSelection class handles the SE selection process.
    * <p>
    * It provides a way to do explicit SE selection or to post process a default SE selection.
    */
class SeSelection final : public std::enable_shared_from_this<SeSelection> {
private:
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(SeSelection));

    /*
        * list of target classes and selection requests used to build the AbstractMatchingSe list in
        * return of processSelection methods
        */
    std::vector<std::shared_ptr<AbstractSeSelectionRequest>> seSelectionRequestList = std::vector<std::shared_ptr<AbstractSeSelectionRequest>>();
    const std::shared_ptr<SeRequestSet> selectionRequestSet       = std::make_shared<SeRequestSet>(std::make_shared<std::set<std::shared_ptr<SeRequest>>>());
    int selectionIndex = 0;

    /**
        * Initializes the SeSelection
        */
public:
    SeSelection();

    /**
        * Prepare a selection: add the selection request from the provided selector to the selection
        * request set.
        * <p>
        *
        * @param seSelectionRequest the selector to prepare
        * @return the selection index giving the current selection position in the selection request.
        */
    int prepareSelection(std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest);

    /**
        * Process the selection response either from a
        * {@link org.eclipse.keyple.core.seproxy.event.ReaderEvent} (default selection) or from an
        * explicit selection.
        * <p>
        * The responses from the {@link SeResponseSet} is parsed and checked.
        * <p>
        * A {@link AbstractMatchingSe} list is build and returned. Non matching SE are signaled by a
        * null element in the list
        * 
        * @param defaultSelectionsResponse the selection response
        * @return the {@link SelectionsResult} containing the result of all prepared selection cases,
        *         including {@link AbstractMatchingSe} and {@link SeResponse}.
        */
private:
    std::shared_ptr<SelectionsResult> processSelection(std::shared_ptr<DefaultSelectionsResponse> defaultSelectionsResponse);

    /**
        * Parses the response to a selection operation sent to a SE and return a list of
        * {@link AbstractMatchingSe}
        * <p>
        * Selection cases that have not matched the current SE are set to null.
        *
        * @param defaultSelectionsResponse the response from the reader to the
        *        {@link DefaultSelectionsRequest}
        * @return the {@link SelectionsResult} containing the result of all prepared selection cases,
        *         including {@link AbstractMatchingSe} and {@link SeResponse}.
        */
public:
    std::shared_ptr<SelectionsResult> processDefaultSelection(std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse);

    /**
        * Execute the selection process and return a list of {@link AbstractMatchingSe}.
        * <p>
        * Selection requests are transmitted to the SE through the supplied SeReader.
        * <p>
        * The process stops in the following cases:
        * <ul>
        * <li>All the selection requests have been transmitted</li>
        * <li>A selection request matches the current SE and the keepChannelOpen flag was true</li>
        * </ul>
        * <p>
        *
        * @param seReader the SeReader on which the selection is made
        * @return the {@link SelectionsResult} containing the result of all prepared selection cases,
        *         including {@link AbstractMatchingSe} and {@link SeResponse}.
        * @throws KeypleReaderException if the requests transmission failed
        */
    std::shared_ptr<SelectionsResult> processExplicitSelection(std::shared_ptr<SeReader> seReader);

    /**
        * The SelectionOperation is the DefaultSelectionsRequest to process in ordered to select a SE
        * among others through the selection process. This method is useful to build the prepared
        * selection to be executed by a reader just after a SE insertion.
        * 
        * @return the {@link DefaultSelectionsRequest} previously prepared with prepareSelection
        */
    std::shared_ptr<AbstractDefaultSelectionsRequest> getSelectionOperation();
};

}
}
}
}
}

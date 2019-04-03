#pragma once

#include <set>
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

#include <vector>
#include "exceptionhelper.h"
#include <memory>

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

/* Core */
#include "KeypleReaderException.h"
#include "DefaultSelectionRequest.h"
#include "ProxyReader.h"
#include "MatchingSe.h"
#include "SeRequest.h"
#include "SeReader.h"
#include "KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace transaction {

                using SeReader                = org::eclipse::keyple::seproxy::SeReader;
                using DefaultSelectionRequest = org::eclipse::keyple::seproxy::event::DefaultSelectionRequest;
                using SelectionResponse       = org::eclipse::keyple::seproxy::event::SelectionResponse;
                using KeypleReaderException   = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                using SeRequest               = org::eclipse::keyple::seproxy::message::SeRequest;
                using LoggerFactory           = org::eclipse::keyple::common::LoggerFactory;
                using Logger                  = org::eclipse::keyple::common::Logger;
                using SeRequestSet            = org::eclipse::keyple::seproxy::message::SeRequestSet;

                /**
                 * The SeSelection class handles the SE selection process.
                 * <p>
                 * It provides a way to do explicit SE selection or to post process a default SE selection.
                 */
class SeSelection final : public std::enable_shared_from_this<SeSelection> {
  private:
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(SeSelection));

                    const std::shared_ptr<SeReader> seReader;
    std::vector<std::shared_ptr<MatchingSe>> matchingSeList = std::vector<std::shared_ptr<MatchingSe>>();
                    std::shared_ptr<SeRequestSet> selectionRequestSet = std::make_shared<SeRequestSet>(std::make_shared<std::set<std::shared_ptr<SeRequest>>>());
    std::shared_ptr<MatchingSe> selectedSe;

    /**
                     * Initializes the SeSelection
                     *
                     * @param seReader the reader to use to make the selection
                     */
  public:
    SeSelection(std::shared_ptr<SeReader> seReader);

    /**
                     * Prepare a selection: add the selection request from the provided selector to the selection
                     * request set.
                     * <p>
                     * Create a MatchingSe, retain it in a list and return it. The MatchingSe may be an extended
                     * class
                     *
                     * @param seSelectionRequest the selector to prepare
                     * @return a MatchingSe for further information request about this selector
                     */
                    std::shared_ptr<MatchingSe> prepareSelection(std::shared_ptr<SeSelectionRequest> seSelectionRequest);

                    /**
                     * Process the selection response either from a
                     * {@link org.eclipse.keyple.seproxy.event.ReaderEvent} (default selection) or from an explicit
                     * selection.
                     * <p>
                     * The responses from the {@link SeResponseSet} is parsed and checked.
                     * <p>
                     * If one of the responses has matched, the corresponding {@link MatchingSe} is updated with the
                     * data from the response.
                     * <p>
                     * If the updated {@link MatchingSe} is selectable (logical channel requested to be kept open)
                     * the selectedSe field is updated (making the MatchingSe available through getSelectedSe).
                     *
                     * @param selectionResponse the selection response
                     * @return true if a successful selection has been made.
                     */
  private:
    bool processSelection(std::shared_ptr<SelectionResponse> selectionResponse);

    /**
                     * Parses the response to a selection operation sent to a SE and sets the selectedSe if any
                     * <p>
                     * The returned boolean indicates if at least one response was successful.
                     * <p>
                     * If one of the response also corresponds to a request for which the channel has been asked to
                     * be kept open, then it is retained as a selection answer.
                     * <p>
                     * Responses that have not matched the current SE are set to null.
                     *
                     * @param selectionResponse the response from the reader to the {@link DefaultSelectionRequest}
                     * @return boolean true if a SE was selected
                     */
  public:
    bool processDefaultSelection(std::shared_ptr<SelectionResponse> selectionResponse);

    /**
                     * Execute the selection process.
                     * <p>
                     * The selection requests are transmitted to the SE.
                     * <p>
                     * The process stops in the following cases:
                     * <ul>
                     * <li>All the selection requests have been transmitted</li>
                     * <li>A selection request matches the current SE and the keepChannelOpen flag was true</li>
                     * </ul>
                     * <p>
                     * The returned boolean indicates if at least one response was successful.
                     * <p>
                     * If one of the response also corresponds to a request for which the channel has been asked to
                     * be kept open, then it is retained as a selection answer.
                     * <p>
                     * Responses that have not matched the current PO are set to null.
                     *
                     * @return boolean true if a SE was selected
                     * @throws KeypleReaderException if the requests transmission failed
                     */
    bool processExplicitSelection() throw(KeypleReaderException);

    /**
                     * Returns the {@link MatchingSe} if there is one, null if not
                     *
                     * @return a {@link MatchingSe} or null
                     */
    std::shared_ptr<MatchingSe> getSelectedSe();

    /**
                     * Returns the updated list of prepared {@link MatchingSe} updated with the responses to the
                     * selection requests sent.
                     *
                     * @return a list of {@link MatchingSe}
                     */
    std::vector<std::shared_ptr<MatchingSe>> getMatchingSeList();

    /**
                     * The SelectionOperation is the DefaultSelectionRequest to process in ordered to select a SE
                     * among others through the selection process. This method is useful to build the prepared
                     * selection to be executed by a reader just after a SE insertion.
                     *
                     * @return the {@link DefaultSelectionRequest} previously prepared with prepareSelection
                     */
                    std::shared_ptr<DefaultSelectionRequest> getSelectionOperation();
};

} // namespace transaction
} // namespace keyple
} // namespace eclipse
} // namespace org

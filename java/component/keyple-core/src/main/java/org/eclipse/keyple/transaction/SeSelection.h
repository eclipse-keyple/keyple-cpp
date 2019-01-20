#pragma once

#include <set>
#include <vector>
#include "exceptionhelper.h"
#include <memory>

#include "KeypleReaderException.h"
#include "Logger.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ProxyReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class MatchingSe; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeRequest; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SeSelector; } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SelectionResponse; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SelectionRequest; } } } }

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
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace transaction {

                using SeReader = org::eclipse::keyple::seproxy::SeReader;
                using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;

                /**
                 * The SeSelection class handles the SE selection process
                 */
                class SeSelection final : public std::enable_shared_from_this<SeSelection> {
                private:
                    const std::shared_ptr<Logger> logger;

                    const std::shared_ptr<ProxyReader> proxyReader;
                    std::vector<std::shared_ptr<MatchingSe>> matchingSeList = std::vector<std::shared_ptr<MatchingSe>>();
                    std::shared_ptr<std::set<std::shared_ptr<SeRequest>>> selectionRequestSet = std::make_shared<std::set<std::shared_ptr<SeRequest>>>();
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
                     * @param seSelector the selector to prepare
                     * @return a MatchingSe for further information request about this selector
                     */
                    std::shared_ptr<MatchingSe> prepareSelection(std::shared_ptr<SeSelector> seSelector);

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
                     * @param selectionResponse the response from the reader to the {@link SelectionRequest}
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
                     * The SelectionOperation is the SelectionRequest to process in ordered to select a SE among
                     * others through the selection process. This method is useful to build the prepared selection
                     * to be executed by a reader just after a SE insertion.
                     *
                     * @return the {@link SelectionRequest} previously prepared with prepareSelection
                     */
                    std::shared_ptr<SelectionRequest> getSelectionOperation();
                };

            }
        }
    }
}

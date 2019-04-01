#pragma once

#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeResponse; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SeSelectionRequest; } } } }

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

                using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;

                /**
                 * MatchingSe is the class to manage the elements of the result of a selection.
                 *
                 */
                class MatchingSe : public std::enable_shared_from_this<MatchingSe> {
                private:
                    const bool channelIsKeptOpen;
                    const std::string extraInfo;
                    std::shared_ptr<SeResponse> selectionSeResponse;

                    /**
                     * Constructor taking a SeSelector as an argument. Keeps the isKeepChannelOpen flag and the
                     * extraInfo for later usage.
                     *
                     * @param seSelectionRequest the SE selection request
                     */
                public:
                    MatchingSe(std::shared_ptr<SeSelectionRequest> seSelectionRequest);

                    /**
                     * Sets the SeResponse obtained in return from the selection process
                     *
                     * @param selectionResponse the selection SeResponse
                     */
                    virtual void setSelectionResponse(std::shared_ptr<SeResponse> selectionResponse);

                    /**
                     * Gets the SeResponse obtained in return from the selection process
                     *
                     * @return the selection SeResponse
                     */
                    std::shared_ptr<SeResponse> getSelectionSeResponse();

                    /**
                     * Indicates whether the current SE is eligible to application selection in preparation for a
                     * transaction.
                     * <p>
                     * The SE will only be eligible if the logical channel is required to be kept open after the
                     * selection process.
                     *
                     * @return true or false
                     */
                public:
                    bool isSelectable();

                    /**
                     * Indicates whether the current SE has been identified as selected: the logical channel is open
                     * and the selection process returned either a FCI or an ATR
                     *
                     * @return true or false
                     */
                public:
                    bool isSelected();

                    /**
                     * Gets back the information string provided in the constructor for information purposes (logs)
                     *
                     * @return a string
                     */
                    std::string getExtraInfo();

                    /**
                     * Restore the initial state of the MatchingSe.
                     * <p>
                     * Called by SeSelection at the beginning of the processing of a selection
                     * <p>
                     * This method should be overloaded by the objects derived from MatchingSe in order to reset
                     * their additional attributes.
                     */
                protected:
                    virtual void reset();
                };

            }
        }
    }
}

#pragma once

#include "MatchingSe.h"
#include "../seproxy/ChannelState.h"
#include <string>
#include <vector>
#include <typeinfo>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeSelector; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ApduRequest; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace protocol { class SeProtocol; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeRequest; } } } } }

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

                using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                using SeSelector = org::eclipse::keyple::seproxy::SeSelector;
                using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                using org::slf4j::Logger;
                using org::slf4j::LoggerFactory;

                /**
                 * The SeSelectionRequest class combines a SeSelector with additional helper methods useful to the
                 * selection process done in {@link SeSelection}.
                 * <p>
                 * This class may also be extended to add particular features specific to a SE family.
                 */
                class SeSelectionRequest : public std::enable_shared_from_this<SeSelectionRequest> {
                private:
                    static const std::shared_ptr<Logger> logger;

                    std::shared_ptr<SeSelector> seSelector;
                    std::type_info matchingClass = MatchingSe::typeid;
                    std::type_info selectionClass = SeSelectionRequest::typeid;
                    /** optional apdu requests list to be executed following the selection process */
                    const std::vector<std::shared_ptr<ApduRequest>> seSelectionApduRequestList = std::vector<std::shared_ptr<ApduRequest>>();
                    /**
                     * the channelState and protocolFlag may be accessed from derived classes. Let them with the
                     * protected access level.
                     */
                protected:
                    const ChannelState channelState;
                    const std::shared_ptr<SeProtocol> protocolFlag;

                public:
                    SeSelectionRequest(std::shared_ptr<SeSelector> seSelector, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag);

                    /**
                     * Returns a selection SeRequest built from the information provided in the constructor and
                     * possibly completed with the seSelectionApduRequestList
                     *
                     * @return the selection SeRequest
                     */
                protected:
                    std::shared_ptr<SeRequest> getSelectionRequest();

                public:
                    virtual std::shared_ptr<SeSelector> getSeSelector();

                    /**
                     * The matchingClass is the MatchingSe class or one of its extensions
                     * <p>
                     * It is used in SeSelection to determine what kind of MatchingSe is to be instantiated.
                     *
                     * This method must be called in the classes that extend SeSelector in order to specify the
                     * expected class derived from MatchingSe in return to the selection process.
                     *
                     * @param matchingClass the expected class for this SeSelector
                     */
                protected:
                    void setMatchingClass(std::type_info matchingClass);

                    /**
                     * The selectionClass is the SeSelector class or one of its extensions
                     * <p>
                     * It is used in SeSelection to determine what kind of SeSelector is to be used as argument to
                     * the matchingClass constructor.
                     *
                     * This method must be called in the classes that extend SeSelector in order to specify the
                     * expected class derived from SeSelector used as an argument to derived form of MatchingSe.
                     *
                     * @param selectionClass the argument for the constructor of the matchingClass
                     */
                    void setSelectionClass(std::type_info selectionClass);

                    /**
                     * The default value for the selectionClass (unless setSelectionClass is used) is
                     * SeSelector.class
                     * 
                     * @return the current selectionClass
                     */
                    std::type_info getSelectionClass();

                    /**
                     * The default value for the matchingClass (unless setMatchingClass is used) is MatchingSe.class
                     *
                     * @return the current matchingClass
                     */
                    std::type_info getMatchingClass();

                    /**
                     * Add an additional {@link ApduRequest} to be executed after the selection process if it
                     * succeeds.
                     * <p>
                     * If more than one {@link ApduRequest} is added, all will be executed in the order in which
                     * they were added.
                     * 
                     * @param apduRequest an {@link ApduRequest}
                     */
                    void addApduRequest(std::shared_ptr<ApduRequest> apduRequest);

                public:
                    std::string toString() override;
                };

            }
        }
    }
}

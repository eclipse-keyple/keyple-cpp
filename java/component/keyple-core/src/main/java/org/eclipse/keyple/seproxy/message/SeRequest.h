#pragma once

#include "../../transaction/SeSelector.h"
#include "../protocol/Protocol.h"
#include "../ChannelState.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include "stringhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SeSelector; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ApduRequest; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace protocol { class SeProtocol; } } } } }

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
            namespace seproxy {
                namespace message {

                    using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                    using Protocol = org::eclipse::keyple::seproxy::protocol::Protocol;
                    using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using SeSelector = org::eclipse::keyple::transaction::SeSelector;

                    /**
                     * List of APDU requests that will result in a {@link SeResponse}
                     * 
                     * @see SeResponse
                     */
                    class SeRequest final : public std::enable_shared_from_this<SeRequest>, public Serializable {

                    public:
                        static constexpr long long serialVersionUID = 6018469841127325812LL;

                        /**
                         * The Selector inner class is dedicated to handle the selection of the SE either through a
                         * selection command with AID (AtrSelector) or through a matching test between the SE ATR and a
                         * regular expression (AtrSelector).
                         *
                         */
                    public:
                        class Selector : public std::enable_shared_from_this<Selector> {
                        };

                    public:
                        class AidSelector final : public SeRequest::Selector {

                        public:
                            static constexpr int AID_MIN_LENGTH = 5;
                            static constexpr int AID_MAX_LENGTH = 16;
                        protected:
                            SeSelector::SelectMode selectMode = SeSelector::SelectMode::FIRST;

                            /**
                             * - AID’s bytes of the SE application to select. In case the SE application is currently
                             * not selected, a logical channel is established and the corresponding SE application is
                             * selected by the SE reader, otherwise keep the current channel.
                             *
                             * - Could be missing when operating SE which don’t support the Select Application command
                             * (as it is the case for SAM).
                             */
                        private:
                            std::vector<char> aidToSelect;

                            /**
                             * AID based selector
                             * 
                             * @param aidToSelect byte array
                             */
                        public:
                            AidSelector(std::vector<char> &aidToSelect);

                            /**
                             * AID based selector with selection mode
                             * <p>
                             * The selectMode parameter defines the selection options P2 of the SELECT command message
                             * <ul>
                             * <li>false: first or only occurrence</li>
                             * <li>true: next occurrence</li>
                             * </ul>
                             * 
                             * @param aidToSelect byte array
                             * @param selectMode selection mode FIRST or NEXT
                             */
                            AidSelector(std::vector<char> &aidToSelect, SeSelector::SelectMode selectMode);

                            /**
                             * Getter for the AID provided at construction time
                             * 
                             * @return byte array containing the AID
                             */
                            std::vector<char> getAidToSelect();

                            /**
                             * Indicates whether the selection command is targeting the first or the next occurrence
                             * 
                             * @return true or false
                             */
                            bool isSelectNext();

                            /**
                             * Print out the AID in hex
                             * 
                             * @return a string
                             */
                            std::string toString();

protected:
                            std::shared_ptr<AidSelector> shared_from_this() {
                                return std::static_pointer_cast<AidSelector>(SeRequest.Selector::shared_from_this());
                            }
                        };

                    public:
                        class AtrSelector final : public SeRequest::Selector {
                            /**
                             * Regular expression dedicated to handle SE logical channel opening based on ATR pattern
                             */
                        private:
                            std::string atrRegex;

                            /**
                             * ATR based selection
                             *
                             * @param atrRegex String hex regular expression
                             */
                        public:
                            AtrSelector(const std::string &atrRegex);

                            /**
                             * Getter for the regular expression provided at construction time
                             *
                             * @return Regular expression string
                             */
                            std::string getAtrRegex();

                            /**
                             * Tells if the provided ATR matches the registered regular expression
                             *
                             * If the registered regular expression is empty, the ATR is always matching.
                             *
                             * @param atr a buffer containing the ATR to be checked
                             * @return a boolean true the ATR matches the current regex
                             */
                            bool atrMatches(std::vector<char> &atr);

                            /**
                             * Print out the ATR regex
                             * 
                             * @return a string
                             */
                            std::string toString();

protected:
                            std::shared_ptr<AtrSelector> shared_from_this() {
                                return std::static_pointer_cast<AtrSelector>(SeRequest.Selector::shared_from_this());
                            }
                        };

                        /**
                         * SE selector is either an AID or an ATR regular expression
                         */
                    private:
                        const std::shared_ptr<Selector> selector;

                        /**
                         * List of status codes in response to the select application command that should be considered
                         * successful although they are different from 9000
                         */
                        std::shared_ptr<Set<Integer>> successfulSelectionStatusCodes = std::make_shared<LinkedHashSet<Integer>>();

                        /**
                         * contains a group of APDUCommand to operate on the selected SE application by the SE reader.
                         */
                        std::vector<std::shared_ptr<ApduRequest>> apduRequests;


                        /**
                         * the protocol flag is used to target specific SE technologies for a given request
                         */
                        std::shared_ptr<SeProtocol> protocolFlag = Protocol::ANY;

                        /**
                         * the final logical channel status: the SE reader may kept active the logical channel of the SE
                         * application after processing the group of APDU commands otherwise the SE reader will close
                         * the logical channel of the SE application after processing the group of APDU commands (i.e.
                         * after the receipt of the last APDU response).
                         */
                        ChannelState channelState = static_cast<ChannelState>(0);

                        /**
                         * The constructor called by a ProxyReader in order to open a logical channel, to send a set of
                         * APDU commands to a SE application, or both of them.
                         * <ul>
                         * <li>For SE requiring an AID based selection, the Selector should be defined with a non null
                         * byte array.</li>
                         * <li>For SE requiring an ATR based selection, the Selector should be defined with a non null
                         * String regular expression.</li>
                         * <li>For SE supporting neither AID selection nor ATR selection, the Selector should be defined
                         * as null.</li>
                         * <li>The protocolFlag parameter is optional.</li>
                         * </ul>
                         *
                         * @param selector the SE selector
                         * @param apduRequests the apdu requests
                         * @param channelState the keep channel open
                         * @param protocolFlag the expected protocol
                         * @param successfulSelectionStatusCodes a list of successful status codes for the select
                         *        application command
                         */
                    public:
                        SeRequest(std::shared_ptr<Selector> selector, std::vector<std::shared_ptr<ApduRequest>> &apduRequests, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag, std::shared_ptr<Set<Integer>> successfulSelectionStatusCodes);

                        /**
                         * Constructor to be used when the SE is already selected
                         * 
                         * @param apduRequests a list of ApudRequest
                         * @param channelState a flag to tell if the channel has to be closed at the end
                         */
                        SeRequest(std::vector<std::shared_ptr<ApduRequest>> &apduRequests, ChannelState channelState);


                        /**
                         * Gets the SE selector.
                         *
                         * @return the current SE selector
                         */
                        std::shared_ptr<Selector> getSelector();

                        /**
                         * Gets the apdu requests.
                         *
                         * @return the group of APDUs to be transmitted to the SE application for this instance of
                         *         SERequest.
                         */
                        std::vector<std::shared_ptr<ApduRequest>> getApduRequests();

                        /**
                         * Define if the channel should be kept open after the the {@link SeRequestSet} has been
                         * executed.
                         *
                         * @return If the channel should be kept open
                         */
                        bool isKeepChannelOpen();

                        /**
                         * Gets the protocol flag of the request
                         * 
                         * @return protocolFlag
                         */
                        std::shared_ptr<SeProtocol> getProtocolFlag();

                        /**
                         * Gets the list of successful selection status codes
                         * 
                         * @return the list of status codes
                         */
                        std::shared_ptr<Set<Integer>> getSuccessfulSelectionStatusCodes();

                        std::string toString() override;
                    };

                }
            }
        }
    }
}

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

#include <set>
#include "MatchingSe.h"
#include "../seproxy/ChannelState.h"
#include <string>
#include <unordered_set>
#include <vector>
#include <typeindex>
#include <memory>

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

/* Forward class declarations */
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {
                    class ApduRequest;
                }
            } // namespace seproxy
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace protocol {
                    class SeProtocol;
                }
            } // namespace seproxy
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {
                    class SeRequest;
                }
            } // namespace seproxy
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace transaction {

                using ChannelState  = org::eclipse::keyple::seproxy::ChannelState;
                using ApduRequest   = org::eclipse::keyple::seproxy::message::ApduRequest;
                using SeRequest     = org::eclipse::keyple::seproxy::message::SeRequest;
                using SeProtocol    = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                using Logger        = org::eclipse::keyple::common::Logger;
                using LoggerFactory = org::eclipse::keyple::common::LoggerFactory;

                /**
                 * The SeSelector class groups the information and methods used to select a particular secure
                 * element
                 */
                class SeSelector : public std::enable_shared_from_this<SeSelector> {
                    /** logger */
                private:
                    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(SeSelector));

                    const std::shared_ptr<AidSelector> aidSelector;
                    const std::shared_ptr<AtrFilter> atrFilter;
                    const std::string extraInfo;

                    /**
                     * Static nested class to hold the data elements used to perform an AID based selection
                     */
                public:
                    class AidSelector : public std::enable_shared_from_this<AidSelector> {

                    /**
                         * FileOccurrence indicates how to carry out the file occurrence in accordance with
                         * ISO7816-4
                         * <p>
                         * For now only FIRST and NEXT options are supported
                     */
                  public:
                        enum class FileOccurrence {
                        FIRST,
                            LAST,
                            NEXT,
                            PREVIOUS
                    };

                        /**
                         * FileOccurrence indicates how to which template is expected in accordance with ISO7816-4
                         * <p>
                         * For now only FCI option is supported
                         */
                    public:
                        enum class FileControlInformation {
                            FCI,
                            FCP,
                            FMD,
                            NO_RESPONSE
                        };

                    public:
                        static constexpr int AID_MIN_LENGTH = 5;
                        static constexpr int AID_MAX_LENGTH = 16;
                  private:
                        FileOccurrence fileOccurrence = FileOccurrence::FIRST;
                        FileControlInformation fileControlInformation = FileControlInformation::FCI;

                        /**
                         * - AID’s bytes of the SE application to select. In case the SE application is currently
                         * not selected, a logical channel is established and the corresponding SE application is
                         * selected by the SE reader, otherwise keep the current channel.
                         *
                         * - Could be missing when operating SE which don’t support the Select Application command
                         * (as it is the case for SAM).
                         */
                        std::vector<char> aidToSelect;

                        /**
                         * List of status codes in response to the select application command that should be
                         * considered successful although they are different from 9000
                         */
                        std::shared_ptr<Set<Integer>> successfulSelectionStatusCodes = std::make_shared<LinkedHashSet<Integer>>();

                        /**
                         * AidSelector with additional select application successful status codes, file occurrence
                         * and file control information.
                         * <p>
                         * The fileOccurrence parameter defines the selection options P2 of the SELECT command
                         * message
                         * <p>
                         * The fileControlInformation parameter defines the expected command output template.
                         * <p>
                         * Refer to ISO7816-4.2 for detailed information about these parameters
                         *
                         * @param aidToSelect byte array
                         * @param successfulSelectionStatusCodes list of successful status codes for the select
                         *        application response
                         */
                  public:
                        AidSelector(std::vector<char> &aidToSelect, std::shared_ptr<Set<Integer>> successfulSelectionStatusCodes, FileOccurrence fileOccurrence, FileControlInformation fileControlInformation);

                        /**
                         * AidSelector with additional select application successful status codes
                         * <p>
                         * The fileOccurrence field is set by default to FIRST
                         * <p>
                         * The fileControlInformation field is set by default to FCI
                         *
                         * @param aidToSelect byte array
                         * @param successfulSelectionStatusCodes list of successful status codes for the select
                         *        application response
                         */
                        AidSelector(std::vector<char> &aidToSelect, std::shared_ptr<Set<Integer>> successfulSelectionStatusCodes);

                        /**
                         * Getter for the AID provided at construction time
                         *
                         * @return byte array containing the AID
                         */
                        virtual std::vector<char> getAidToSelect();

                        /**
                         * Indicates whether the selection command is targeting the first or the next occurrence
                         *
                         * @return true or false
                         */
                        virtual bool isSelectNext();

                    /**
                         * Gets the list of successful selection status codes
                         *
                         * @return the list of status codes
                     */
                        virtual std::shared_ptr<Set<Integer>> getSuccessfulSelectionStatusCodes();


                    /**
                         * Print out the AID in hex
                         *
                         * @return a string
                     */
                        virtual std::string toString();
                    };

                    /**
                     * Static nested class to hold the data elements used to perform an ATR based filtering
                     */
                public:
                    class AtrFilter : public std::enable_shared_from_this<AtrFilter> {
                        /**
                         * Regular expression dedicated to handle SE logical channel opening based on ATR pattern
                         */
                    private:
                        std::string atrRegex;

                    /**
                         * Regular expression based filter
                     *
                         * @param atrRegex String hex regular expression
                     */
                    public:
                        AtrFilter(const std::string &atrRegex);

                    /**
                         * Setter for the regular expression provided at construction time
                     *
                         * @param atrRegex expression string
                     */
                        virtual void setAtrRegex(const std::string &atrRegex);

                    /**
                         * Getter for the regular expression provided at construction time
                     *
                         * @return Regular expression string
                     */
                        virtual std::string getAtrRegex();

                    /**
                         * Tells if the provided ATR matches the registered regular expression
                     *
                         * If the registered regular expression is empty, the ATR is always matching.
                     *
                         * @param atr a buffer containing the ATR to be checked
                         * @return a boolean true the ATR matches the current regex
                     */
                        virtual bool atrMatches(std::vector<char> &atr);

                    /**
                         * Print out the ATR regex
                         *
                         * @return a string
                         */
                        virtual std::string toString();
                    };

                    /**
                     * Create a SeSelector to perform the SE selection
                     * <p>
                     * if aidSelector is null, no 'select application' command is generated. In this case the SE
                     * must have a default application selected. (e.g. SAM or Rev1 Calypso cards)
                     * <p>
                     * if aidSelector is not null, a 'select application' command is generated and performed.
                     * Furthermore, the status code is checked against the list of successful status codes in the
                     * {@link AidSelector} to determine if the SE matched or not the selection data.
                     * <p>
                     * if atrFilter is null, no check of the ATR is performed. All SE will match.
                     * <p>
                     * if atrFilter is not null, the ATR of the SE is compared with the regular expression provided
                     * in the {@link AtrFilter} in order to determine if the SE match or not the expected ATR.
                     *
                     * @param aidSelector the AID selection data
                     * @param atrFilter the ATR filter
                     * @param extraInfo information string (to be printed in logs)
                     */
                public:
                    SeSelector(std::shared_ptr<AidSelector> aidSelector, std::shared_ptr<AtrFilter> atrFilter, const std::string &extraInfo);

                    /**
                     * Getter
                     *
                     * @return the {@link AidSelector} provided at construction time
                     */
                    virtual std::shared_ptr<AidSelector> getAidSelector();

                    /**
                     * Getter
                     *
                     * @return the {@link AtrFilter} provided at construction time
                     */
                    virtual std::shared_ptr<AtrFilter> getAtrFilter();

                    /**
                     * Gets the information string
                     *
                     * @return a string to be printed in logs
                     */
                    std::string getExtraInfo();

                    std::string toString() override;
                };

            } // namespace seproxy
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org

#pragma once

#include <set>
#include "MatchingSe.h"
#include "../seproxy/ChannelState.h"
#include <string>
#include <unordered_set>
#include <vector>
#include <typeinfo>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
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
                using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
//                using org::slf4j::Logger;
//                using org::slf4j::LoggerFactory;

                /**
                 * The SeSelector class groups the information and methods used to select a particular secure
                 * element
                 */
                class SeSelector : public std::enable_shared_from_this<SeSelector> {
                    /**
                     * SelectMode indicates how to carry out the application selection in accordance with ISO7816-4
                     */
                public:
                    enum class SelectMode {
                        FIRST,
                        NEXT
                    };

                private:
//                    static const std::shared_ptr<Logger> logger;

                protected:
                    std::vector<std::shared_ptr<ApduRequest>> seSelectionApduRequestList;
                    std::set<int> selectApplicationSuccessfulStatusCodes;
                private:
                    std::type_info matchingClass;
                    std::type_info selectorClass;
                    const ChannelState channelState;
                    const std::shared_ptr<SeProtocol> protocolFlag;
                    const std::string atrRegex;
                    std::vector<char> const aid;
                    const SelectMode selectMode;
                    const bool selectionByAid;
                    std::string extraInfo;

                public:
                    virtual std::string getAtrRegex();

                    virtual std::vector<char> getAid();

                    virtual SelectMode getSelectMode();

                    virtual bool isSelectionByAid();

                    /**
                     * Instantiate a SeSelector object with the selection data (atrRegex), dedicated to select a SE
                     * that does not support the select application command, the channel management after the
                     * selection and the protocol flag to possibly target a specific protocol
                     *
                     * @param atrRegex a regular expression to compare with the ATR of the targeted SE
                     * @param channelState flag to tell if the logical channel should be left open at the end of the
                     *        selection
                     * @param protocolFlag flag to be compared with the protocol identified when communicating the
                     *        SE
                     * @param extraInfo information string (to be printed in logs)
                     */
                public:
                    SeSelector(const std::string &atrRegex, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag, const std::string &extraInfo);

                    /**
                     * Instantiate a SeSelector object with the selection data (AID), dedicated to select a SE that
                     * does not support the select application command, the channel management after the selection
                     * and the protocol flag to possibly target a specific protocol
                     *
                     * @param aid the target AID (end bytes can be truncated)
                     * @param selectMode a flag to indicate if the first or the next occurrence is requested (see
                     *        ISO7816-4 for a complete description of the select next mechanism)
                     * @param channelState flag to tell if the logical channel should be left open at the end of the
                     *        selection
                     * @param protocolFlag flag to be compared with the protocol identified when communicating the
                     *        SE
                     * @param extraInfo information string (to be printed in logs)
                     */
                    SeSelector(std::vector<char> &aid, SelectMode selectMode, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag, const std::string &extraInfo);

                    /**
                     * @return the protocolFlag defined by the constructor
                     */
                    std::shared_ptr<SeProtocol> getProtocolFlag();

                    /**
                     * Sets the list of ApduRequest to be executed following the selection operation at once
                     *
                     * @param seSelectionApduRequestList the list of requests
                     */
                    void setSelectionApduRequestList(std::vector<std::shared_ptr<ApduRequest>> &seSelectionApduRequestList);

                    /**
                     * Returns a selection SeRequest built from the information provided in the constructor and
                     * possibly completed with the seSelectionApduRequestList
                     *
                     * @return the selection SeRequest
                     */
                    std::shared_ptr<SeRequest> getSelectorRequest();

                    /**
                     * Gets the information string
                     *
                     * @return a string to be printed in logs
                     */
                public:
                    std::string getExtraInfo();

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
                     * The selectorClass is the SeSelector class or one of its extensions
                     * <p>
                     * It is used in SeSelection to determine what kind of SeSelector is to be used as argument to
                     * the matchingClass constructor.
                     *
                     * This method must be called in the classes that extend SeSelector in order to specify the
                     * expected class derived from SeSelector used as an argument to derived form of MatchingSe.
                     *
                     * @param selectorClass the argument for the constructor of the matchingClass
                     */
                    void setSelectorClass(std::type_info selectorClass);

                    /**
                     * The default value for the matchingClass (unless setMatchingClass is used) is MatchingSe.class
                     *
                     * @return the current matchingClass
                     */
                    std::type_info getMatchingClass();

                public:
                    /**
                     * The default value for the selectorClass (unless setSelectorClass is used) is SeSelector.class
                     *
                     * @return the current selectorClass
                     */
                    std::type_info getSelectorClass();
                };

            }
        }
    }
}

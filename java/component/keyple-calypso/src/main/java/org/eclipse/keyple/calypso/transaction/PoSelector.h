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

#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

/* Core */
#include "SeSelector.h"
#include "ChannelState.h"
#include "AbstractApduResponseParser.h"

/* Calypso */
#include "PoClass.h"
#include "ReadDataStructure.h"
#include "ReadRecordsRespPars.h"
#include "SelectFileRespPars.h"

/* Common */
#include "exceptionhelper.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {



                    using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                    using ReadDataStructure = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
                    using ReadRecordsRespPars = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
                    using SelectFileRespPars = org::eclipse::keyple::calypso::command::po::parser::SelectFileRespPars;
                    using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                    using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                    using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                    using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                    using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using SeSelector = org::eclipse::keyple::transaction::SeSelector;
                    using org::eclipse::keyple::common::Logger;
                    using org::eclipse::keyple::common::LoggerFactory;

                    /**
                     * Specialized selector to manage the specific characteristics of Calypso POs
                     */
                    class PoSelector final : public SeSelector {
                    private:
                        static const std::shared_ptr<Logger> logger;

                        const PoClass poClass;
                        const std::shared_ptr<SeProtocol> protocolFlag;

                        /** The list to contain the parsers associated to the prepared commands */
                        std::vector<std::shared_ptr<AbstractApduResponseParser>> poResponseParserList = std::vector<std::shared_ptr<AbstractApduResponseParser>>();

                        /**
                         * Calypso PO revision 1 selector
                         * <p>
                         * The PO class is set to LEGACY in order to produce Apdus with legacy class byte
                         * 
                         * @param atrRegex a regular expression to compare with the ATR of the targeted Rev1 PO
                         * @param channelState indicates whether the logical channel should remain open
                         * @param protocolFlag the protocol flag to filter POs according to their communication protocol
                         * @param extraInfo information string
                         */
                    public:
                        PoSelector(const std::string &atrRegex, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag, const std::string &extraInfo);

                        /**
                         * Calypso PO revision 2+ selector
                         * <p>
                         * The PO class is set to ISO in order to produce Apdus with ISO class byte
                         *
                         * @param aid a regular expression to compare with the ATR of the targeted Rev1 PO
                         * @param selectMode a flag to indicate if the first or the next occurrence is requested
                         * @param channelState indicates whether the logical channel should remain open
                         * @param protocolFlag the protocol flag to filter POs according to their communication protocol
                         * @param extraInfo information string
                         */
                        PoSelector(std::vector<char> &aid, SelectMode selectMode, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag, const std::string &extraInfo);

                        /**
                         * Prepare one or more read record ApduRequest based on the target revision to be executed
                         * following the selection.
                         * <p>
                         * In the case of a mixed target (rev2 or rev3) two commands are prepared. The first one in rev3
                         * format, the second one in rev2 format (mainly class byte)
                         * 
                         * @param sfi the sfi top select
                         * @param readDataStructureEnum read mode enum to indicate a SINGLE, MULTIPLE or COUNTER read
                         * @param firstRecordNumber the record number to read (or first record to read in case of
                         *        several records)
                         * @param expectedLength the expected length of the record(s)
                         * @param extraInfo extra information included in the logs (can be null or empty)
                         */
                    private:
                        std::shared_ptr<ReadRecordsRespPars> prepareReadRecordsCmdInternal(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, int expectedLength, const std::string &extraInfo);

                        /**
                         * Prepare one or more read record ApduRequest based on the target revision to be executed
                         * following the selection.
                         * <p>
                         * The expected length is provided and its value is checked between 1 and 250.
                         * <p>
                         * In the case of a mixed target (rev2 or rev3) two commands are prepared. The first one in rev3
                         * format, the second one in rev2 format (mainly class byte)
                         *
                         * @param sfi the sfi top select
                         * @param readDataStructureEnum read mode enum to indicate a SINGLE, MULTIPLE or COUNTER read
                         * @param firstRecordNumber the record number to read (or first record to read in case of
                         *        several records)
                         * @param expectedLength the expected length of the record(s)
                         * @param extraInfo extra information included in the logs (can be null or empty)
                         */
                    public:
                        std::shared_ptr<ReadRecordsRespPars> prepareReadRecordsCmd(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, int expectedLength, const std::string &extraInfo);

                        /**
                         * Prepare one or more read record ApduRequest based on the target revision to be executed
                         * following the selection. No expected length is specified, the record output length is handled
                         * automatically.
                         * <p>
                         * In the case of a mixed target (rev2 or rev3) two commands are prepared. The first one in rev3
                         * format, the second one in rev2 format (mainly class byte)
                         *
                         * @param sfi the sfi top select
                         * @param readDataStructureEnum read mode enum to indicate a SINGLE, MULTIPLE or COUNTER read
                         * @param firstRecordNumber the record number to read (or first record to read in case of
                         *        several records)
                         * @param extraInfo extra information included in the logs (can be null or empty)
                         */
                        std::shared_ptr<ReadRecordsRespPars> prepareReadRecordsCmd(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, const std::string &extraInfo);

                        /**
                         * Prepare a select file ApduRequest to be executed following the selection.
                         * <p>
                         * 
                         * @param path path from the MF (MF identifier excluded)
                         * @param extraInfo extra information included in the logs (can be null or empty)
                         */
                        std::shared_ptr<SelectFileRespPars> prepareSelectFileDfCmd(std::vector<char> &path, const std::string &extraInfo);

                        /**
                         * Prepare a custom read ApduRequest to be executed following the selection.
                         * 
                         * @param name the name of the command (will appear in the ApduRequest log)
                         * @param apduRequest the ApduRequest (the correct instruction byte must be provided)
                         */
                        void preparePoCustomReadCmd(const std::string &name, std::shared_ptr<ApduRequest> apduRequest);

                        /**
                         * Prepare a custom modification ApduRequest to be executed following the selection.
                         *
                         * @param name the name of the command (will appear in the ApduRequest log)
                         * @param apduRequest the ApduRequest (the correct instruction byte must be provided)
                         */
                        void preparePoCustomModificationCmd(const std::string &name, std::shared_ptr<ApduRequest> apduRequest);


                        /**
                         * Loops on the SeResponse and updates the list of parsers previously memorized
                         *
                         * @param seResponse the seResponse from the PO
                         */
                    protected:
                        void updateParsersWithResponses(std::shared_ptr<SeResponse> seResponse);

protected:
                        std::shared_ptr<PoSelector> shared_from_this() {
                            return std::static_pointer_cast<PoSelector>(SeSelector::shared_from_this());
                        }
                    };

                }
            }
        }
    }
}

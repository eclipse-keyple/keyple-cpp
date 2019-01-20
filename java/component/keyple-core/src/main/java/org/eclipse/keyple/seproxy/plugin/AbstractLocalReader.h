#pragma once

#include "AbstractObservableReader.h"
#include "../event/ObservableReader.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "exceptionhelper.h"
#include <memory>
#include "SelectionStatus.h"
#include "ByteArrayUtils.h"
#include "System.h"
#include "KeypleApplicationSelectionException.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class Selector; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace protocol { class SeProtocol; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace event { class ObservableReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SelectionRequest; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleIOReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace protocol { class SeProtocolSetting; } } } } }

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
                namespace plugin {

                    using ObservableReader = org::eclipse::keyple::seproxy::event::ObservableReader;
                    using KeypleApplicationSelectionException = org::eclipse::keyple::seproxy::exception::KeypleApplicationSelectionException;
                    using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using namespace org::eclipse::keyple::seproxy::message;
                    using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                    using SelectionRequest = org::eclipse::keyple::transaction::SelectionRequest;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                    using SelectionStatus = org::eclipse::keyple::seproxy::message::SelectionStatus;
//                    using org::slf4j::Logger;
//                    using org::slf4j::LoggerFactory;


                    /**
                     * Manage the loop processing for SeRequest transmission in a set and for SeResponse reception in a
                     * set
                     */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings({"PMD.TooManyMethods", "PMD.CyclomaticComplexity"}) public abstract class AbstractLocalReader extends AbstractObservableReader
                    class AbstractLocalReader : public AbstractObservableReader {

                    private:
                        const std::shared_ptr<Logger> logger;
                        static std::vector<char> getResponseHackRequestBytes;
                        bool logicalChannelIsOpen = false;
                        std::vector<char> aidCurrentlySelected;
                        std::shared_ptr<SelectionStatus> currentSelectionStatus;
                        bool presenceNotified = false;
                        long long before = 0; // timestamp recorder

                    public:
                        AbstractLocalReader(const std::string &pluginName, const std::string &readerName);

                        /**
                         * Open (if needed) a physical channel (try to connect a card to the terminal, attempt to select
                         * the application)
                         *
                         * @param selector the SE Selector: either the AID of the application to select or an ATR
                         *        selection regular expression
                         * @param successfulSelectionStatusCodes the list of successful status code for the select
                         *        command
                         * @return a {@link SelectionStatus} object containing the SE ATR, the SE FCI and a flag giving
                         *         the selection process result. When ATR or FCI are not available, they are set to null
                         * @throws KeypleReaderException if a reader error occurs
                         * @throws KeypleApplicationSelectionException if the application selection fails
                         */
                    protected:
                        virtual std::shared_ptr<SelectionStatus> openLogicalChannelAndSelect(std::shared_ptr<SeRequest::Selector> selector, std::shared_ptr<std::set<int>> successfulSelectionStatusCodes) = 0;

                        /**
                         * Closes the current physical channel.
                         *
                         * @throws KeypleChannelStateException if a reader error occurs
                         */
                        virtual void closePhysicalChannel() = 0;

                        /**
                         * Transmits a single APDU and receives its response. The implementation of this abstract method
                         * must handle the case where the SE response is 61xy and execute the appropriate get response
                         * command
                         *
                         * @param apduIn byte buffer containing the ingoing data
                         * @return apduResponse byte buffer containing the outgoing data.
                         * @throws KeypleIOReaderException if the transmission fails
                         */
                        virtual std::vector<char> transmitApdu(std::vector<char> &apduIn) = 0;

                        /**
                         * Test if the current protocol matches the flag
                         *
                         * @param protocolFlag the protocol flag
                         * @return true if the current protocol matches the provided protocol flag
                         * @throws KeypleReaderException in case of a reader exception
                         */
                        virtual bool protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) = 0;

                        /**
                         * If defined, the prepared setDefaultSelectionRequest will be processed as soon as a SE is
                         * inserted. The result of this request set will be added to the reader event.
                         * <p>
                         * Depending on the notification mode, the observer will be notified whenever an SE is inserted,
                         * regardless of the selection status, or only if the current SE matches the selection criteria.
                         *
                         * @param defaultSelectionRequest the {@link SelectionRequest} to be executed when a SE is
                         *        inserted
                         * @param notificationMode the notification mode enum (ALWAYS or MATCHED_ONLY)
                         */
                    public:
                        virtual void setDefaultSelectionRequest(std::shared_ptr<SelectionRequest> defaultSelectionRequest, ObservableReader::NotificationMode notificationMode) override;

                        /**
                         * This method is invoked when a SE is removed
                         * <p>
                         * The SE will be notified removed only if it has been previously notified present
                         */
                    protected:
                        virtual void cardRemoved();

                        /**
                         * This method is invoked when a SE is inserted
                         * <p>
                         * It will fire an ReaderEvent in the following cases:
                         * <ul>
                         * <li>SE_INSERTED: if no default selection request was defined</li>
                         * <li>SE_MATCHED: if a default selection request was defined in any mode a SE matched the
                         * selection</li>
                         * <li>SE_INSERTED: if a default selection request was defined in ALWAYS mode but no SE matched
                         * the selection (the SelectionResponse is however transmitted)</li>
                         * </ul>
                         * <p>
                         * It will do nothing if a default selection is defined in MATCHED_ONLY mode but no SE matched
                         * the selection.
                         */
                        virtual void cardInserted();

                        /**
                         * Transmits an ApduRequest and receives the ApduResponse with time measurement.
                         *
                         * @param apduRequest APDU request
                         * @return APDU response
                         * @throws KeypleIOReaderException Exception faced
                         */
                        std::shared_ptr<ApduResponse> processApduRequest(std::shared_ptr<ApduRequest> apduRequest) throw(KeypleIOReaderException);

                        /**
                         * Execute a get response command in order to get outgoing data from specific cards answering
                         * 9000 with no data although the command has outgoing data. Note that this method relies on the
                         * right get response management by transmitApdu
                         *
                         * @param originalStatusCode the status code of the command that didn't returned data
                         * @return ApduResponse the response to the get response command
                         * @throws KeypleIOReaderException if the transmission fails.
                         */
                    private:
                        std::shared_ptr<ApduResponse> case4HackGetResponse(int originalStatusCode) throw(KeypleIOReaderException);


                        /**
                         * Do the transmission of all needed requestSet requests contained in the provided requestSet
                         * according to the protocol flag selection logic. The responseSet responses are returned in the
                         * responseSet object. The requestSet requests are ordered at application level and the
                         * responses match this order. When a requestSet is not matching the current PO, the responseSet
                         * responses pushed in the responseSet object is set to null.
                         *
                         * @param requestSet the request set
                         * @return SeResponseSet the response set
                         * @throws KeypleIOReaderException if a reader error occurs
                         */
                    protected:
                        std::shared_ptr<SeResponseSet> processSeRequestSet(std::shared_ptr<SeRequestSet> requestSet) throw(KeypleReaderException) final override;

                        /**
                         * Tells if a logical channel is open
                         *
                         * @return true if the logical channel is open
                         */
                        bool isLogicalChannelOpen();

                        void closeLogicalChannel();

                    private:
                        void setLogicalChannelOpen();

                        /**
                         * Executes a request made of one or more Apdus and receives their answers. The selection of the
                         * application is handled.
                         *
                         * @param seRequest the SeRequest
                         * @return the SeResponse to the SeRequest
                         * @throws KeypleReaderException if a transmission fails
                         */
                    protected:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings({"PMD.ModifiedCyclomaticComplexity", "PMD.CyclomaticComplexity", "PMD.StdCyclomaticComplexity", "PMD.NPathComplexity", "PMD.ExcessiveMethodLength"}) protected final SeResponse processSeRequest(SeRequest seRequest) throws IllegalStateException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        std::shared_ptr<SeResponse> processSeRequest(std::shared_ptr<SeRequest> seRequest) throw(IllegalStateException, KeypleReaderException) final override;

                        /**
                         * PO selection map associating seProtocols and selection strings (e.g. ATR regex for Pcsc
                         * plugins)
                         */
                        std::unordered_map<std::shared_ptr<SeProtocol>, std::string> protocolsMap = std::unordered_map<std::shared_ptr<SeProtocol>, std::string>();

                    public:
                        virtual void addSeProtocolSetting(std::shared_ptr<SeProtocolSetting> seProtocolSetting) override;

protected:
/*
                        std::shared_ptr<AbstractLocalReader> shared_from_this() {
                            return std::static_pointer_cast<AbstractLocalReader>(AbstractObservableReader::shared_from_this());
                        }
*/
                    };

                }
            }
        }
    }
}

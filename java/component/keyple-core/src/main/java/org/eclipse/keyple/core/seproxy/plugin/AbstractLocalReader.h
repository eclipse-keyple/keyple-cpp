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

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "exceptionhelper.h"

/* Common */
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "System.h"

/* Core */
#include "AbstractDefaultSelectionsRequest.h"
#include "AbstractObservableReader.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "KeypleApplicationSelectionException.h"
#include "ObservableReader.h"
#include "SelectionStatus.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using AbstractDefaultSelectionsRequest    = org::eclipse::keyple::core::seproxy::event::AbstractDefaultSelectionsRequest;
using ApduResponse                        = org::eclipse::keyple::core::seproxy::message::ApduResponse;
using ObservableReader                    = org::eclipse::keyple::core::seproxy::event::ObservableReader;
using KeypleApplicationSelectionException = org::eclipse::keyple::core::seproxy::exception::KeypleApplicationSelectionException;
using KeypleChannelStateException         = org::eclipse::keyple::core::seproxy::exception::KeypleChannelStateException;
using KeypleIOReaderException             = org::eclipse::keyple::core::seproxy::exception::KeypleIOReaderException;
using KeypleReaderException               = org::eclipse::keyple::core::seproxy::exception::KeypleReaderException;
using SeProtocol                          = org::eclipse::keyple::core::seproxy::protocol::SeProtocol;
using SeProtocolSetting                   = org::eclipse::keyple::core::seproxy::protocol::SeProtocolSetting;
using ByteArrayUtil                       = org::eclipse::keyple::core::util::ByteArrayUtil;
using SelectionStatus                     = org::eclipse::keyple::core::seproxy::message::SelectionStatus;
using Logger                              = org::eclipse::keyple::common::Logger;
using LoggerFactory                       = org::eclipse::keyple::common::LoggerFactory;

/**
    * Manage the loop processing for SeRequest transmission in a set and for SeResponse reception in a
    * set
    */
class EXPORT AbstractLocalReader : public AbstractObservableReader {

    /** logger */
private:
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(AbstractLocalReader));

    /** predefined "get response" byte array */
    std::vector<char> getResponseHackRequestBytes;

    /** logical channel status flag */
    bool logicalChannelIsOpen = false;

                            bool forceGetDataFlag = false;

    /** current AID if any */
                            std::shared_ptr<SeSelector::AidSelector::IsoAid> aidCurrentlySelected;

    /** current selection status */
    std::shared_ptr<SelectionStatus> currentSelectionStatus;

    /** notification status flag used to avoid redundant notifications */
    bool presenceNotified = false;

    /** Timestamp recorder */
    long long before = 0;

    /** ==== Constructor =================================================== */

    /**
        * Reader constructor
        * <p>
        * Force the definition of a name through the use of super method.
        * <p>
        * Initialize the time measurement
        *
        * @param pluginName the name of the plugin that instantiated the reader
        * @param readerName the name of the reader
        */
public:
    AbstractLocalReader(const std::string &pluginName, const std::string &readerName);

    /** ==== Card presence management ====================================== */

    /**
        * Check the presence of a SE
        * <p>
        * This method is recommended for non-observable readers.
        * <p>
        * When the card is not present the logical and physical channels status may be refreshed
        * through a call to the cardRemoved method.
        *
        * @return true if the SE is present
        */
    bool isSePresent() override;

    /**
        * Wrapper for the native method of the plugin specific local reader to verify the presence of
        * the SE.
        * <p>
        * This method must be implemented by the ProxyReader plugin (e.g. Pcsc reader plugin).
        * <p>
        * This method is invoked by isSePresent.
        *
        * @return true if the SE is present
        * @throws NoStackTraceThrowable exception without stack trace
        */
protected:
    virtual bool checkSePresence()
    { 
        return false;
    }

    /**
        * This method is invoked when a SE is inserted in the case of an observable reader.
        * <p>
        * e.g. from the monitoring thread in the case of a Pcsc plugin
        * ({@link AbstractSelectionLocalReader}) or from the NfcAdapter callback method onTagDiscovered
        * in the case of a Android NFC plugin.
        * <p>
        * It will fire an ReaderEvent in the following cases:
        * <ul>
        * <li>SE_INSERTED: if no default selection request was defined</li>
        * <li>SE_MATCHED: if a default selection request was defined in any mode and a SE matched the
        * selection</li>
        * <li>SE_INSERTED: if a default selection request was defined in ALWAYS mode but no SE matched
                             * the selection (the DefaultSelectionsResponse is however transmitted)</li>
        * </ul>
        * <p>
        * It will do nothing if a default selection is defined in MATCHED_ONLY mode but no SE matched
        * the selection.
        */
    void cardInserted();

    /**
        * This method is invoked when a SE is removed in the case of an observable reader
        * ({@link AbstractThreadedLocalReader}).
        * <p>
        * It will also be invoked if isSePresent is called and at least one of the physical or logical
        * channels is still open (case of a non-observable reader)
        * <p>
        * The SE will be notified removed only if it has been previously notified present (observable
        * reader only)
        */
    void cardRemoved();

    /** ==== Physical and logical channels management ====================== */

    /**
        * This abstract method must be implemented by the derived class in order to provide the SE ATR
        * when available.
        * <p>
        * Gets the SE Answer to reset
        *
        * @return ATR returned by the SE or reconstructed by the reader (contactless)
        */
    virtual std::vector<char> getATR() = 0;


                            /**
                             * This abstract method must be implemented by the derived class in order to proceed to the
                             * application selection
                             * <p>
                             * Gets application selection data according to
                             * {@link org.eclipse.keyple.core.seproxy.SeSelector.AidSelector} attributes.
                             *
                             * @return a ApduResponse containing the FCI or similar data output from selection application
                             * @throws KeypleIOReaderException if a reader error occurs
                             */
                            virtual std::shared_ptr<ApduResponse> openChannelForAid(std::shared_ptr<SeSelector::AidSelector> aidSelector) = 0;


                            /**
                             * This method is dedicated to the case where no FCI data is available in return for the select
                             * command.
                             * <p>
                             * We force here selection without response and proceed to a get data command to get the
                             * expected FCI.
                             * 
                             * @param aidSelector
                             * @return a ApduResponse containing the FCI
                             */
                        private:
                            std::shared_ptr<ApduResponse> openChannelForAidHackGetData(std::shared_ptr<SeSelector::AidSelector> aidSelector);


                            /**
                             * Set the flag that enables the execution of the Get Data hack to get the FCI
                             * <p>
                             * This method should called by the reader plugins that need this specific behavior (ex. OMAPI)
                             *
                             * <p>
                             * The default value for the forceGetDataFlag is false, thus only specific readers plugins
                             * should call this method.
                             * 
                             * @param forceGetDataFlag true or false
                             */
                        protected:
                            virtual void setForceGetDataFlag(bool forceGetDataFlag);

    /**
        * This abstract method must be implemented by the derived class in order to provide a selection
        * and ATR filtering mechanism.
        * <p>
        * The Selector provided in argument holds all the needed data to handle the Application
        * Selection and ATR matching process and build the resulting SelectionStatus.
        *
        * @param seSelector the SE selector
                             * @return the SelectionStatus
                             */
                            /** ==== ATR filtering and application selection by AID ================ */

                            /**
                             * Build a select application command, transmit it to the SE and deduct the SelectionStatus.
                             *
                             * @param seSelector the targeted application SE selector
        * @return the SelectionStatus containing the actual selection result (ATR and/or FCI and the
        *         matching status flag).
                             * @throws KeypleIOReaderException if a reader error occurs
        */
    virtual std::shared_ptr<SelectionStatus> openLogicalChannel(std::shared_ptr<SeSelector> seSelector) = 0;


/**
        * Open (if needed) a physical channel and try to establish a logical channel.
        * <p>
        * The logical opening is done either by sending a Select Application command (AID based
        * selection) or by checking the current ATR received from the SE (ATR based selection).
        * <p>
        * If the selection is successful, the logical channel is considered open. On the contrary, if
        * the selection fails, the logical channel remains closed.
        * <p>
        * This method relies on the abstracts methods openLogicalChannelByAtr and
        * openLogicalChannelByAid implemented either by {@link AbstractSelectionLocalReader} or by any
        * other derived class that provides a SE selection mechanism (e.g. OmapiReader).
        *
        * @param seSelector the SE Selector: either the AID of the application to select or an ATR
        *        selection regular expression
        * @return a {@link SelectionStatus} object containing the SE ATR, the SE FCI and a flag giving
        *         the selection process result. When ATR or FCI are not available, they are set to null
        *         but they can't be both null at the same time.
        * @throws KeypleIOReaderException if a reader error occurs
        * @throws KeypleApplicationSelectionException if the application selection fails
        */
    std::shared_ptr<SelectionStatus> openLogicalChannelAndSelect(std::shared_ptr<SeSelector> seSelector);

    /**
        * Attempts to open the physical channel
        *
        * @throws KeypleChannelStateException if the channel opening fails
        */
    virtual void openPhysicalChannel() = 0;

    /**
        * Closes the current physical channel.
        * <p>
        * This method must be implemented by the ProxyReader plugin (e.g. Pcsc/Nfc/Omapi Reader).
        *
        * @throws KeypleChannelStateException if a reader error occurs
        */
    virtual void closePhysicalChannel() = 0;
 
    /**
        * Tells if the physical channel is open or not
        * <p>
        * This method must be implemented by the ProxyReader plugin (e.g. Pcsc/Nfc/Omapi Reader).
        *
        * @return true is the channel is open
        */
    virtual bool isPhysicalChannelOpen() = 0;

    /**
        * Tells if a logical channel is open
        *
        * @return true if the logical channel is open
        */
public:
    bool isLogicalChannelOpen();

    /**
        * Close the logical channel.
        */
                        private:
    void closeLogicalChannel();

    /** ==== Protocol management =========================================== */

    /**
        * PO selection map associating seProtocols and selection strings.
        * <p>
        * The String associated with a particular protocol can be anything that is relevant to be
        * interpreted by reader plugins implementing protocolFlagMatches (e.g. ATR regex for Pcsc
        * plugins, technology name for Nfc plugins, etc).
        */
protected:
    std::unordered_map<std::shared_ptr<SeProtocol>, std::string> protocolsMap;

    /**
        * Defines the protocol setting Map to allow SE to be differentiated according to their
        * communication protocol.
        *
                             * @param seProtocol the protocol key identifier to be added to the plugin internal list
                             * @param protocolRule a string use to define how to identify the protocol
        */
public:
    void addSeProtocolSetting(std::shared_ptr<SeProtocol> seProtocol, const std::string &protocolRule) override;

    /**
        * Complete the current setting map with the provided map
        * 
        * @param protocolSetting
        */
    virtual void setSeProtocolSetting(std::unordered_map<std::shared_ptr<SeProtocol>, std::string> &protocolSetting);

    /**
        * Test if the current protocol matches the provided protocol flag.
        * <p>
        * The method must be implemented by the ProxyReader plugin.
        * <p>
        * The protocol flag is used to retrieve from the protocolsMap the String used to differentiate
        * this particular protocol. (e.g. in PC/SC the only way to identify the SE protocol is to
        * analyse the ATR returned by the reader [ISO SE and memory card SE have specific ATR], in
        * Android Nfc the SE protocol can be deduced with the TagTechnology interface).
        * 
        * @param protocolFlag the protocol flag
        * @return true if the current protocol matches the provided protocol flag
        * @throws KeypleReaderException in case of a reader exception
        */
protected:
    virtual bool protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) = 0;

    /** ==== SeRequestSe and SeRequest transmission management ============= */

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
    std::shared_ptr<SeResponseSet> processSeRequestSet(std::shared_ptr<SeRequestSet> requestSet) final override;

    /**
        * Executes a request made of one or more Apdus and receives their answers. The selection of the
        * application is handled.
        * <p>
        * The physical channel is closed if requested.
        *
        * @param seRequest the SeRequest
        * @return the SeResponse to the SeRequest
        * @throws KeypleReaderException if a transmission fails
        */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings({"PMD.ModifiedCyclomaticComplexity", "PMD.CyclomaticComplexity", "PMD.StdCyclomaticComplexity", "PMD.NPathComplexity", "PMD.ExcessiveMethodLength"}) protected final SeResponse processSeRequest(SeRequest seRequest) throws IllegalStateException, KeypleReaderException
    std::shared_ptr<SeResponse> processSeRequest(std::shared_ptr<SeRequest> seRequest) final override;

    /**
        * Implements the logical processSeRequest.
        * <p>
        * This method is called by processSeRequestSet and processSeRequest.
        * <p>
        * It opens both physical and logical channels if needed.
        * <p>
        * The logical channel is closed when CLOSE_AFTER is requested.
        *
        * @param seRequest
        * @return seResponse
        * @throws IllegalStateException
        * @throws KeypleReaderException
        */
private:
    std::shared_ptr<SeResponse> processSeRequestLogical(std::shared_ptr<SeRequest> seRequest);

    /** ==== APDU transmission management ================================== */

    /**
        * Transmits an ApduRequest and receives the ApduResponse
        * <p>
        * The time measurement is carried out and logged with the detailed information of the exchanges
        * (TRACE level).
        *
        * @param apduRequest APDU request
        * @return APDU response
        * @throws KeypleIOReaderException Exception faced
        */
protected:
    std::shared_ptr<ApduResponse> processApduRequest(std::shared_ptr<ApduRequest> apduRequest);

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
    std::shared_ptr<ApduResponse> case4HackGetResponse(int originalStatusCode);

    /**
        * Transmits a single APDU and receives its response.
        * <p>
        * This abstract method must be implemented by the ProxyReader plugin (e.g. Pcsc, Nfc). The
        * implementation must handle the case where the SE response is 61xy and execute the appropriate
        * get response command.
        *
        * @param apduIn byte buffer containing the ingoing data
        * @return apduResponse byte buffer containing the outgoing data.
        * @throws KeypleIOReaderException if the transmission fails
        */
protected:
    virtual std::vector<char> transmitApdu(std::vector<char> &apduIn) = 0;

    /** ==== Default selection assignment ================================== */

    /**
        * If defined, the prepared setDefaultSelectionRequest will be processed as soon as a SE is
        * inserted. The result of this request set will be added to the reader event.
        * <p>
        * Depending on the notification mode, the observer will be notified whenever an SE is inserted,
        * regardless of the selection status, or only if the current SE matches the selection criteria.
        *
                             * @param defaultSelectionsRequest the {@link AbstractDefaultSelectionsRequest} to be executed
                             *        when a SE is inserted
        * @param notificationMode the notification mode enum (ALWAYS or MATCHED_ONLY)
        */
public:
    virtual void setDefaultSelectionRequest(std::shared_ptr<AbstractDefaultSelectionsRequest> defaultSelectionRequest, ObservableReader::NotificationMode notificationMode) override;

protected:
    std::shared_ptr<AbstractLocalReader> shared_from_this() {
        return std::static_pointer_cast<AbstractLocalReader>(AbstractObservableReader::shared_from_this());
    }
};

}
}
}
}
}
}

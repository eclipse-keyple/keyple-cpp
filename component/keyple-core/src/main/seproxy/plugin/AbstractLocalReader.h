/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

/* Common */
#include "LoggerFactory.h"
#include "System.h"

/* Core */
#include "AbstractDefaultSelectionsRequest.h"
#include "AbstractReader.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "ChannelControl.h"
#include "KeypleCoreExport.h"
#include "MultiSeRequestProcessing.h"
#include "ObservableReader.h"
#include "SelectionStatus.h"
#include "SeSelector.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;
using namespace keyple::common;

/**
 * Manage the loop processing for SeRequest transmission in a set and for
 * SeResponse reception in a set
 */
class KEYPLECORE_API AbstractLocalReader : public AbstractReader {
public:
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
    AbstractLocalReader(const std::string& pluginName, const std::string& readerName);

    /**
     *
     */
    virtual ~AbstractLocalReader() = default;

    /**
     * Check the presence of a SE
     * <p>
     * This method is recommended for non-observable readers.
     * <p>
     * When the card is not present the logical and physical channels status may
     * be refreshed through a call to the cardRemoved method.
     *
     * @return true if the SE is present
     * @throw KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    bool isSePresent() override;

    /**
     * Tells if a logical channel is open
     *
     * @return true if the logical channel is open
     */
    virtual bool isLogicalChannelOpen() const final;

    /**
     * Defines the protocol setting Map to allow SE to be differentiated
     * according to their communication protocol.
     *
     * @param seProtocol the protocol key identifier to be added to the plugin
     *        internal list
     * @param protocolRule a string use to define how to identify the protocol
     */
    void addSeProtocolSetting(std::shared_ptr<SeProtocol> seProtocol,
                              const std::string& protocolRule) override;

    /**
      * Complete the current setting map with the provided map
      *
      * @param protocolSetting
      *
      * Alex: removed that function to avoid using a shared_ptr to SeProtocol
      *       (astract class, therefore cannot be used in a set/map)
      */
    virtual void setSeProtocolSetting(const std::map<std::shared_ptr<SeProtocol>,
                                      std::string>& protocolSetting) override;

    /**
     * Build a select application command, transmit it to the SE and deduct the
     * SelectionStatus.
     *
     * @param seSelector the targeted application SE selector
     * @return the SelectionStatus containing the actual selection result (ATR
     *         and/or FCI and the matching status flag).
     * @throw KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    virtual std::shared_ptr<SelectionStatus> openLogicalChannel(
        std::shared_ptr<SeSelector> seSelector);

    /**
     * Open (if needed) a physical channel and try to establish a logical channel.
     *
     * <p>
     * The logical opening is done either by sending a Select Application command (AID based
     * selection) or by checking the current ATR received from the SE (ATR based selection).
     * <p>
     * If the selection is successful, the logical channel is considered open. On the contrary, if
     * the selection fails, the logical channel remains closed.
     * <p>
     *
     * @param seSelector the SE Selector: either the AID of the application to select or an ATR
     *        selection regular expression
     * @return a keyple::core::seproxy::message::SelectionStatus object containing the SE ATR, the
     *         SE FCI and a flag giving the selection process result. When ATR or FCI are not
     *         available, they are set to null but they can't be both null at the same time.
     * @throw KeypleReaderIOException if the communication with the reader or the SE has failed
     */
    std::shared_ptr<SelectionStatus> openLogicalChannelAndSelect(
        std::shared_ptr<SeSelector> seSelector);

    /**
     * Do the transmission of all requests according to the protocol flag selection logic.<br>
     * <br>
     * The received responses are returned as list of keyple::core::seproxy::message::SeResponse
     * The requests are ordered at application level and the responses match this order.<br>
     * When a request is not matching the current PO, the response responses pushed in the response
     * List object is set to null.
     *
     * @param seRequests the request list
     * @param multiSeRequestProcessing the multi se processing mode
     * @param channelControl indicates if the channel has to be closed at the end of the processing
     * @return the response list
     * @throw KeypleReaderIOException if the communication with the reader or the SE has failed
     *
     * /!\ C++ vs. Java: this function is protected in Java
     */
    virtual std::vector<std::shared_ptr<SeResponse>> processSeRequests(
        const std::vector<std::shared_ptr<SeRequest>>& seRequests,
        const MultiSeRequestProcessing& multiSeRequestProcessing,
        const ChannelControl& channelControl) override; //final;

    /**
     * Executes a request made of one or more Apdus and receives their answers. The selection of the
     * application is handled.
     * <p>
     * The physical channel is closed if requested.
     *
     * @param seRequest the SeRequest
     * @param channelControl indicates if the channel has to be closed at the end of the processing
     * @return the SeResponse to the SeRequest
     * @throw KeypleReaderIOException if the communication with the reader or the SE has failed
     *
     * /!\ C++ vs. Java: this function is protected in Java
     */
    virtual std::shared_ptr<SeResponse> processSeRequest(
        const std::shared_ptr<SeRequest> seRequest,
        const ChannelControl& channelControl) override; //final;

    /**
     * Method to be implemented by child classes in order to handle the needed actions when
     * terminating the communication with a SE (closing of the physical channel, initiating a
     * removal sequence, etc.)
     */
    virtual void terminateSeCommunication() = 0;

protected:
    /**
     * Wrapper for the native method of the plugin specific local reader to
     * verify the presence of the SE.
     * <p>
     * This method must be implemented by the ProxyReader plugin (e.g. Pcsc
     * reader plugin).
     * <p>
     * This method is invoked by isSePresent.
     *
     * @return true if the SE is present
     * @throws KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    virtual bool checkSePresence() = 0;

    /**
     * Close both logical and physical channels
     */
    void closeLogicalAndPhysicalChannels();

    /**
     * This abstract method must be implemented by the derived class in order to
     * provide the SE ATR when available.
     * <p>
     * Gets the SE Answer to reset
     *
     * @return ATR returned by the SE or reconstructed by the reader
     *         (contactless)
     */
    virtual const std::vector<uint8_t>& getATR() = 0;

    /**
     * Set the flag that enables the execution of the Get Data hack to get the
     * FCI
     * <p>
     * This method should called by the reader plugins that need this specific
     * behavior (ex. OMAPI)
     *
     * <p>
     * The default value for the forceGetDataFlag is false, thus only specific
     * readers plugins should call this method.
     *
     * @param forceGetDataFlag true or false
     */
    virtual void setForceGetDataFlag(bool forceGetDataFlag);

    /**
     * This abstract method must be implemented by the derived class in order to
     * provide a selection and ATR filtering mechanism.
     * <p>
     * The Selector provided in argument holds all the needed data to handle the
     * Application Selection and ATR matching process and build the resulting
     * SelectionStatus.
     *
     * @param seSelector the SE selector
     * @return the SelectionStatus
     */

    /**
     * Attempts to open the physical channel
     *
     * @throws KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    virtual void openPhysicalChannel() = 0;

    /**
     * Closes the current physical channel.
     * <p>
     * This method must be implemented by the ProxyReader plugin (e.g.
     * Pcsc/Nfc/Omapi Reader).
     *
     * @throws KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    virtual void closePhysicalChannel() = 0;

    /**
     * Tells if the physical channel is open or not
     * <p>
     * This method must be implemented by the ProxyReader plugin (e.g.
     * Pcsc/Nfc/Omapi Reader).
     *
     * @return true is the channel is open
     */
    virtual bool isPhysicalChannelOpen() = 0;

    /**
     * Close the logical channel.
     */
    void closeLogicalChannel();

    /**
     * Test if the current protocol matches the provided protocol flag.
     * <p>
     * The method must be implemented by the ProxyReader plugin.
     * <p>
     * The protocol flag is used to retrieve from the protocolsMap the String
     * used to differentiate this particular protocol. (e.g. in PC/SC the only
     * way to identify the SE protocol is to analyse the ATR returned by the
     * reader [ISO SE and memory card SE have specific ATR], in Android Nfc the
     * SE protocol can be deduced with the TagTechnology interface).
     *
     * @param protocolFlag the protocol flag
     * @return true if the current protocol matches the provided protocol flag
     * @throw KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    virtual bool protocolFlagMatches(
        const std::shared_ptr<SeProtocol> protocolFlag) = 0;

    /**
     * Indicates whether this array of bytes starts with this other one
     *
     * @param source the byte array to examine
     * @param match the byte array to compare in <code>source</code>
     * @return true if the starting bytes of <code>source</code> equal
     *         <code>match</code>
     */
    static bool startsWith(const std::vector<uint8_t>& source,
                           const std::vector<uint8_t>& match);

    /**
     * Transmits an ApduRequest and receives the ApduResponse
     * <p>
     * The time measurement is carried out and logged with the detailed
     * information of the exchanges (TRACE level).
     *
     * @param apduRequest APDU request
     * @return APDU response
     * @throw KeypleReaderIOException if the communication with the reader or
     *        the SE has failed
     */
    std::shared_ptr<ApduResponse> processApduRequest(
        std::shared_ptr<ApduRequest> apduRequest);

    /**
     * Transmits a single APDU and receives its response.
     * <p>
     * This abstract method must be implemented by the ProxyReader plugin (e.g.
     * Pcsc, Nfc). The implementation must handle the case where the SE response
     * is 61xy and execute the appropriate get response command.
     *
     * @param apduIn byte buffer containing the ingoing data
     * @return apduResponse byte buffer containing the outgoing data.
     * @throw KeypleReaderIOException if the communication with the reader or
     *        the SE has failed
     */

    virtual std::vector<uint8_t> transmitApdu(const std::vector<uint8_t>& apduIn) = 0;

    /**
     * @return the Map containing the protocol definitions set by addSeProtocolSetting and
     *         setSeProtocolSetting
     */
    const std::map<std::shared_ptr<SeProtocol>, std::string>& getProtocolsMap() const;

private:
    /**
     * Logger
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(AbstractLocalReader));

    /**
     * PO selection map associating seProtocols and selection strings.
     * <p>
     * The String associated with a particular protocol can be anything that is
     * relevant to be interpreted by reader plugins implementing
     * protocolFlagMatches (e.g. ATR regex for Pcsc plugins, technology name for
     * Nfc plugins, etc).
     */
    std::map<std::shared_ptr<SeProtocol>, std::string> mProtocolsMap;


    /**
     * Predefined "get response" byte array
     */
    const std::vector<uint8_t> mGetResponseHackRequestBytes =
        {0x00, 0xC0, 0x00, 0x00, 0x00};

    /**
     * Logical channel status flag
     */
    bool mLogicalChannelIsOpen = false;

    /**
     *
     */
    bool mForceGetDataFlag = false;

    /**
     * Current AID if any
     */
    std::vector<uint8_t> mAidCurrentlySelected;

    /**
     * Current selection status
     */
    std::shared_ptr<SelectionStatus> mCurrentSelectionStatus;

    /**
     * Timestamp recorder
     */
    long long mBefore = 0;

    /**
     * Executes the selection application command and returns the requested data
     * according to AidSelector attributes.
     *
     * @param aidSelector the selection parameters
     * @return the response to the select application command
     * @throws KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    std::shared_ptr<ApduResponse> processExplicitAidSelection(
        SeSelector::AidSelector& aidSelector);

    /**
     * This method is dedicated to the case where no FCI data is available in
     * return for the select command.
     * <p>
     *
     * @param aidSelector used to retrieve the successful status codes from the
     *        main AidSelector
     * @return a ApduResponse containing the FCI
     * @throws KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    std::shared_ptr<ApduResponse> recoverSelectionFciData(
        SeSelector::AidSelector& aidSelector);

    /**
     * Implements the logical processSeRequest.
     * <p>
     * This method is called by processSeRequests and processSeRequest.
     * <p>
     * It opens both physical and logical channels if needed.
     * <p>
     * The logical channel is closed when requested.
     *
     * @param seRequest the keyple::core::seproxy::message::SeRequest to be sent
     * @return seResponse
     * @throw KeypleReaderIOException if the communication with the reader or
     *        the SE has failed
     */
    std::shared_ptr<SeResponse> processSeRequestLogical(
        std::shared_ptr<SeRequest> seRequest);

    /**
     * Execute a get response command in order to get outgoing data from
     * specific cards answering 9000 with no data although the command has
     * outgoing data. Note that this method relies on the right get response
     * management by transmitApdu
     *
     * @param originalStatusCode the status code of the command that didn't
     *        return data
     * @return ApduResponse the response to the get response command
     * @throw KeypleReaderIOException if the communication with the reader or
     *        the SE has failed
     */
    std::shared_ptr<ApduResponse> case4HackGetResponse(int originalStatusCode);
};

}
}
}
}

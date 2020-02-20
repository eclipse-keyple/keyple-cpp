/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "exceptionhelper.h"
#include <memory>

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "CalypsoBuilderParser.h"
#include "CalypsoPo.h"
#include "ChannelControl.h"
#include "PoBuilderParser.h"
#include "PoModificationCommand.h"
#include "PoResource.h"
#include "SamResource.h"
#include "SamRevision_Import.h"
#include "SecuritySettings.h"
#include "SelectFileCmdBuild.h"

/* Core */
#include "ProxyReader.h"
#include "ReadDataStructure.h"
#include "SeProtocol_Import.h"
#include "SeRequest.h"
#include "TransmissionMode.h"

/* Common */
#include "Export.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::transaction;
using namespace keyple::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;
using namespace keyple::calypso::transaction::exception;

/**
 * Portable Object Secure Session.
 *
 * A non-encrypted secure session with a Calypso PO requires the management of
 * two {@link ProxyReader} in order to communicate with both a Calypso PO and a
 * SAM
 *
 * @author Calypso Networks Association
 */
class EXPORT PoTransaction final
: public std::enable_shared_from_this<PoTransaction> {
private:
    /**
     * The PO Transaction State defined with the elements: ‘IOError’,
     * ‘SEInserted’ and ‘SERemoval’.
     */
    enum class SessionState {
        /**
         * Initial state of a PO transaction. The PO must have been previously
         * selected.
         */
        SESSION_UNINITIALIZED,
        /** The secure session is active. */
        SESSION_OPEN,
        /** The secure session is closed. */
        SESSION_CLOSED
    };


public:
    /**
     * The PO Transaction Access Level: personalization, loading or debiting
     */
    class EXPORT SessionAccessLevel final {
    public:
        /**
         *
         */
        enum class InnerEnum {
            SESSION_LVL_PERSO,
            SESSION_LVL_LOAD,
            SESSION_LVL_DEBIT
        };

    private:
        /**
         *
         */
        class StaticConstructor {
        public:
            StaticConstructor();
        };

    public:
        /**
         * Session Access Level used for personalization purposes.
         */
        static SessionAccessLevel SESSION_LVL_PERSO;

        /**
         * Session Access Level used for reloading purposes
         */
        static SessionAccessLevel SESSION_LVL_LOAD;

        /**
         * Session Access Level used for validating and debiting purposes
         */
        static SessionAccessLevel SESSION_LVL_DEBIT;

    private:
        /**
         *
         */
        static std::vector<SessionAccessLevel> valueList;

        /**
         *
         */
        static StaticConstructor staticConstructor;

        /**
         *
         */
        InnerEnum innerEnumValue;

        /**
         *
         */
        std::string nameValue;

        /**
         *
         */
        int ordinalValue;

        /**
         *
         */
        static int nextOrdinal;

        /**
         *
         */
        std::string name;

        /**
         *
         */
        char sessionKey;

    public:
        /**
         * DEfault constructor
         */
        SessionAccessLevel()
        : innerEnumValue(SESSION_LVL_PERSO.innerEnumValue), ordinalValue(0),
          name("session lvl perso"), sessionKey(0)
        {
        }

        /**
         * Constructor
         */
        SessionAccessLevel(const std::string &nameValue, InnerEnum innerEnum,
                           const std::string &name,
                           char sessionKey);

        /**
         *
         */
        virtual std::string getName();

        /**
         *
         */
        virtual char getSessionKey();

        /**
         *
         */
        bool operator == (const SessionAccessLevel &other);

        /**
         *
         */
        bool operator != (const SessionAccessLevel &other);

        /**
         *
         */
        static std::vector<SessionAccessLevel> values();

        /**
         *
         */
        int ordinal();

        /**
         *
         */
        std::string toString();

        /**
         *
         */
        static SessionAccessLevel valueOf(const std::string &name);
    };

    /**
     * The modification mode indicates whether the secure session can be closed
     * and reopened to manage the limitation of the PO buffer memory.
     */
    enum class ModificationMode {
        /**
         * The secure session is atomic. The consistency of the content of the
         * resulting PO memory is guaranteed.
         */
        ATOMIC,
        /**
         * Several secure sessions can be chained (to manage the writing of
         * large amounts of data). The resulting content of the PO's memory can
         * be inconsistent if the PO is removed during the process.
         */
        MULTIPLE
    };

public:
    /**
     * This class embeds all the resources to manage the secure session digest
     * computation.
     *
     * - initialize: Digest Init command
     *
     * - pushPoExchangeData and appendResponse: check consistency and all needed
     *   Digest Update commands
     *
     * - getTerminalSignature: Digest Close, returns the terminal part of the
     *   signature
     *
     * - checkPoSignature: Digest Authenticate, verify the PO part of the
     *   signature
     */
    class EXPORT DigestProcessor
    : public std::enable_shared_from_this<DigestProcessor> {
        /*
         * The digest data cache stores all PO data to be send to SAM during a
         * Secure Session. The 1st buffer is the data buffer to be provided with
         * Digest Init. The following buffers are PO command/response pairs
         */
    private:
        static std::vector<std::vector<uint8_t>> poDigestDataCache;
        static SamRevision samRevision;
        static PoRevision poRevision;
        static bool encryption;
        static bool verification;
        static bool revMode;
        static uint8_t keyRecordNumber;
        static uint8_t keyKIF;
        static uint8_t keyKVC;

        /**
         *
         */
        static const std::shared_ptr<Logger> logger;

        /**
         * Initializes the digest computation process
         *
         * @param poRev the PO revision
         * @param samRev the SAM revision
         * @param sessionEncryption true if the session is encrypted
         * @param verificationMode true if the verification mode is active
         * @param rev3_2Mode true if the REV3.2 mode is active
         * @param workKeyRecordNumber the key record number
         * @param workKeyKif the PO KIF
         * @param workKeyKVC the PO KVC
         * @param digestData a first bunch of data to digest.
         */
    public:
        static void initialize(PoRevision poRev, SamRevision samRev,
                               bool sessionEncryption, bool verificationMode,
                               bool rev3_2Mode, uint8_t workKeyRecordNumber,
                               uint8_t workKeyKif, uint8_t workKeyKVC,
                               const std::vector<uint8_t> &digestData);

        /**
         * Appends a full PO exchange (request and response) to the digest data
         * cache.
         *
         * @param request PO request
         * @param response PO response
         */
        static void pushPoExchangeData(std::shared_ptr<ApduRequest> request,
                                       std::shared_ptr<ApduResponse> response);

        /**
         * Get a unique SAM request for the whole digest computation process.
         *
         * @return SeRequest all the ApduRequest to send to the SAM in order to
         *         get the terminal signature
         */
        static std::shared_ptr<SeRequest> getSamDigestRequest();
    };

    /**
     * The class handles the anticipated response computation.
     */
    class EXPORT AnticipatedResponseBuilder
    : public std::enable_shared_from_this<AnticipatedResponseBuilder> {
    private:
        /**
         * A nested class to associate a request with a response
         */
        class CommandResponse
        : public std::enable_shared_from_this<CommandResponse> {
        private:
            /**
             *
             */
            const std::shared_ptr<ApduRequest> apduRequest;

            /**
             *
             */
            const std::shared_ptr<ApduResponse> apduResponse;


        public:
            /**
             *
             */
            CommandResponse(std::shared_ptr<ApduRequest> apduRequest,
                            std::shared_ptr<ApduResponse> apduResponse);

            /**
             *
             */
            virtual ~CommandResponse() {}

            /**
             *
             */
            virtual std::shared_ptr<ApduRequest> getApduRequest();

            /**
             *
             */
            virtual std::shared_ptr<ApduResponse> getApduResponse();
        };

        /**
         *
         */
        static const std::shared_ptr<Logger> logger;

    private:
        /**
         * A Map of SFI and Commands/Responses
         */
        static std::unordered_map<char, std::shared_ptr<CommandResponse>>
                   sfiCommandResponseHashMap;

    public:
        /**
         * Store all Read Record exchanges in a Map whose key is the SFI.
         *
         * @param poBuilderParsers the list of commands sent to the PO
         * @param apduRequests the sent apduRequests
         * @param apduResponses the received apduResponses
         * @param skipFirstItem a flag to indicate if the first
         *        apduRequest/apduResponse pair has to be ignored or not.
         */
        static void storeCommandResponse(
                        std::vector<std::shared_ptr<PoBuilderParser<
                            AbstractPoCommandBuilder<
                                AbstractPoResponseParser>>>>& poBuilderParsers,
                        std::vector<std::shared_ptr<ApduRequest>>& apduRequests,
                        std::vector<std::shared_ptr<ApduResponse>>&
                            apduResponses, bool skipFirstItem);

    public:
        /**
         * Establish the anticipated responses to commands provided in
         * poModificationCommands.
         * <p>
         * Append Record and Update Record commands return 9000
         * <p>
         * Increase and Decrease return NNNNNN9000 where NNNNNNN is the new
         * counter value.
         * <p>
         * NNNNNN is determine with the current value of the counter (extracted
         * from the Read Record responses previously collected) and the value to
         * add or subtract provided in the command.
         * <p>
         * The SFI field is used to determine which data should be used to
         * extract the needed
         * information.
         *
         * @param poBuilderParsers the modification command list
         * @return the anticipated responses.
         * @throws KeypleCalypsoSecureSessionException if an response can't be
         *         determined.
         */
        static std::vector<std::shared_ptr<ApduResponse>> getResponses(
                   std::vector<std::shared_ptr<PoBuilderParser<
                       AbstractPoCommandBuilder<AbstractPoResponseParser>>>>&
                           poBuilderParsers);
    };

    /* private constants */
private:
    static const uint8_t KIF_UNDEFINED = 0xFF;

    static const uint8_t CHALLENGE_LENGTH_REV_INF_32;
    static const uint8_t CHALLENGE_LENGTH_REV32;
    static const uint8_t SIGNATURE_LENGTH_REV_INF_32;
    static const uint8_t SIGNATURE_LENGTH_REV32;

    static constexpr int OFFSET_CLA  = 0;
    static constexpr int OFFSET_INS  = 1;
    static constexpr int OFFSET_P1   = 2;
    static constexpr int OFFSET_P2   = 3;
    static constexpr int OFFSET_Lc   = 4;
    static constexpr int OFFSET_DATA = 5;

    /**
     * Ratification command APDU for rev <= 2.4
     */
    static std::vector<uint8_t> ratificationCmdApduLegacy;

    /**
     * Ratification command APDU for rev > 2.4
     */
    static std::vector<uint8_t> ratificationCmdApdu;

    /**
     *
     */
    const std::shared_ptr<Logger> logger =
              LoggerFactory::getLogger(typeid(PoTransaction));

    /**
     * The reader for PO
     */
    const std::shared_ptr<ProxyReader> poReader;

    /**
     * The reader for session SAM
     */
    std::shared_ptr<ProxyReader> samReader;

    /**
     * The SAM default revision
     */
    const SamRevision samRevision = SamRevision::C1;

    /**
     * The security settings
     */
    std::shared_ptr<SecuritySettings> securitySettings;

    /**
     * The PO serial number extracted from FCI
     */
    std::vector<uint8_t> poCalypsoInstanceSerial;

    /**
     * The current CalypsoPo
     */
    const std::shared_ptr<CalypsoPo> calypsoPo;

    /**
     * the type of the notified event
     */
    SessionState sessionState;

    /**
     * Selected AID of the Calypso PO
     */
    std::vector<uint8_t> poCalypsoInstanceAid;

    /**
     * The PO Calypso Revision
     */
    PoRevision poRevision = static_cast<PoRevision>(0);

    /**
     * The PO Secure Session final status according to mutual authentication
     * result
     */
    bool transactionResult = false;

    /**
     * The diversification status
     */
    bool isDiversificationDone = false;

    /**
     * The PO KIF
     */
    uint8_t poKif = 0;

    /**
     * The previous PO Secure Session ratification status
     */
    bool wasRatified_Renamed = false;

    /**
     * The data read at opening
     */
    std::vector<uint8_t> openRecordDataRead;

    /**
     * The list to contain the prepared commands and their parsers
     */
    std::vector<std::shared_ptr<PoBuilderParser<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>>> poBuilderParserList =
            std::vector<std::shared_ptr<PoBuilderParser<
                AbstractPoCommandBuilder<AbstractPoResponseParser>>>>();

    /**
     * The current secure session modification mode: ATOMIC or MULTIPLE
     */
    ModificationMode currentModificationMode;

    /**
     * The current secure session access level: PERSO, RELOAD, DEBIT
     */
    SessionAccessLevel currentAccessLevel;

    /**
     * Modifications counter management
     */
    bool modificationsCounterIsInBytes = false;

    /**
     *
     */
    int modificationsCounterMax        = 0;

    /**
     *
     */
    int modificationsCounter           = 0;

    /**
     *
     */
    bool preparedCommandsProcessed = false;

    /**
     *
     */
    int preparedCommandIndex = 0;

public:
    /**
     * PoTransaction with PO and SAM readers.
     * <ul>
     * <li>Logical channels with PO &amp; SAM could already be established or
     * not.</li>
     * <li>A list of SAM parameters is provided as en EnumMap.</li>
     * </ul>
     *
     * @param poResource the PO resource (combination of {@link SeReader} and
     *        {@link CalypsoPo})
     * @param samResource the SAM resource (combination of {@link SeReader} and
     *        {@link CalypsoSam})
     * @param securitySettings a list of security settings ({@link
     *        SecuritySettings}) used in the session (such as key
     *        identification)
     */
    PoTransaction(std::shared_ptr<PoResource> poResource,
                  std::shared_ptr<SamResource> samResource,
                  std::shared_ptr<SecuritySettings> securitySettings);

    /**
     * PoTransaction with PO reader and without SAM reader.
     * <ul>
     * <li>Logical channels with PO could already be established or not.</li>
     * </ul>
     *
     * @param poResource the PO resource (combination of {@link SeReader} and
     *        {@link CalypsoPo})
     */
    PoTransaction(std::shared_ptr<PoResource> poResource);

    /**
     * Open a Secure Session.
     * <ul>
     * <li>The PO must have been previously selected, so a logical channel with
     * the PO application must be already active.</li>
     * <li>The PO serial &amp; revision are identified from FCI data.</li>
     * <li>A first request is sent to the SAM session reader.
     * <ul>
     * <li>In case not logical channel is active with the SAM, a channel is
     * open.</li>
     * <li>Then a Select Diversifier (with the PO serial) &amp; a Get Challenge
     * are automatically operated. The SAM challenge is recovered.</li>
     * </ul>
     * </li>
     * <li>The PO Open Session command is built according to the PO revision,
     * the SAM challenge, the keyIndex, and openingSfiToSelect /
     * openingRecordNumberToRead.</li>
     * <li>Next the PO reader is requested:
     * <ul>
     * <li>for the current selected PO AID, with channelControl set to
     * KEEP_OPEN,</li>
     * <li>and some PO Apdu Requests including at least the Open Session command
     * and optionally some PO command to operate inside the session.</li>
     * </ul>
     * </li>
     * <li>The session PO keyset reference is identified from the PO Open
     * Session response, the PO challenge is recovered too.</li>
     * <li>According to the PO responses of Open Session and the PO commands
     * sent inside the session, a "cache" of SAM commands is filled with the
     * corresponding Digest Init &amp; Digest Update commands.</li>
     * <li>Returns the corresponding PO SeResponse (responses to
     * poBuilderParsers).</li>
     * </ul>
     *
     * @param accessLevel access level of the session (personalization, load or
     *        debit).
     * @param openingSfiToSelect SFI of the file to select (0 means no file to
     *        select)
     * @param openingRecordNumberToRead number of the record to read
     * @param poBuilderParsers the po commands inside session
     * @return SeResponse response to all executed commands including the self
     *         generated "Open Secure Session" command
     * @throws KeypleReaderException the IO reader exception
     */
  private:
    std::shared_ptr<SeResponse> processAtomicOpening(
        SessionAccessLevel accessLevel, uint8_t openingSfiToSelect,
        uint8_t openingRecordNumberToRead,
        std::vector<std::shared_ptr<PoBuilderParser<AbstractPoCommandBuilder<
            AbstractPoResponseParser>>>>& poBuilderParsers);

    /**
     * Change SendableInSession List to ApduRequest List .
     *
     * @param poOrSamCommandsInsideSession a po or sam commands list to be sent in session
     * @return the ApduRequest list
     */
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ template equivalent to this generic constraint:
//ORIGINAL LINE: private List<ApduRequest> getApduRequestsToSendInSession(List<? extends org.eclipse.keyple.calypso.command.CalypsoBuilderParser> poOrSamCommandsInsideSession)
    std::vector<std::shared_ptr<ApduRequest>> getApduRequestsToSendInSession(
                    std::vector<std::shared_ptr<PoBuilderParser<AbstractPoCommandBuilder<
                                        AbstractPoResponseParser>>>> poOrSamCommandsInsideSession);

    /**
     * Process PO commands in a Secure Session.
     * <ul>
     * <li>On the PO reader, generates a SeRequest with channelControl set to KEEP_OPEN, and
     * ApduRequests with the PO commands.</li>
     * <li>In case the secure session is active, the "cache" of SAM commands is completed with the
     * corresponding Digest Update commands.</li>
     * <li>If a session is open and channelControl is set to CLOSE_AFTER, the current PO session is
     * aborted</li>
     * <li>Returns the corresponding PO SeResponse.</li>
     * </ul>
     *
     * @param poBuilderParsers the po commands inside session
     * @param channelControl indicated if the SE channel of the PO reader must be closed after the
     *        last command
     * @return SeResponse all responses to the provided commands
     *
     * @throws KeypleReaderException IO Reader exception
     */
    std::shared_ptr<SeResponse> processAtomicPoCommands(
         std::vector<std::shared_ptr<PoBuilderParser<AbstractPoCommandBuilder<
                                                    AbstractPoResponseParser>>>> &poBuilderParsers,
         ChannelControl channelControl);

    /**
     * Process SAM commands.
     * <ul>
     * <li>On the SAM reader, transmission of a SeRequest with channelControl set to KEEP_OPEN.</li>
     * <li>Returns the corresponding SAM SeResponse.</li>
     * </ul>
     *
     * @param samBuilderParsers a list of commands to sent to the SAM
     * @return SeResponse all sam responses
     * @throws KeypleReaderException if a reader error occurs
     */
    // public SeResponse processSamCommands(List<SamBuilderParser> samBuilderParsers)
    // throws KeypleReaderException {
    //
    // /* Init SAM ApduRequest List - for the first SAM exchange */
    // List<ApduRequest> samApduRequestList =
    // this.getApduRequestsToSendInSession(samBuilderParsers);
    //
    // /* SeRequest from the command list */
    // SeRequest samSeRequest = new SeRequest(samApduRequestList, channelControl.KEEP_OPEN);
    //
    // logger.debug("processSamCommands => SAMSEREQUEST = {}", samSeRequest);
    //
    // /* Transmit SeRequest and get SeResponse */
    // SeResponse samSeResponse = samReader.transmit(samSeRequest);
    //
    // if (samSeResponse == null) {
    // throw new KeypleCalypsoSecureSessionException("Null response received",
    // KeypleCalypsoSecureSessionException.Type.SAM, samSeRequest.getApduRequests(),
    // null);
    // }
    //
    // if (sessionState == SessionState.SESSION_OPEN
    // && !samSeResponse.wasChannelPreviouslyOpen()) {
    // throw new KeypleCalypsoSecureSessionException("The logical channel was not open",
    // KeypleCalypsoSecureSessionException.Type.SAM, samSeRequest.getApduRequests(),
    // null);
    // }
    // // TODO check if the wasChannelPreviouslyOpen should be done in the case where the session
    // // is closed
    //
    // return samSeResponse;
    // }

    /**
     * Close the Secure Session.
     * <ul>
     * <li>The SAM cache is completed with the Digest Update commands related to the new PO commands
     * to be sent and their anticipated responses. A Digest Close command is also added to the SAM
     * command cache.</li>
     * <li>On the SAM session reader side, a SeRequest is transmitted with SAM commands from the
     * command cache. The SAM command cache is emptied.</li>
     * <li>The SAM certificate is retrieved from the Digest Close response. The terminal signature
     * is identified.</li>
     * <li>Then, on the PO reader, a SeRequest is transmitted with the provided channelControl, and
     * apduRequests including the new PO commands to send in the session, a Close Session command
     * (defined with the SAM certificate), and optionally a ratificationCommand.
     * <ul>
     * <li>The management of ratification is conditioned by the mode of communication.
     * <ul>
     * <li>If the communication mode is CONTACTLESS, a specific ratification command is sent after
     * the Close Session command. No ratification is requested in the Close Session command.</li>
     * <li>If the communication mode is CONTACTS, no ratification command is sent after the Close
     * Session command. Ratification is requested in the Close Session command.</li>
     * </ul>
     * </li>
     * <li>Otherwise, the PO Close Secure Session command is defined to directly set the PO as
     * ratified.</li>
     * </ul>
     * </li>
     * <li>The PO responses of the poModificationCommands are compared with the
     * poAnticipatedResponses. The PO signature is identified from the PO Close Session
     * response.</li>
     * <li>The PO certificate is recovered from the Close Session response. The card signature is
     * identified.</li>
     * <li>Finally, on the SAM session reader, a Digest Authenticate is automatically operated in
     * order to verify the PO signature.</li>
     * <li>Returns the corresponding PO SeResponse.</li>
     * </ul>
     *
     * The method is marked as deprecated because the advanced variant defined below must be used at
     * the application level.
     *
     * @param poModificationCommands a list of commands that can modify the PO memory content
     * @param poAnticipatedResponses a list of anticipated PO responses to the modification commands
     * @param transmissionMode the communication mode. If the communication mode is CONTACTLESS, a
     *        ratification command will be generated and sent to the PO after the Close Session
     *        command; the ratification will not be requested in the Close Session command. On the
     *        contrary, if the communication mode is CONTACTS, no ratification command will be sent
     *        to the PO and ratification will be requested in the Close Session command
     * @param channelControl indicates if the SE channel of the PO reader must be closed after the
     *        last command
     * @return SeResponse close session response
     * @throws KeypleReaderException the IO reader exception This method is deprecated.
     *         <ul>
     *         <li>The argument of the ratification command is replaced by an indication of the PO
     *         communication mode.</li>
     *         </ul>
     */
    std::shared_ptr<SeResponse> processAtomicClosing(
        std::vector<std::shared_ptr<PoBuilderParser<AbstractPoCommandBuilder<
                                              AbstractPoResponseParser>>>> &poModificationCommands,
        std::vector<std::shared_ptr<ApduResponse>> &poAnticipatedResponses,
        TransmissionMode transmissionMode, ChannelControl channelControl);

    /**
     * Advanced variant of processAtomicClosing in which the list of expected responses is
     * determined from previous reading operations.
     *
     * @param poBuilderParsers a list of commands that can modify the PO memory content
     * @param transmissionMode the communication mode. If the communication mode is CONTACTLESS, a
     *        ratification command will be generated and sent to the PO after the Close Session
     *        command; the ratification will not be requested in the Close Session command. On the
     *        contrary, if the communication mode is CONTACTS, no ratification command will be sent
     *        to the PO and ratification will be requested in the Close Session command
     * @param channelControl indicates if the SE channel of the PO reader must be closed after the
     *        last command
     * @return SeResponse close session response
     * @throws KeypleReaderException the IO reader exception This method is deprecated.
     *         <ul>
     *         <li>The argument of the ratification command is replaced by an indication of the PO
     *         communication mode.</li>
     *         </ul>
     */
    std::shared_ptr<SeResponse> processAtomicClosing(
         std::vector<std::shared_ptr<PoBuilderParser<AbstractPoCommandBuilder<
                                                    AbstractPoResponseParser>>>> &poBuilderParsers,
         TransmissionMode transmissionMode, ChannelControl channelControl);

    /**
     * Get the Secure Session Status.
     * <ul>
     * <li>To check the result of a closed secure session, returns true if the SAM Digest
     * Authenticate is successful.</li>
     * </ul>
     *
     * @return the {@link PoTransaction}.transactionResult
     */
public:
    bool isSuccessful();

    /**
     * Get the ratification status obtained at Session Opening
     *
     * @return true or false
     * @throws IllegalStateException if no session has been initiated
     */
    bool wasRatified();

    /**
     * Get the data read at Session Opening
     *
     * @return a byte array containing the data
     * @throws IllegalStateException if no session has been initiated
     */
    const std::vector<uint8_t>& getOpenRecordDataRead() const;

    /**
     * Open a Secure Session.
     * <ul>
     * <li>The PO must have been previously selected, so a logical channel with
     * the PO application must be already active.</li>
     * <li>The PO serial &amp; revision are identified from FCI data.</li>
     * <li>A first request is sent to the SAM session reader.
     * <ul>
     * <li>In case not logical channel is active with the SAM, a channel is
     * open.</li>
     * <li>Then a Select Diversifier (with the PO serial) &amp; a Get Challenge
     * are automatically operated. The SAM challenge is recovered.</li>
     * </ul>
     * </li>
     * <li>The PO Open Session command is built according to the PO revision,
     * the SAM challenge, the keyIndex, and openingSfiToSelect /
     * openingRecordNumberToRead.</li>
     * <li>Next the PO reader is requested:
     * <ul>
     * <li>for the currently selected PO, with channelControl set to KEEP_OPEN,
     * </li>
     * <li>and some PO Apdu Requests including at least the Open Session command
     * and all prepared PO command to operate inside the session.</li>
     * </ul>
     * </li>
     * <li>The session PO keyset reference is identified from the PO Open
     * Session response, the PO challenge is recovered too.</li>
     * <li>According to the PO responses of Open Session and the PO commands
     * sent inside the session, a "cache" of SAM commands is filled with the
     * corresponding Digest Init &amp; Digest Update commands.</li>
     * <li>All parsers keept by the prepare command methods are updated with the
     * Apdu responses from the PO and made available with the getCommandParser
     * method.</li>
     * </ul>
     *
     * @param modificationMode the modification mode: ATOMIC or MULTIPLE (see
     *        {@link ModificationMode})
     * @param accessLevel access level of the session (personalization, load or
     *        debit).
     * @param openingSfiToSelect SFI of the file to select (0 means no file to
     *        select)
     * @param openingRecordNumberToRead number of the record to read
     * @return true if all commands are successful
     * @throws KeypleReaderException the IO reader exception
     */
public:
    bool processOpening(ModificationMode modificationMode,
                        SessionAccessLevel accessLevel,
                        uint8_t openingSfiToSelect,
                        uint8_t openingRecordNumberToRead);

    /**
     * Process all prepared PO commands (outside a Secure Session).
     * <ul>
     * <li>On the PO reader, generates a SeRequest with channelControl set to the
     * provided value and ApduRequests containing the PO commands.</li>
     * <li>All parsers keept by the prepare command methods are updated with the
     * Apdu responses from the PO and made available with the getCommandParser
     * method.</li>
     * </ul>
     *
     * @param channelControl indicates if the SE channel of the PO reader must be
     *        closed after the last command
     * @return true if all commands are successful
     *
     * @throws KeypleReaderException IO Reader exception
     */
    bool processPoCommands(ChannelControl channelControl);

    /**
     * Process all prepared PO commands in a Secure Session.
     * <ul>
     * <li>On the PO reader, generates a SeRequest with channelControl set to
     * KEEP_OPEN, and ApduRequests containing the PO commands.</li>
     * <li>In case the secure session is active, the "cache" of SAM commands is
     * completed with the corresponding Digest Update commands.</li>
     * <li>All parsers keept by the prepare command methods are updated with the
     * Apdu responses from the PO and made available with the getCommandParser
     * method.</li>
     * </ul>
     *
     * @return true if all commands are successful
     *
     * @throws KeypleReaderException IO Reader exception
     */
    bool processPoCommandsInSession();

    /**
     * Sends the currently prepared commands list (may be empty) and closes the
     * Secure Session.
     * <ul>
     * <li>The ratification is handled according to the communication mode.</li>
     * <li>The logical channel can be left open or closed.</li>
     * <li>All parsers keept by the prepare command methods are updated with the
     * Apdu responses from the PO and made available with the getCommandParser
     * method.</li>
     * </ul>
     *
     * <p>
     * The communication mode is retrieved from CalypsoPO to manage the
     * ratification process. If the communication mode is CONTACTLESS, a
     * ratification command will be generated and sent to the PO after the Close
     * Session command; the ratification will not be requested in the Close
     * Session command. On the contrary, if the communication mode is CONTACTS,
     * no ratification command will be sent to the PO and ratification will be
     * requested in the Close Session command
     *
     * @param channelControl indicates if the SE channel of the PO reader must be
     *        closed after the last command
     * @return true if all commands are successful
     * @throws KeypleReaderException the IO reader exception This method is
     *         deprecated.
     *         <ul>
     *         <li>The argument of the ratification command is replaced by an
     *         indication of the PO communication mode.</li>
     *         </ul>
     */
    bool processClosing(ChannelControl channelControl);

    /**
     * Abort a Secure Session.
     * <p>
     * Send the appropriate command to the PO
     * <p>
     * Clean up internal data and status.
     *
     * @param channelControl indicates if the SE channel of the PO reader must be
     *        closed after the abort session command
     * @return true if the abort command received a successful response from the
     *         PO
     */
    bool processCancel(ChannelControl channelControl);

    /**
     * Loops on the SeResponse and create the appropriate builders
     *
     * @param seResponse the seResponse from the PO
     * @param poBuilderParsers the list of {@link PoBuilderParser} (sublist of
     *        the global list)
     * @return false if one or more of the commands do not succeed
     */
private:
    bool createResponseParsers(
            std::shared_ptr<SeResponse> seResponse,
            std::vector<std::shared_ptr<PoBuilderParser<
                AbstractPoCommandBuilder<AbstractPoResponseParser>>>>&
                    poBuilderParsers);

    /**
     * Checks whether the requirement for the modifications buffer of the
     * command provided in argument is compatible with the current usage level
     * of the buffer.
     * <p>
     * If it is compatible, the requirement is subtracted from the current level
     * and the method returns false. If this is not the case, the method returns
     * true.
     *
     * @param modificationCommand the modification command
     * @return true or false
     */
    bool willOverflowBuffer(std::shared_ptr<PoModificationCommand>
                            modificationCommand);

    /**
     * Initialized the modifications buffer counter to its maximum value for the
     *  current PO
     */
    void resetModificationsBufferCounter();

    /**
     * Manage the builders and parsers lists.
     * <p>
     * Handle the clearing of the lists.
     */
    int createAndStoreCommandBuilder(
            std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
                commandBuilder);

    /**
     * Prepare a select file ApduRequest to be executed following the selection.
     * <p>
     *
     * @param path path from the CURRENT_DF (CURRENT_DF identifier excluded)
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index (input order, starting at 0)
     */
public:
    int prepareSelectFileCmd(const std::vector<uint8_t>& path,
                             const std::string& extraInfo);

    /**
     * Prepare a select file ApduRequest to be executed following the selection.
     * <p>
     *
     * @param selectControl provides the navigation case: FIRST, NEXT or CURRENT
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index (input order, starting at 0)
     */
    int prepareSelectFileCmd(SelectFileCmdBuild::SelectControl selectControl,
                             const std::string& extraInfo);

    /**
     * Internal method to handle expectedLength checks in public variants
     *
     * @param sfi the sfi top select
     * @param readDataStructureEnum read mode enum to indicate a SINGLE,
     *        MULTIPLE or COUNTER read
     * @param firstRecordNumber the record number to read (or first record to
     *        read in case of several records)
     * @param expectedLength the expected length of the record(s)
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index (input order, starting at 0)
     * @throws IllegalArgumentException - if record number &lt; 1
     * @throws IllegalArgumentException - if the request is inconsistent
     */
private:
    int prepareReadRecordsCmdInternal(
            uint8_t sfi, ReadDataStructure readDataStructureEnum,
            uint8_t firstRecordNumber, int expectedLength,
            const std::string& extraInfo);

    /**
     * Builds a ReadRecords command and add it to the list of commands to be
     * sent with the next process command.
     * <p>
     * The expected length is provided and its value is checked between 1 and
     * 250.
     * <p>
     * Returns the associated response parser.
     *
     * @param sfi the sfi top select
     * @param readDataStructureEnum read mode enum to indicate a SINGLE,
     *        MULTIPLE or COUNTER read
     * @param firstRecordNumber the record number to read (or first record to
     *        read in case of several records)
     * @param expectedLength the expected length of the record(s)
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index (input order, starting at 0)
     * @throws IllegalArgumentException - if record number &lt; 1
     * @throws IllegalArgumentException - if the request is inconsistent
     */
public:
    int prepareReadRecordsCmd(uint8_t sfi,
                              ReadDataStructure readDataStructureEnum,
                              uint8_t firstRecordNumber, int expectedLength,
                              const std::string& extraInfo);

    /**
     * Builds a ReadRecords command and add it to the list of commands to be
     * sent with the next process command. No expected length is specified, the
     * record output length is handled automatically.
     * <p>
     * Returns the associated response parser.
     *
     * @param sfi the sfi top select
     * @param readDataStructureEnum read mode enum to indicate a SINGLE,
     *        MULTIPLE or COUNTER read
     * @param firstRecordNumber the record number to read (or first record to
     *        read in case of several records)
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index (input order, starting at 0)
     * @throws IllegalArgumentException - if record number &lt; 1
     * @throws IllegalArgumentException - if the request is inconsistent
     */
    int prepareReadRecordsCmd(uint8_t sfi,
                              ReadDataStructure readDataStructureEnum,
                              uint8_t firstRecordNumber,
                              const std::string& extraInfo);

    /**
     * Builds an AppendRecord command and add it to the list of commands to be
     * sent with the next process command.
     * <p>
     * Returns the associated response parser.
     *
     * @param sfi the sfi to select
     * @param newRecordData the new record data to write
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index (input order, starting at 0)
     * @throws IllegalArgumentException - if the command is inconsistent
     */
    int prepareAppendRecordCmd(uint8_t sfi,
                               const std::vector<uint8_t>& newRecordData,
                               const std::string &extraInfo);

    /**
     * Builds an UpdateRecord command and add it to the list of commands to be
     * sent with the next process command
     * <p>
     * Returns the associated response parser index.
     *
     * @param sfi the sfi to select
     * @param recordNumber the record number to update
     * @param newRecordData the new record data. If length &lt; RecSize, bytes
     *        beyond length are left unchanged.
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index (input order, starting at 0)
     * @throws IllegalArgumentException - if record number is &lt; 1
     * @throws IllegalArgumentException - if the request is inconsistent
     */
    int prepareUpdateRecordCmd(uint8_t sfi, uint8_t recordNumber,
                               const std::vector<uint8_t>& newRecordData,
                               const std::string& extraInfo);

    /**
     * Builds an WriteRecord command and add it to the list of commands to be
     * sent with the next process command
     * <p>
     * Returns the associated response parser index.
     *
     * @param sfi the sfi to select
     * @param recordNumber the record number to write
     * @param overwriteRecordData the data to overwrite in the record. If length
     *        &lt; RecSize, bytes beyond length are left unchanged.
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index (input order, starting at 0)
     * @throws IllegalArgumentException - if record number is &lt; 1
     * @throws IllegalArgumentException - if the request is inconsistent
     */
    int prepareWriteRecordCmd(uint8_t sfi, uint8_t recordNumber,
                              const std::vector<uint8_t>& overwriteRecordData,
                              const std::string& extraInfo);

    /**
     * Builds a Increase command and add it to the list of commands to be sent
     * with the next process command
     * <p>
     * Returns the associated response parser index.
     *
     * @param counterNumber &gt;= 01h: Counters file, number of the counter.
     *        00h: Simulated Counter file.
     * @param sfi SFI of the file to select or 00h for current EF
     * @param incValue Value to add to the counter (defined as a positive int
     *        &lt;= 16777215 [FFFFFFh])
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index (input order, starting at 0)
     * @throws IllegalArgumentException - if the decrement value is out of range
     * @throws IllegalArgumentException - if the command is inconsistent
     */
    int prepareIncreaseCmd(uint8_t sfi, uint8_t counterNumber, int incValue,
                           const std::string& extraInfo);

    /**
     * Builds a Decrease command and add it to the list of commands to be sent
     * with the next process command
     * <p>
     * Returns the associated response parser index.
     *
     * @param counterNumber &gt;= 01h: Counters file, number of the counter.
     *        00h: Simulated Counter file.
     * @param sfi SFI of the file to select or 00h for current EF
     * @param decValue Value to subtract to the counter (defined as a positive
     *        int &lt;= 16777215 [FFFFFFh])
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index (input order, starting at 0)
     * @throws IllegalArgumentException - if the decrement value is out of range
     * @throws IllegalArgumentException - if the command is inconsistent
     */
    int prepareDecreaseCmd(uint8_t sfi, uint8_t counterNumber, int decValue,
                           const std::string& extraInfo);

    /**
     * Get the response parser matching the prepared command for which the index
     * is provided
     *
     * @param commandIndex the index of the parser to be retrieved
     * @return the corresponding command parser
     */
    std::shared_ptr<AbstractApduResponseParser>
        getResponseParser(int commandIndex);
};

}
}
}

#pragma once

#include "../command/sam/SamRevision.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/TransmissionMode.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ProxyReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace transaction { class CalypsoPo; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace command { class AbstractApduResponseParser; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace command { template<typename T>class SendableInSession; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace command { namespace sam { class SamSendableInSession; } } } } } }

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
            namespace calypso {
                namespace transaction {

                    using SendableInSession = org::eclipse::keyple::calypso::command::SendableInSession;
                    using namespace org::eclipse::keyple::calypso::command::po;
                    using namespace org::eclipse::keyple::calypso::command::po::builder;
                    using namespace org::eclipse::keyple::calypso::command::po::parser;
                    using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;
                    using SamSendableInSession = org::eclipse::keyple::calypso::command::sam::SamSendableInSession;
                    using namespace org::eclipse::keyple::calypso::transaction::exception;
                    using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                    using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                    using SeReader = org::eclipse::keyple::seproxy::SeReader;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using namespace org::eclipse::keyple::seproxy::message;
                    using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                    using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;


                    /**
                     * Portable Object Secure Session.
                     *
                     * A non-encrypted secure session with a Calypso PO requires the management of two
                     * {@link ProxyReader} in order to communicate with both a Calypso PO and a SAM
                     *
                     * @author Calypso Networks Association
                     */
                    class PoTransaction final : public std::enable_shared_from_this<PoTransaction> {

                        /* public constants */
                        /** The key index for personalization operations (issuer key needed) */
                    public:
                        static const char KEY_INDEX_PERSONALIZATION = static_cast<char>(0x01);
                        /** The key index for reloading operations (loading key needed) */
                        static const char KEY_INDEX_LOAD = static_cast<char>(0x02);
                        /** The key index for debit and validation operations (validation key needed) */
                        static const char KEY_INDEX_VALIDATION_DEBIT = static_cast<char>(0x03);
                        /** The default KIF value for personalization */
                        static const char DEFAULT_KIF_PERSO = static_cast<char>(0x21);
                        /** The default KIF value for loading */
                        static const char DEFAULT_KIF_LOAD = static_cast<char>(0x27);
                        /** The default KIF value for debiting */
                        static const char DEFAULT_KIF_DEBIT = static_cast<char>(0x30);
                        /** The default key record number */
                        static const char DEFAULT_KEY_RECORD_NUMER = static_cast<char>(0x00);

                        /* private constants */
                    private:
                        static const char KIF_UNDEFINED = static_cast<char>(0xFF);

                        static const char CHALLENGE_LENGTH_REV_INF_32 = static_cast<char>(0x04);
                        static const char CHALLENGE_LENGTH_REV32 = static_cast<char>(0x08);
                        static const char SIGNATURE_LENGTH_REV_INF_32 = static_cast<char>(0x04);
                        static const char SIGNATURE_LENGTH_REV32 = static_cast<char>(0x08);

                        static constexpr int OFFSET_CLA = 0;
                        static constexpr int OFFSET_INS = 1;
                        static constexpr int OFFSET_P1 = 2;
                        static constexpr int OFFSET_P2 = 3;
                        static constexpr int OFFSET_Lc = 4;
                        static constexpr int OFFSET_DATA = 5;

                        /** Ratification command APDU for rev <= 2.4 */
                        static std::vector<char> const ratificationCmdApduLegacy;
                        /** Ratification command APDU for rev > 2.4 */
                        static std::vector<char> const ratificationCmdApdu;

                        static const std::shared_ptr<Logger> logger;

                        /** The reader for PO. */
                        const std::shared_ptr<ProxyReader> poReader;
                        /** The reader for session SAM. */
                        std::shared_ptr<ProxyReader> samReader;
                        /** The SAM default revision. */
                        const SamRevision samRevision = SamRevision::C1;
                        /** The SAM settings map. */
                        const std::shared_ptr<EnumMap<SamSettings, Byte>> samSetting = std::make_shared<EnumMap<SamSettings, Byte>>(SamSettings::typeid);
                        /** The PO serial number extracted from FCI */
                        std::vector<char> const poCalypsoInstanceSerial;
                        /** The current CalypsoPo */
                    protected:
                        const std::shared_ptr<CalypsoPo> calypsoPo;
                        /** the type of the notified event. */
                    private:
                        SessionState currentState = static_cast<SessionState>(0);
                        /** Selected AID of the Calypso PO. */
                        std::vector<char> poCalypsoInstanceAid;
                        /** The PO Calypso Revision. */
                        PoRevision poRevision = PoRevision::REV3_1;
                        /** The PO Secure Session final status according to mutual authentication result */
                        bool transactionResult = false;
                        /** The diversification status */
                        bool isDiversificationDone = false;
                        /** The PO KIF */
                        char poKif = 0;
                        /** The previous PO Secure Session ratification status */
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                        bool wasRatified_Renamed = false;
                        /** The data read at opening */
                        std::vector<char> openRecordDataRead;
                        /** The list to contain the prepared commands */
                        std::vector<std::shared_ptr<PoSendableInSession>> poCommandBuilderList = std::vector<std::shared_ptr<PoSendableInSession>>();
                        /** The list to contain the parsers associated to the prepared commands */
                        std::vector<std::shared_ptr<AbstractApduResponseParser>> poResponseParserList = std::vector<std::shared_ptr<AbstractApduResponseParser>>();
                        /** The SAM settings status */
                        bool samSettingsDefined = false;
                        /** List of authorized KVCs */
                        std::vector<Byte> authorizedKvcList;
                        /** The current secure session modification mode: ATOMIC or MULTIPLE */
                        ModificationMode currentModificationMode = static_cast<ModificationMode>(0);
                        /** The current secure session access level: PERSO, RELOAD, DEBIT */
                        SessionAccessLevel currentAccessLevel = static_cast<SessionAccessLevel>(0);
                        /* modifications counter management */
                        bool modificationsCounterIsInBytes = false;
                        int modificationsCounterMax = 0;
                        int modificationsCounter = 0;

                        /**
                         * PoTransaction with PO and SAM readers.
                         * <ul>
                         * <li>Logical channels with PO &amp; SAM could already be established or not.</li>
                         * <li>A list of SAM parameters is provided as en EnumMap.</li>
                         * </ul>
                         *
                         * @param poReader the PO reader
                         * @param calypsoPO the CalypsoPo object obtained at the end of the selection step
                         * @param samReader the SAM reader
                         * @param samSetting a list of SAM related parameters. In the case this parameter is null,
                         *        default parameters are applied. The available setting keys are defined in
                         *        {@link SamSettings}
                         */
                    public:
                        PoTransaction(std::shared_ptr<SeReader> poReader, std::shared_ptr<CalypsoPo> calypsoPO, std::shared_ptr<SeReader> samReader, std::shared_ptr<EnumMap<SamSettings, Byte>> samSetting);

                        /**
                         * PoTransaction with PO reader and without SAM reader.
                         * <ul>
                         * <li>Logical channels with PO could already be established or not.</li>
                         * </ul>
                         *
                         * @param poReader the PO reader
                         * @param calypsoPO the CalypsoPo object obtained at the end of the selection step
                         */
                        PoTransaction(std::shared_ptr<SeReader> poReader, std::shared_ptr<CalypsoPo> calypsoPO);

                        /**
                         * Sets the SAM parameters for Secure Session management
                         * 
                         * @param samReader
                         * @param samSetting
                         */
                        void setSamSettings(std::shared_ptr<SeReader> samReader, std::shared_ptr<EnumMap<SamSettings, Byte>> samSetting);

                        /**
                         * Provides a list of authorized KVC
                         *
                         * If this method is not called, the list will remain empty and all KVCs will be accepted.
                         *
                         * If a list is provided and a PO with a KVC not belonging to this list is presented, a
                         * {@link KeypleCalypsoSecureSessionUnauthorizedKvcException} will be raised.
                         * 
                         * @param authorizedKvcList the list of authorized KVCs
                         */
                        void setAuthorizedKvcList(std::vector<Byte> &authorizedKvcList);

                        /**
                         * Indicates whether or not the SAM settings have been defined
                         * 
                         * @return true if the SAM settings have been defined.
                         */
                        bool isSamSettingsDefined();

                        /**
                         * Open a Secure Session.
                         * <ul>
                         * <li>The PO must have been previously selected, so a logical channel with the PO application
                         * must be already active.</li>
                         * <li>The PO serial &amp; revision are identified from FCI data.</li>
                         * <li>A first request is sent to the SAM session reader.
                         * <ul>
                         * <li>In case not logical channel is active with the SAM, a channel is open.</li>
                         * <li>Then a Select Diversifier (with the PO serial) &amp; a Get Challenge are automatically
                         * operated. The SAM challenge is recovered.</li>
                         * </ul>
                         * </li>
                         * <li>The PO Open Session command is built according to the PO revision, the SAM challenge, the
                         * keyIndex, and openingSfiToSelect / openingRecordNumberToRead.</li>
                         * <li>Next the PO reader is requested:
                         * <ul>
                         * <li>for the current selected PO AID, with channelState set to KEEP_OPEN,</li>
                         * <li>and some PO Apdu Requests including at least the Open Session command and optionally some
                         * PO command to operate inside the session.</li>
                         * </ul>
                         * </li>
                         * <li>The session PO keyset reference is identified from the PO Open Session response, the PO
                         * challenge is recovered too.</li>
                         * <li>According to the PO responses of Open Session and the PO commands sent inside the
                         * session, a "cache" of SAM commands is filled with the corresponding Digest Init &amp; Digest
                         * Update commands.</li>
                         * <li>Returns the corresponding PO SeResponse (responses to poCommandsInsideSession).</li>
                         * </ul>
                         *
                         * @param accessLevel access level of the session (personalization, load or debit).
                         * @param openingSfiToSelect SFI of the file to select (0 means no file to select)
                         * @param openingRecordNumberToRead number of the record to read
                         * @param poCommandsInsideSession the po commands inside session
                         * @return SeResponse response to all executed commands including the self generated "Open
                         *         Secure Session" command
                         * @throws KeypleReaderException the IO reader exception
                         */
                    private:
                        std::shared_ptr<SeResponse> processAtomicOpening(SessionAccessLevel accessLevel, char openingSfiToSelect, char openingRecordNumberToRead, std::vector<std::shared_ptr<PoSendableInSession>> &poCommandsInsideSession) throw(KeypleReaderException);

                        /**
                         * Change SendableInSession List to ApduRequest List .
                         *
                         * @param poOrSamCommandsInsideSession a po or sam commands list to be sent in session
                         * @return the ApduRequest list
                         */
                        std::vector<std::shared_ptr<ApduRequest>> getApduRequestsToSendInSession(std::vector<std::shared_ptr<SendableInSession>> &poOrSamCommandsInsideSession);

                        /**
                         * Process PO commands in a Secure Session.
                         * <ul>
                         * <li>On the PO reader, generates a SeRequest with channelState set to KEEP_OPEN, and
                         * ApduRequests with the PO commands.</li>
                         * <li>In case the secure session is active, the "cache" of SAM commands is completed with the
                         * corresponding Digest Update commands.</li>
                         * <li>If a session is open and channelState is set to CLOSE_AFTER, the current PO session is
                         * aborted</li>
                         * <li>Returns the corresponding PO SeResponse.</li>
                         * </ul>
                         *
                         * @param poCommands the po commands inside session
                         * @param channelState indicated if the SE channel of the PO reader must be closed after the
                         *        last command
                         * @return SeResponse all responses to the provided commands
                         *
                         * @throws KeypleReaderException IO Reader exception
                         */
                        std::shared_ptr<SeResponse> processAtomicPoCommands(std::vector<std::shared_ptr<PoSendableInSession>> &poCommands, ChannelState channelState) throw(KeypleReaderException);

                        /**
                         * Process SAM commands.
                         * <ul>
                         * <li>On the SAM reader, transmission of a SeRequest with channelState set to KEEP_OPEN.</li>
                         * <li>Returns the corresponding SAM SeResponse.</li>
                         * </ul>
                         *
                         * @param samCommands a list of commands to sent to the SAM
                         * @return SeResponse all sam responses
                         * @throws KeypleReaderException if a reader error occurs
                         */
                    public:
                        std::shared_ptr<SeResponse> processSamCommands(std::vector<std::shared_ptr<SamSendableInSession>> &samCommands) throw(KeypleReaderException);

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
                         * <li>Then, on the PO reader, a SeRequest is transmitted with the provided channelState, and
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
                         * @param channelState indicates if the SE channel of the PO reader must be closed after the
                         *        last command
                         * @return SeResponse close session response
                         * @throws KeypleReaderException the IO reader exception This method is deprecated.
                         *         <ul>
                         *         <li>The argument of the ratification command is replaced by an indication of the PO
                         *         communication mode.</li>
                         *         </ul>
                         */
                    private:
                        std::shared_ptr<SeResponse> processAtomicClosing(std::vector<std::shared_ptr<PoModificationCommand>> &poModificationCommands, std::vector<std::shared_ptr<ApduResponse>> &poAnticipatedResponses, TransmissionMode transmissionMode, ChannelState channelState) throw(KeypleReaderException);

                        /**
                         * Advanced variant of processAtomicClosing in which the list of expected responses is
                         * determined from previous reading operations.
                         *
                         * @param poModificationCommands a list of commands that can modify the PO memory content
                         * @param transmissionMode the communication mode. If the communication mode is CONTACTLESS, a
                         *        ratification command will be generated and sent to the PO after the Close Session
                         *        command; the ratification will not be requested in the Close Session command. On the
                         *        contrary, if the communication mode is CONTACTS, no ratification command will be sent
                         *        to the PO and ratification will be requested in the Close Session command
                         * @param channelState indicates if the SE channel of the PO reader must be closed after the
                         *        last command
                         * @return SeResponse close session response
                         * @throws KeypleReaderException the IO reader exception This method is deprecated.
                         *         <ul>
                         *         <li>The argument of the ratification command is replaced by an indication of the PO
                         *         communication mode.</li>
                         *         </ul>
                         */
                        std::shared_ptr<SeResponse> processAtomicClosing(std::vector<std::shared_ptr<PoModificationCommand>> &poModificationCommands, TransmissionMode transmissionMode, ChannelState channelState) throw(KeypleReaderException);

                        /**
                         * Gets the PO Revision.
                         *
                         * @return the PoPlainSecureSession_OLD.poRevision
                         */
                    public:
                        PoRevision getRevision();

                        /**
                         * Get the Secure Session Status.
                         * <ul>
                         * <li>To check the result of a closed secure session, returns true if the SAM Digest
                         * Authenticate is successful.</li>
                         * </ul>
                         *
                         * @return the {@link PoTransaction}.transactionResult
                         */
                        bool isSuccessful();

                        /**
                         * Get the PO KIF
                         * 
                         * @return the PO KIF byte
                         */
                        char getPoKif();

                        /**
                         * Get the ratification status obtained at Session Opening
                         * 
                         * @return true or false
                         */
                        bool wasRatified();

                        /**
                         * Get the data read at Session Opening
                         * 
                         * @return a byte array containing the data
                         */
                        std::vector<char> getOpenRecordDataRead();

                        /**
                         * List of SAM settings keys that can be provided when the secure session is created.
                         */
                    public:
                        enum class SamSettings {
                            /** KIF for personalization used when not provided by the PO */
                            SAM_DEFAULT_KIF_PERSO,
                            /** KIF for load used when not provided by the PO */
                            SAM_DEFAULT_KIF_LOAD,
                            /** KIF for debit used when not provided by the PO */
                            SAM_DEFAULT_KIF_DEBIT,
                            /** Key record number to use when KIF/KVC is unavailable */
                            SAM_DEFAULT_KEY_RECORD_NUMBER
                        };

                        /**
                         * The PO Transaction Access Level: personalization, loading or debiting.
                         */
                    public:
                        enum class SessionAccessLevel {
                            /** Session Access Level used for personalization purposes. */
                            SESSION_LVL_PERSO,
                            /** Session Access Level used for reloading purposes. */
                            SESSION_LVL_LOAD,
                            /** Session Access Level used for validating and debiting purposes. */
                            SESSION_LVL_DEBIT
                        };

                        /**
                         * The modification mode indicates whether the secure session can be closed and reopened to
                         * manage the limitation of the PO buffer memory.
                         */
                    public:
                        enum class ModificationMode {
                            /**
                             * The secure session is atomic. The consistency of the content of the resulting PO memory
                             * is guaranteed.
                             */
                            ATOMIC,
                            /**
                             * Several secure sessions can be chained (to manage the writing of large amounts of data).
                             * The resulting content of the PO's memory can be inconsistent if the PO is removed during
                             * the process.
                             */
                            MULTIPLE
                        };

                        /**
                         * The PO Transaction State defined with the elements: ‘IOError’, ‘SEInserted’ and ‘SERemoval’.
                         */
                    public:
                        enum class SessionState {
                            /** Initial state of a PO transaction. The PO must have been previously selected. */
                            SESSION_CLOSED,
                            /** The secure session is active. */
                            SESSION_OPEN
                        };

                        /**
                         * This class embeds all the resources to manage the secure session digest computation.
                         *
                         * - initialize: Digest Init command
                         *
                         * - pushPoExchangeData and appendResponse: check consistency and all needed Digest Update
                         * commands
                         *
                         * - getTerminalSignature: Digest Close, returns the terminal part of the signature
                         *
                         * - checkPoSignature: Digest Authenticate, verify the PO part of the signature
                         */
                    private:
                        class DigestProcessor : public std::enable_shared_from_this<DigestProcessor> {
                            /*
                             * The digest data cache stores all PO data to be send to SAM during a Secure Session. The
                             * 1st buffer is the data buffer to be provided with Digest Init. The following buffers are
                             * PO command/response pairs
                             */
                        private:
                            static const std::vector<std::vector<char>> poDigestDataCache;
                            static SamRevision samRevision;
                            static PoRevision poRevision;
                            static bool encryption;
                            static bool verification;
                            static bool revMode;
                            static char keyRecordNumber;
                            static char keyKIF;
                            static char keyKVC;

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
                            static void initialize(PoRevision poRev, SamRevision samRev, bool sessionEncryption, bool verificationMode, bool rev3_2Mode, char workKeyRecordNumber, char workKeyKif, char workKeyKVC, std::vector<char> &digestData);

                            /**
                             * Appends a full PO exchange (request and response) to the digest data cache.
                             *
                             * @param request PO request
                             * @param response PO response
                             */
                            static void pushPoExchangeData(std::shared_ptr<ApduRequest> request, std::shared_ptr<ApduResponse> response);

                            /**
                             * Get a unique SAM request for the whole digest computation process.
                             * 
                             * @return SeRequest all the ApduRequest to send to the SAM in order to get the terminal
                             *         signature
                             */
                            // TODO optimization with the use of Digest Update Multiple whenever possible.
                            static std::shared_ptr<SeRequest> getSamDigestRequest();
                        };

                        /**
                         * The class handles the anticipated response computation.
                         */
                    private:
                        class AnticipatedResponseBuilder : public std::enable_shared_from_this<AnticipatedResponseBuilder> {
                            /**
                             * A nested class to associate a request with a response
                             */
                        private:
                            class CommandResponse : public std::enable_shared_from_this<CommandResponse> {
                            private:
                                const std::shared_ptr<ApduRequest> apduRequest;
                                const std::shared_ptr<ApduResponse> apduResponse;

                            public:
                                CommandResponse(std::shared_ptr<ApduRequest> apduRequest, std::shared_ptr<ApduResponse> apduResponse);

                                virtual std::shared_ptr<ApduRequest> getApduRequest();

                                virtual std::shared_ptr<ApduResponse> getApduResponse();
                            };

                            /**
                             * A Map of SFI and Commands/Responses
                             */
                        private:
                            static std::unordered_map<Byte, std::shared_ptr<CommandResponse>> sfiCommandResponseHashMap;

                            /**
                             * Store all Read Record exchanges in a Map whose key is the SFI.
                             * 
                             * @param poSendableInSessions the list of commands sent to the PO
                             * @param apduRequests the sent apduRequests
                             * @param apduResponses the received apduResponses
                             * @param skipFirstItem a flag to indicate if the first apduRequest/apduResponse pair has to
                             *        be ignored or not.
                             */
                        public:
                            static void storeCommandResponse(std::vector<std::shared_ptr<PoSendableInSession>> &poSendableInSessions, std::vector<std::shared_ptr<ApduRequest>> &apduRequests, std::vector<std::shared_ptr<ApduResponse>> &apduResponses, Boolean skipFirstItem);

                            /**
                             * Establish the anticipated responses to commands provided in poModificationCommands.
                             * <p>
                             * Append Record and Update Record commands return 9000
                             * <p>
                             * Increase and Decrease return NNNNNN9000 where NNNNNNN is the new counter value.
                             * <p>
                             * NNNNNN is determine with the current value of the counter (extracted from the Read Record
                             * responses previously collected) and the value to add or subtract provided in the command.
                             * <p>
                             * The SFI field is used to determine which data should be used to extract the needed
                             * information.
                             *
                             * @param poModificationCommands the modification command list
                             * @return the anticipated responses.
                             * @throws KeypleCalypsoSecureSessionException if an response can't be determined.
                             */
                            static std::vector<std::shared_ptr<ApduResponse>> getResponses(std::vector<std::shared_ptr<PoModificationCommand>> &poModificationCommands) throw(KeypleCalypsoSecureSessionException);
                        };

                        /**
                         * Open a Secure Session.
                         * <ul>
                         * <li>The PO must have been previously selected, so a logical channel with the PO application
                         * must be already active.</li>
                         * <li>The PO serial &amp; revision are identified from FCI data.</li>
                         * <li>A first request is sent to the SAM session reader.
                         * <ul>
                         * <li>In case not logical channel is active with the SAM, a channel is open.</li>
                         * <li>Then a Select Diversifier (with the PO serial) &amp; a Get Challenge are automatically
                         * operated. The SAM challenge is recovered.</li>
                         * </ul>
                         * </li>
                         * <li>The PO Open Session command is built according to the PO revision, the SAM challenge, the
                         * keyIndex, and openingSfiToSelect / openingRecordNumberToRead.</li>
                         * <li>Next the PO reader is requested:
                         * <ul>
                         * <li>for the currently selected PO, with channelState set to KEEP_OPEN,</li>
                         * <li>and some PO Apdu Requests including at least the Open Session command and all prepared PO
                         * command to operate inside the session.</li>
                         * </ul>
                         * </li>
                         * <li>The session PO keyset reference is identified from the PO Open Session response, the PO
                         * challenge is recovered too.</li>
                         * <li>According to the PO responses of Open Session and the PO commands sent inside the
                         * session, a "cache" of SAM commands is filled with the corresponding Digest Init &amp; Digest
                         * Update commands.</li>
                         * <li>All parsers returned by the prepare command methods are updated with the Apdu responses
                         * from the PO.</li>
                         * </ul>
                         *
                         * @param modificationMode the modification mode: ATOMIC or MULTIPLE (see
                         *        {@link ModificationMode})
                         * @param accessLevel access level of the session (personalization, load or debit).
                         * @param openingSfiToSelect SFI of the file to select (0 means no file to select)
                         * @param openingRecordNumberToRead number of the record to read
                         * @return true if all commands are successful
                         * @throws KeypleReaderException the IO reader exception
                         */
                    public:
                        bool processOpening(ModificationMode modificationMode, SessionAccessLevel accessLevel, char openingSfiToSelect, char openingRecordNumberToRead) throw(KeypleReaderException);

                        /**
                         * Process all prepared PO commands in a Secure Session.
                         * <ul>
                         * <li>On the PO reader, generates a SeRequest for the current selected AID, with channelState
                         * set to KEEP_OPEN, and ApduRequests with the PO commands.</li>
                         * <li>In case the secure session is active, the "cache" of SAM commands is completed with the
                         * corresponding Digest Update commands.</li>
                         * <li>All parsers returned by the prepare command methods are updated with the Apdu responses
                         * from the PO.</li>
                         * </ul>
                         *
                         * @param channelState indicates if the SE channel of the PO reader must be closed after the
                         *        last command
                         * @return true if all commands are successful
                         *
                         * @throws KeypleReaderException IO Reader exception
                         */
                        bool processPoCommands(ChannelState channelState) throw(KeypleReaderException);

                        /**
                         * Sends the currently prepared commands list (may be empty) and closes the Secure Session.
                         * <ul>
                         * <li>The ratification is handled according to the communication mode.</li>
                         * <li>The logical channel can be left open or closed.</li>
                         * <li>All parsers returned by the prepare command methods are updated with the Apdu responses
                         * from the PO.</li>
                         * </ul>
                         * 
                         * @param transmissionMode the communication mode. If the communication mode is CONTACTLESS, a
                         *        ratification command will be generated and sent to the PO after the Close Session
                         *        command; the ratification will not be requested in the Close Session command. On the
                         *        contrary, if the communication mode is CONTACTS, no ratification command will be sent
                         *        to the PO and ratification will be requested in the Close Session command
                         * @param channelState indicates if the SE channel of the PO reader must be closed after the
                         *        last command
                         * @return true if all commands are successful
                         * @throws KeypleReaderException the IO reader exception This method is deprecated.
                         *         <ul>
                         *         <li>The argument of the ratification command is replaced by an indication of the PO
                         *         communication mode.</li>
                         *         </ul>
                         */
                        bool processClosing(TransmissionMode transmissionMode, ChannelState channelState) throw(KeypleReaderException);

                        /**
                         * Loops on the SeResponse and updates the list of parsers pointed out by the provided iterator
                         * 
                         * @param seResponse the seResponse from the PO
                         * @param parserIterator the parser list iterator
                         * @return false if one or more of the commands do not succeed
                         */
                    private:
                        bool updateParsersWithResponses(std::shared_ptr<SeResponse> seResponse, std::shared_ptr<Iterator<std::shared_ptr<AbstractApduResponseParser>>> parserIterator);

                        /**
                         * Checks whether the requirement for the modifications buffer of the command provided in
                         * argument is compatible with the current usage level of the buffer.
                         * <p>
                         * If it is compatible, the requirement is subtracted from the current level and the method
                         * returns false. If this is not the case, the method returns true.
                         * 
                         * @param modificationCommand the modification command
                         * @return true or false
                         */
                        bool willOverflowBuffer(std::shared_ptr<PoModificationCommand> modificationCommand);

                        /**
                         * Initialized the modifications buffer counter to its maximum value for the current PO
                         */
                        void resetModificationsBufferCounter();

                        /**
                         * Internal method to handle expectedLength checks in public variants
                         * 
                         * @param sfi the sfi top select
                         * @param readDataStructureEnum read mode enum to indicate a SINGLE, MULTIPLE or COUNTER read
                         * @param firstRecordNumber the record number to read (or first record to read in case of
                         *        several records)
                         * @param expectedLength the expected length of the record(s)
                         * @param extraInfo extra information included in the logs (can be null or empty)
                         * @return ReadRecordsRespPars the ReadRecords command response parser
                         * @throws java.lang.IllegalArgumentException - if record number &lt; 1
                         * @throws java.lang.IllegalArgumentException - if the request is inconsistent
                         */
                        std::shared_ptr<ReadRecordsRespPars> prepareReadRecordsCmdInternal(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, int expectedLength, const std::string &extraInfo);

                        /**
                         * Builds a ReadRecords command and add it to the list of commands to be sent with the next
                         * process command.
                         * <p>
                         * The expected length is provided and its value is checked between 1 and 250.
                         * <p>
                         * Returns the associated response parser.
                         *
                         * @param sfi the sfi top select
                         * @param readDataStructureEnum read mode enum to indicate a SINGLE, MULTIPLE or COUNTER read
                         * @param firstRecordNumber the record number to read (or first record to read in case of
                         *        several records)
                         * @param expectedLength the expected length of the record(s)
                         * @param extraInfo extra information included in the logs (can be null or empty)
                         * @return ReadRecordsRespPars the ReadRecords command response parser
                         * @throws java.lang.IllegalArgumentException - if record number &lt; 1
                         * @throws java.lang.IllegalArgumentException - if the request is inconsistent
                         */
                    public:
                        std::shared_ptr<ReadRecordsRespPars> prepareReadRecordsCmd(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, int expectedLength, const std::string &extraInfo);

                        /**
                         * Builds a ReadRecords command and add it to the list of commands to be sent with the next
                         * process command. No expected length is specified, the record output length is handled
                         * automatically.
                         * <p>
                         * Returns the associated response parser.
                         *
                         * @param sfi the sfi top select
                         * @param readDataStructureEnum read mode enum to indicate a SINGLE, MULTIPLE or COUNTER read
                         * @param firstRecordNumber the record number to read (or first record to read in case of
                         *        several records)
                         * @param extraInfo extra information included in the logs (can be null or empty)
                         * @return ReadRecordsRespPars the ReadRecords command response parser
                         * @throws java.lang.IllegalArgumentException - if record number &lt; 1
                         * @throws java.lang.IllegalArgumentException - if the request is inconsistent
                         */
                        std::shared_ptr<ReadRecordsRespPars> prepareReadRecordsCmd(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, const std::string &extraInfo);

                        /**
                         * Builds an AppendRecord command and add it to the list of commands to be sent with the next
                         * process command.
                         * <p>
                         * Returns the associated response parser.
                         *
                         * @param sfi the sfi to select
                         * @param newRecordData the new record data to write
                         * @param extraInfo extra information included in the logs (can be null or empty)
                         * @return AppendRecordRespPars the AppendRecord command response parser
                         * @throws java.lang.IllegalArgumentException - if the command is inconsistent
                         */
                        std::shared_ptr<AppendRecordRespPars> prepareAppendRecordCmd(char sfi, std::vector<char> &newRecordData, const std::string &extraInfo);

                        /**
                         * Builds an UpdateRecord command and add it to the list of commands to be sent with the next
                         * process command
                         * <p>
                         * Returns the associated response parser.
                         *
                         * @param sfi the sfi to select
                         * @param recordNumber the record number to update
                         * @param newRecordData the new record data to write
                         * @param extraInfo extra information included in the logs (can be null or empty)
                         * @return UpdateRecordRespPars the UpdateRecord command response parser
                         * @throws java.lang.IllegalArgumentException - if record number is &lt; 1
                         * @throws java.lang.IllegalArgumentException - if the request is inconsistent
                         */
                        std::shared_ptr<UpdateRecordRespPars> prepareUpdateRecordCmd(char sfi, char recordNumber, std::vector<char> &newRecordData, const std::string &extraInfo);

                        /**
                         * Builds a Increase command and add it to the list of commands to be sent with the next process
                         * command
                         * <p>
                         * Returns the associated response parser.
                         *
                         * @param counterNumber &gt;= 01h: Counters file, number of the counter. 00h: Simulated Counter
                         *        file.
                         * @param sfi SFI of the file to select or 00h for current EF
                         * @param incValue Value to add to the counter (defined as a positive int &lt;= 16777215
                         *        [FFFFFFh])
                         * @param extraInfo extra information included in the logs (can be null or empty)
                         * @return IncreaseRespPars the Increase command response parser
                         * @throws java.lang.IllegalArgumentException - if the decrement value is out of range
                         * @throws java.lang.IllegalArgumentException - if the command is inconsistent
                         */
                        std::shared_ptr<IncreaseRespPars> prepareIncreaseCmd(char sfi, char counterNumber, int incValue, const std::string &extraInfo);

                        /**
                         * Builds a Decrease command and add it to the list of commands to be sent with the next process
                         * command
                         * <p>
                         * Returns the associated response parser.
                         *
                         * @param counterNumber &gt;= 01h: Counters file, number of the counter. 00h: Simulated Counter
                         *        file.
                         * @param sfi SFI of the file to select or 00h for current EF
                         * @param decValue Value to subtract to the counter (defined as a positive int &lt;= 16777215
                         *        [FFFFFFh])
                         * @param extraInfo extra information included in the logs (can be null or empty)
                         * @return DecreaseRespPars the Decrease command response parser
                         * @throws java.lang.IllegalArgumentException - if the decrement value is out of range
                         * @throws java.lang.IllegalArgumentException - if the command is inconsistent
                         */
                        std::shared_ptr<DecreaseRespPars> prepareDecreaseCmd(char sfi, char counterNumber, int decValue, const std::string &extraInfo);
                    };

                }
            }
        }
    }
}

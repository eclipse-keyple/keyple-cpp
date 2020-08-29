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
#include <memory>

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "CalypsoBuilderParser.h"
#include "CalypsoPo.h"
#include "ChannelControl.h"
#include "KeypleCalypsoExport.h"
#include "PoBuilderParser.h"
#include "PoModificationCommand.h"
#include "PoResource.h"
#include "SamResource.h"
#include "SamRevision.h"
#include "SecuritySettings.h"
#include "SelectFileCmdBuild.h"

/* Core */
#include "ProxyReader.h"
#include "ReadDataStructure.h"
#include "SeProtocol.h"
#include "SeRequest.h"
#include "TransmissionMode.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::calypso::command::sam;
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
class KEYPLECALYPSO_API PoTransaction final {
public:
    static class SessionSetting {
    public:
        /**
         * The modification mode indicates whether the secure session can be
         * closed and reopened to manage the limitation of the PO buffer memory.
         */
        enum class ModificationMode {
            /**
             * The secure session is atomic. The consistency of the content of
             * the resulting PO memory is guaranteed.
             */
            ATOMIC,
            /**
             * Several secure sessions can be chained (to manage the writing of
             * large amounts of data). The resulting content of the PO's memory
             * can be inconsistent if the PO is removed during the process.
             */
            MULTIPLE
        };

        /**
         * The PO Transaction Access Level: personalization, loading or debiting
         */
        class KEYPLECALYPSO_API AccessLevel final {
        public:
            /**
             * Session Access Level used for personalization purposes.
             */
            static AccessLevel SESSION_LVL_PERSO;

            /**
             * Session Access Level used for reloading purposes
             */
            static AccessLevel SESSION_LVL_LOAD;

            /**
             * Session Access Level used for validating and debiting purposes
             */
            static AccessLevel SESSION_LVL_DEBIT;

            /**
             *
             */
            const std::string& getName() const;

            /**
             *
             */
            const uint8_t getSessionKey() const;

            /**
             *
             */
            bool operator==(const AccessLevel& other) const;

            /**
             *
             */
            bool operator!=(const AccessLevel& other) const;

        private:
            /**
             *
             */
            const std::string mName;

            /**
             *
             */
            const uint8_t mSessionKey;
        };

        /**
         * The ratification mode defines the behavior of processClosing
         * regarding the ratification process.
         */
        enum class RatificationMode {
            CLOSE_RATIFIED,
            CLOSE_NOT_RATIFIED
        };
    };

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
     * @param poSecuritySettings a list of security settings ({@link
     *        PoSecuritySettings}) used in
     */
    PoTransaction(std::shared_ptr<SeResource<CalypsoPo> poResource,
                  std::shared_ptr<PoSecuritySettings> poSecuritySettings);

    /**
     * PoTransaction with PO reader and without SAM reader.
     * <ul>
     * <li>Logical channels with PO could already be established or not.</li>
     * </ul>
     *
     * @param poResource the PO resource (combination of {@link SeReader} and
     *        {@link CalypsoPo})
     */
    PoTransaction(std::shared_ptr<SeResource<CalypsoPo> poResource);

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
     * @param accessLevel access level of the session (personalization, load or
     *        debit).
     * @throw CalypsoPoTransactionException if a functional error occurs
     *        (including PO and SAM IO errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     * @throw CalypsoSamCommandException if a response from the SAM was
     *        unexpected
     */
    void processOpening(const AccessLevel accessLevel);

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
     * @throw CalypsoPoTransactionException if a functional error occurs
     *        (including PO and SAM IO errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    void processPoCommands(const ChannelControl channelControl);

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
     * @throw CalypsoPoTransactionException if a functional error occurs
     *        (including PO and SAM IO errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     * @throw CalypsoSamCommandException if a response from the SAM was
     *        unexpected
     */
    void processPoCommandsInSession();

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
     * @throw CalypsoPoTransactionException if a functional error occurs
     *        (including PO and SAM IO errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     * @throw CalypsoSamCommandException if a response from the SAM was
     *        unexpected
     */
    void processClosing(const ChannelControl channelControl);

    /**
     * Abort a Secure Session.
     * <p>
     * Send the appropriate command to the PO
     * <p>
     * Clean up internal data and status.
     *
     * @param channelControl indicates if the SE channel of the PO reader must be
     *        closed after the abort session command
     * @throw CalypsoPoTransactionException if a functional error occurs
     *        (including PO and SAM IO errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    void processCancel(const ChannelControl channelControl);

    /**
     * Prepare a select file ApduRequest to be executed following the selection.
     *
     * @param lid the LID of the EF to select
     */
    void prepareSelectFile(const std::vector<uint8_>& lid);

    /**
     * Prepare a select file ApduRequest to be executed following the selection.
     *
     * @param control provides the navigation case: FIRST, NEXT or CURRENT
     */
    void prepareSelectFile(const SelectFileControl control);

    /**
     * Read a single record from the indicated EF
     *
     * @param sfi the SFI of the EF to read
     * @param recordNumber the record number to read
     * @throw IllegalArgumentException if one of the provided argument is out of
     *        range
     */
    void prepareReadRecordFile(const uint8_t sfi, const int recordNumber);

    /**
     * Read one or more records from the indicated EF
     *
     * @param sfi the SFI of the EF
     * @param firstRecordNumber the record number to read (or first record to
     *        read in case of several records)
     * @param numberOfRecords the number of records expected
     * @param recordSize the record length
     * @throw IllegalArgumentException if one of the provided argument is out of
     *        range
     */
    void prepareReadRecordFile(const uint8_t sfi,
                               const int firstRecordNumber,
                               const int numberOfRecords,
                               const int recordSize);

    /**
     * Read a record of the indicated EF, which should be a count file.
     * <p>
     * The record will be read up to the counter location indicated in
     * parameter.<br>
     * Thus all previous counters will also be read.
     *
     * @param sfi the SFI of the EF
     * @param countersNumber the number of the last counter to be read
     * @throw IllegalArgumentException if one of the provided argument is out of
     *        range
     */
    void prepareReadCounterFile(const uint8_t sfi, const int countersNumber);

    /**
     * Builds an AppendRecord command and add it to the list of commands to be
     * sent with the next process command.
     * <p>
     * Returns the associated response parser.
     *
     * @param sfi the sfi to select
     * @param recordData the new record data to write
     * @throw IllegalArgumentException - if the command is inconsistent
     */
    void prepareAppendRecord(const uint8_t sfi,
                             const std::vector<uint8_t>& recordData);

    /**
     * Builds an UpdateRecord command and add it to the list of commands to be
     * sent with the next process command
     * <p>
     * Returns the associated response parser index.
     *
     * @param sfi the sfi to select
     * @param recordNumber the record number to update
     * @param recordData the new record data. If length &lt; RecSize, bytes
     *        beyond length are left unchanged.
     * @throw IllegalArgumentException - if record number is &lt; 1
     * @throw IllegalArgumentException - if the request is inconsistent
     */
    void prepareUpdateRecord(const uint8_t sfi,
                             const int recordNumber,
                             const std::vector<uint8_t>& recordData);


    /**
     * Builds an WriteRecord command and add it to the list of commands to be
     * sent with the next process command
     * <p>
     * Returns the associated response parser index.
     *
     * @param sfi the sfi to select
     * @param recordNumber the record number to write
     * @param recordData the data to overwrite in the record. If length &lt;
     *        RecSize, bytes beyond length are left unchanged.
     * @throw IllegalArgumentException - if record number is &lt; 1
     * @throw IllegalArgumentException - if the request is inconsistent
     */
    void prepareWriteRecord(const uint8_t sfi,
                            const int recordNumber,
                            const std::vector<uint8_t>& recordData);

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
     * @throw IllegalArgumentException - if the decrement value is out of range
     * @throw IllegalArgumentException - if the command is inconsistent
     */
    void prepareIncreaseCounter(const uint8_t sfi,
                                const int counterNumber,
                                const int incValue);

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
     * @throw IllegalArgumentException - if the decrement value is out of range
     * @throw IllegalArgumentException - if the command is inconsistent
     */
    void prepareDecreaseCounter(const uint8_t sfi,
                                const int counterNumber,
                                const int decValue);

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

    /**
     * Commands that modify the content of the PO in session have a cost on the
     * session buffer equal to the length of the outgoing data plus 6 bytes
     */
    static const int SESSION_BUFFER_CMD_ADDITIONAL_COST = 6;
    static const int APDU_HEADER_LENGTH = 5;

    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(PoTransaction));

    /**
     * The reader for PO
     */
    const std::shared_ptr<ProxyReader> mPoReader;

    /**
     * The PO security settings used to manage the secure session
     */
    std::shared_ptr<PoSecuritySettings> mPoSecuritySettings;

    /**
     * The SAM commands processor
     */
    std::shared_ptr<SamCommandProcessor> mSamCommandProcessor;

    /**
     * The current CalypsoPo
     */
    const std::shared_ptr<CalypsoPo> mCalypsoPo;

    /**
     * The type of the notified event
     */
    SessionState mSessionState;

    /**
     * The current secure session access level: PERSO, RELOAD, DEBIT
     */
    AccessLevel mCurrentAccessLevel;

    /**
     * Modifications counter management
     */
    int mModificationsCounter;

    /**
     *
     */
    PoCommandManager mPoCommandManager;

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
     * <li>Returns the corresponding PO SeResponse (responses to poCommands).
     * </li>
     * </ul>
     *
     * @param accessLevel access level of the session (personalization, load or
     *        debit).
     * @param poCommands the po commands inside session
     * @throw CalypsoPoTransactionException if a functional error occurs
     *        (including PO and SAM IO errors)
     * @throw CalypsoPoCommandException if a response from the PO was
     *        unexpected
     * @throw CalypsoSamCommandException if a response from the SAM was
     *        unexpected
     */
    void processAtomicOpening(
        const std::shared_ptr<AccessLevel>,
        std::vector<std::shared_ptr<AbstractPoCommandBuilder<
            AbstractPoResponseParser>>>& poCommands);

    /**
     * Create an ApduRequest List from a AbstractPoCommandBuilder List.
     *
     * @param poCommands a list of PO commands
     * @return the ApduRequest list
     */
    std::vector<std::shared_ptr<ApduRequest>> getApduRequests(
        const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
            AbstractPoResponseParser>>>& poCommands) const;

    /**
     * Process PO commands in a Secure Session.
     * <ul>
     * <li>On the PO reader, generates a SeRequest with channelControl set to
     * KEEP_OPEN, and ApduRequests with the PO commands.</li>
     * <li>In case the secure session is active, the "cache" of SAM commands is
     * completed with the corresponding Digest Update commands.</li>
     * <li>If a session is open and channelControl is set to CLOSE_AFTER, the
     * current PO session is aborted</li>
     * <li>Returns the corresponding PO SeResponse.</li>
     * </ul>
     *
     * @param poBuilderParsers the po commands inside session
     * @param channelControl indicated if the SE channel of the PO reader must
     *        be closed after the last command
     * @return SeResponse all responses to the provided commands
     *
     * @throw KeypleReaderException IO Reader exception
     */
    void processAtomicPoCommands(
        const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
            AbstractPoCommandBuilder<AbstractPoResponseParser>>>& poCommands,
        const ChannelControl channelControl);


    /**
     * Close the Secure Session.
     * <ul>
     * <li>The SAM cache is completed with the Digest Update commands related to
     * the new PO commands to be sent and their anticipated responses. A Digest
     * Close command is also added to the SAM command cache.</li>
     * <li>On the SAM session reader side, a SeRequest is transmitted with SAM
     * commands from the command cache. The SAM command cache is emptied.</li>
     * <li>The SAM certificate is retrieved from the Digest Close response. The
     * terminal signature is identified.</li>
     * <li>Then, on the PO reader, a SeRequest is transmitted with the provided
     * channelControl, and apduRequests including the new PO commands to send in
     * the session, a Close Session command (defined with the SAM certificate),
     * and optionally a ratificationCommand.
     * <ul>
     * <li>The management of ratification is conditioned by the mode of
     * communication.
     * <ul>
     * <li>If the communication mode is CONTACTLESS, a specific ratification
     * command is sent after the Close Session command. No ratification is
     * requested in the Close Session command.</li>
     * <li>If the communication mode is CONTACTS, no ratification command is
     * sent after the Close Session command. Ratification is requested in the
     * Close Session command.</li>
     * </ul>
     * </li>
     * <li>Otherwise, the PO Close Secure Session command is defined to directly
     * set the PO as ratified.</li>
     * </ul>
     * </li>
     * <li>The PO responses of the poModificationCommands are compared with the
     * poAnticipatedResponses. The PO signature is identified from the PO Close
     * Session response.</li>
     * <li>The PO certificate is recovered from the Close Session response. The
     * card signature is identified.</li>
     * <li>Finally, on the SAM session reader, a Digest Authenticate is
     * automatically operated in order to verify the PO signature.</li>
     * <li>Returns the corresponding PO SeResponse.</li>
     * </ul>
     *
     * The method is marked as deprecated because the advanced variant defined
     * below must be used at the application level.
     *
     * @param poModificationCommands a list of commands that can modify the PO
     *        memory content
     * @param poAnticipatedResponses a list of anticipated PO responses to the
     *        modification commands
     * @param ratificationMode the ratification mode tells if the session is
     *        closed ratified or not
     * @param channelControl indicates if the SE channel of the PO reader must
     *        be closed after the last command
     * @throw CalypsoPoTransactionException if a functional error occurs
     *        (including PO and SAM IO errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     * @throw CalypsoSamCommandException if a response from the SAM was
     *        unexpected
     */
    void processAtomicClosing(
        const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
            AbstractPoResponseParser>>>& poModificationCommands,
        const std::vector<std::shared_ptr<ApduResponse>>&
            poAnticipatedResponses,
        const SessionSetting::RatificationMode ratificationMode,
        const ChannelControl channelControl);

    /**
     * Advanced variant of processAtomicClosing in which the list of expected
     * responses is determined from previous reading operations.
     *
     * @param poCommands a list of commands that can modify the PO memory
     *        content
     * @param ratificationMode the ratification mode tells if the session is
     *        closed ratified or not
     * @param channelControl indicates if the SE channel of the PO reader must
     *        be closed after the last command
     * @throw CalypsoPoTransactionException if a functional error occurs
     *        (including PO and SAM IO errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     * @throw CalypsoSamCommandException if a response from the SAM was
     *        unexpected
     */
    void processAtomicClosing(
        const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
            AbstractPoResponseParser>>& poCommands,
        const RatificationMode ratificationMode,
        const ChannelControl channelControl);

    /**
     * Gets the value of the designated counter
     *
     * @param sfi the SFI of the EF containing the counter
     * @param counter the number of the counter
     * @return the value of the counter
     * @{@link CalypsoPo}
     */
    const int getCounterValue(const uint8_t sfi, const int counter) const;

    /**
     * Create an anticipated response to an Increase/Decrease command
     *
     * @param newCounterValue the anticipated counter value
     * @return an {@link ApduResponse} containing the expected bytes
     */
    std::unique_ptr<ApduResponse> createIncreaseDecreaseResponse(
        const int newCounterValue) const;

    /**
     * Get the anticipated response to the command sent in processClosing.<br>
     * These commands are supposed to be "modifying commands" i.e.
     * Increase/Decrease/UpdateRecord/WriteRecord ou AppendRecord.
     *
     * @param poCommands the list of PO commands sent
     * @return the list of the anticipated responses.
     * @throw CalypsoPoTransactionIllegalStateException if the anticipation
     *        process failed
     */
    std::vector<std::shared_ptr<ApduResponse>> getAnticipatedResponses(
        const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
            AbstractPoResponseParser>>>& poCommands) const;

    /**
     *
     */
    std::shared_ptr<SeResponse> safePoTransmit(
        const std::shared_ptr<SeRequest> poSeRequest,
        const ChannelControl channelControl);

    /**
     * Checks if a Secure Session is open, raises an exception if not
     *
     * @throw CalypsoPoTransactionIllegalStateException if no session is open
     */
    void checkSessionIsOpen() const;

    /**
     * Checks if a Secure Session is not open, raises an exception if not
     *
     * @throw CalypsoPoTransactionIllegalStateException if a session is open
     */
    void checkSessionIsNotOpen() const;

    /**
     * Checks if the number of responses matches the number of commands.<br>
     * Throw a {@link CalypsoDesynchronizedExchangesException} if not.
     *
     * @param commandsNumber the number of commands
     * @param responsesNumber the number of responses
     * @throwsCalypsoDesynchronizedExchangesException if the test failed
     */
    void checkCommandsResponsesSynchronization(const int commandsNumber,
                                               const int responsesNumber) const;

    /**
     * Checks the provided command from the session buffer overflow management
     * perspective<br>
     * A exception is raised if the session buffer is overflowed in ATOMIC
     * modification mode.<br>
     * Returns false if the command does not affect the session buffer.<br>
     * Sets the overflow flag and the neededSessionBufferSpace value according
     * to the characteristics of the command in other cases.
     *
     * @param builder the command builder
     * @param overflow flag set to true if the command overflowed the buffer
     * @param neededSessionBufferSpace updated with the size of the buffer
     *        consumed by the command
     * @return true if the command modifies the content of the PO, false if not
     * @throw CalypsoPoTransactionIllegalStateException if the command overflows
     *        the buffer in ATOMIC modification mode
     */
    bool checkModifyingCommand(
        std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>
            builder,
        std::atomic<bool> overflow,
        std::atomic<int> neededSessionBufferSpace);

    /**
     * Checks whether the requirement for the modifications buffer of the
     * command provided in argument is compatible with the current usage level
     * of the buffer.
     * <p>
     * If it is compatible, the requirement is subtracted from the current level
     * and the method returns false. If this is not the case, the method returns
     * true and the current level is left unchanged.
     *
     * @param sessionBufferSizeConsumed session buffer requirement
     * @return true or false
     */
    bool isSessionBufferOverflowed(const int sessionBufferSizeConsumed);

    /**
     * Initialized the modifications buffer counter to its maximum value for the
     *  current PO
     */
    void resetModificationsBufferCounter();
};

}
}
}

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

#include <atomic>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <memory>

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "ChannelControl.h"
#include "KeypleCalypsoExport.h"
#include "SamRevision.h"
#include "SelectFileCmdBuild.h"

/* Core */
#include "ProxyReader.h"
#include "SeProtocol.h"
#include "SeRequest.h"
#include "SeResource.h"
#include "TransmissionMode.h"

/* Forward declarations */
namespace keyple { namespace calypso { namespace transaction { class PoSecuritySettings; } } }
namespace keyple { namespace calypso { namespace transaction { class CalypsoPo; } } }
namespace keyple { namespace calypso { namespace transaction { class SamCommandProcessor; } } }
namespace keyple { namespace calypso { namespace transaction { class PoCommandManager; } } }

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::calypso::command::sam;
using namespace keyple::common;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;

/**
 * Portable Object Secure Session.
 *
 * A non-encrypted secure session with a Calypso PO requires the management of
 * two keyple::core::seproxy::message::ProxyReader in order to communicate with both a Calypso PO
 * and a SAM
 *
 * @author Calypso Networks Association
 */
class KEYPLECALYPSO_API PoTransaction final {
public:
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
     * The keyple::calypso::transaction::PoTransaction::PinTransmissionMode indicates whether the
     * PIN transmission is encrypted or not.
     */
    enum class PinTransmissionMode {
        PLAIN,
        ENCRYPTED
    };

    /**
     * A set of enumerations used to manage Stored Value transactions
     */
    class SvSettings {
    public:
        /**
         * keyple::calypso::transaction::PoTransaction::SvSettings::Operation specifies the type of
         * operation intended to be carried out
         */
        enum class Operation {
            NONE,
            /**
             * Increase the balance of the stored value
             */
            RELOAD,
            /**
             * Decrease the balance of the stored value
             */
            DEBIT
        };

        /**
         * keyple::calypso::transaction::PoTransaction::SvSettings::Action specifies the type of
         * action:
         * <ul>
         * <li>Reload: DO loads a positive amount, UNDO loads a negative amount
         * <li>Debit: DO debits a positive amount, UNDO cancels, totally or partially, a previous
         * debit.
         * </ul>
         */
        enum class Action {
            DO,
            UNDO
        };

        /**
         * keyple::calypso::transaction::PoTransaction::SvSettings::LogRead specifies whether only
         * the log related to the current operation is requested or whether both logs are requested.
         */
        enum class LogRead {
            /**
             * Request the RELOAD or DEBIT log according to the currently specified operation
             */
            SINGLE,
            /**
             * Request both RELOAD and DEBIT logs
             */
            ALL
        };

        /**
         * keyple::calypso::transaction::PoTransaction::SvSettings::NegativeBalance indicates
         * whether negative balances are allowed when debiting the SV
         */
        enum class NegativeBalance {
            /**
             * An SV exception will be raised if the attempted debit of the SV would result in a
             * negative balance.
             */
            FORBIDDEN,
            /**
             * Negative balance is allowed
             */
            AUTHORIZED
        };
    };

    class SessionSetting {
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
             * Default value (uninitialized)
             */
            static const AccessLevel SESSION_LVL_NONE;

            /**
             * Session Access Level used for personalization purposes.
             */
            static const AccessLevel SESSION_LVL_PERSO;

            /**
             * Session Access Level used for reloading purposes
             */
            static const AccessLevel SESSION_LVL_LOAD;

            /**
             * Session Access Level used for validating and debiting purposes
             */
            static const AccessLevel SESSION_LVL_DEBIT;

            /**
             *
             */
            const std::string& getName() const;

            /**
             *
             */
            uint8_t getSessionKey() const;

            /**
             *
             */
            bool operator==(const AccessLevel& o) const;

            /**
             *
             */
            bool operator!=(const AccessLevel& o) const;

            /**
             *
             */
            //AccessLevel& operator=(const AccessLevel& o);

            /**
             *
             */
            friend std::ostream& operator<<(std::ostream& os,
                                            const AccessLevel& al);

            /**
             *
             */
            friend bool operator<(const AccessLevel& lhs,
                                  const AccessLevel& rhs);

        private:
            /**
             *
             */
            std::string mName;

            /**
             *
             */
            uint8_t mSessionKey;

            /**
             *
             */
            AccessLevel(const std::string& name, const uint8_t sessionKey);
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
     * <li>Logical channels with PO &amp; SAM could already be established or not.</li>
     * <li>A list of SAM parameters is provided as en EnumMap.</li>
     * </ul>
     *
     * @param poResource the PO resource (combination of keyple::core::seproxy::SeReader and
     *        keyple::calypso::transaction::CalypsoPo)
     * @param poSecuritySettings a list of security settings
     *        (keyple::calypso::transaction::PoSecuritySettings) used in
     */
    PoTransaction(std::shared_ptr<SeResource<CalypsoPo>> poResource,
                  std::shared_ptr<PoSecuritySettings> poSecuritySettings);

    /**
     * PoTransaction with PO reader and without SAM reader.
     * <ul>
     * <li>Logical channels with PO could already be established or not.</li>
     * </ul>
     *
     * @param poResource the PO resource (combination of keyple::core::seproxy::SeReader and
     *        keyple::calypso::transaction::CalypsoPo)
     */
    PoTransaction(std::shared_ptr<SeResource<CalypsoPo>> poResource);

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
     * <li>for the currently selected PO, with channelControl set to KEEP_OPEN,</li>
     * <li>and some PO Apdu Requests including at least the Open Session command and all prepared PO
     * command to operate inside the session.</li>
     * </ul>
     * </li>
     * <li>The session PO keyset reference is identified from the PO Open Session response, the PO
     * challenge is recovered too.</li>
     * <li>According to the PO responses of Open Session and the PO commands sent inside the
     * session, a "cache" of SAM commands is filled with the corresponding Digest Init &amp; Digest
     * Update commands.</li>
     * <li>The result of the commands is placed in CalypsoPo.</li>
     * <li>Any call to prepareReleasePoChannel before this command will be ignored but will remain
     * active for the next process command.</li>
     * </ul>
     *
     * @param accessLevel access level of the session (personalization, load or debit).
     * @throw CalypsoPoTransactionException if a functional error occurs (including PO and SAM IO
     *         errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     * @throw CalypsoSamCommandException if a response from the SAM was unexpected
     */
    void processOpening(const SessionSetting::AccessLevel& accessLevel);

    /**
     * Sends the currently prepared commands list (may be empty) and closes the Secure Session.
     * <ul>
     * <li>The ratification is handled according to the communication mode.</li>
     * <li>The logical channel can be left open or closed.</li>
     * <li>The result of the commands is placed in CalypsoPo.</li>
     * </ul>
     *
     * <p>
     * The communication mode is retrieved from CalypsoPO to manage the ratification process. If the
     * communication mode is CONTACTLESS, a ratification command will be generated and sent to the
     * PO after the Close Session command; the ratification will not be requested in the Close
     * Session command. On the contrary, if the communication mode is CONTACTS, no ratification
     * command will be sent to the PO and ratification will be requested in the Close Session
     * command
     *
     * @throw CalypsoPoTransactionException if a functional error occurs (including PO and SAM IO
     *         errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     * @throw CalypsoSamCommandException if a response from the SAM was unexpected
     */
    void processClosing();

    /**
     * Process all prepared PO commands outside or in a Secure Session.
     * <ul>
     * <li>In case the secure session is active, the "cache" of SAM commands is completed with the
     * corresponding Digest Update commands. Also, the PO channel is kept open.</li>
     * <li>Outside of a secure session, the PO channel is closed depending on whether or not
     * prepareReleasePoChannel has been called.</li>
     * <li>The result of the commands is placed in CalypsoPo.</li>
     * </ul>
     *
     * @throw CalypsoPoTransactionException if a functional error occurs (including PO and SAM IO
     *         errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     * @throw CalypsoSamCommandException if a response from the SAM was unexpected
     */
    void processPoCommands();

    /**
     * Abort a Secure Session.
     * <p>
     * Send the appropriate command to the PO
     * <p>
     * Clean up internal data and status.
     *
     * @throw CalypsoPoTransactionException if a functional error occurs (including PO and SAM IO
     *         errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    void processCancel();

    /**
     * Performs a PIN verification, in order to authenticate the cardholder and/or unlock access to
     * certain PO files.<br>
     * This command can be performed both in and out of a secure session.<br>
     * The PIN code can be transmitted in plain text or encrypted according to the parameter set in
     * PoSecuritySettings (by default the transmission is encrypted).<br>
     * If the execution is done out of session but an encrypted transmission is requested, then
     * PoTransaction must be constructed with {@link PoSecuritySettings}<br>
     * If PoTransaction is constructed without {@link PoSecuritySettings} the transmission in done
     * in plain.<br>
     * The PO channel is closed if prepareReleasePoChannel is called before this command.
     *
     * @param pin the PIN code value (4-byte long byte array)
     * @throw CalypsoPoTransactionException if a functional error occurs (including PO and SAM IO
     *         errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     * @throw CalypsoPoPinException if the PIN presentation failed (the remaining attempt counter
     *         is update in Calypso). See {@link CalypsoPo#isPinBlocked} and
     *         {@link CalypsoPo#getPinAttemptRemaining} methods
     * @throw CalypsoPoTransactionIllegalStateException if the PIN feature is not available for
     *         this PO or if commands have been prepared before calling this process method.
     */
    void processVerifyPin(const std::vector<uint8_t>& pin);

    /**
     * ProcessVerifyPin variant with the PIN supplied as an ASCII string.<br>
     * E.g. "1234" will be transmited as { 0x31,032,0x33,0x34 }
     *
     * @param pin an ASCII string (4-character long)
     */
    void processVerifyPin(const std::string& pin);

    /**
     * Prepare a select file ApduRequest to be executed following the selection.
     *
     * @param lid the LID of the EF to select
     */
    void prepareSelectFile(const std::vector<uint8_t>& lid);

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
     * @throw IllegalArgumentException if one of the provided argument is out of range
     */
    void prepareReadRecordFile(const uint8_t sfi, const uint8_t recordNumber);

    /**
     * Read one or more records from the indicated EF
     *
     * @param sfi the SFI of the EF
     * @param firstRecordNumber the record number to read (or first record to read in case of
              several records)
     * @param numberOfRecords the number of records expected
     * @param recordSize the record length
     * @throw IllegalArgumentException if one of the provided argument is out of range
     */
    void prepareReadRecordFile(const uint8_t sfi,
                               const uint8_t firstRecordNumber,
                               const uint8_t numberOfRecords,
                               const uint8_t recordSize);

    /**
     * Read a record of the indicated EF, which should be a count file.
     * <p>
     * The record will be read up to the counter location indicated in
     * parameter.<br>
     * Thus all previous counters will also be read.
     *
     * @param sfi the SFI of the EF
     * @param countersNumber the number of the last counter to be read
     * @throw IllegalArgumentException if one of the provided argument is out of range
     */
    void prepareReadCounterFile(const uint8_t sfi, const uint8_t countersNumber);

    /**
     * Builds an AppendRecord command and add it to the list of commands to be sent with the next
     * process command.
     * <p>
     *
     * @param sfi the sfi to select
     * @param recordData the new record data to write
     * @throw IllegalArgumentException - if the command is inconsistent
     */
    void prepareAppendRecord(const uint8_t sfi, const std::vector<uint8_t>& recordData);

    /**
     * Builds an UpdateRecord command and add it to the list of commands to be sent with the next
     * process command
     * <p>
     *
     * @param sfi the sfi to select
     * @param recordNumber the record number to update
     * @param recordData the new record data. If length &lt; RecSize, bytes beyond length are left
     *        unchanged.
     * @throw IllegalArgumentException - if record number is &lt; 1
     * @throw IllegalArgumentException - if the request is inconsistent
     */
    void prepareUpdateRecord(const uint8_t sfi,
                             const uint8_t recordNumber,
                             const std::vector<uint8_t>& recordData);


    /**
     * Builds an WriteRecord command and add it to the list of commands to be sent with the next
     * process command
     * <p>
     *
     * @param sfi the sfi to select
     * @param recordNumber the record number to write
     * @param recordData the data to overwrite in the record. If length &lt; RecSize, bytes beyond
     *        length are left unchanged.
     * @throw IllegalArgumentException - if record number is &lt; 1
     * @throw IllegalArgumentException - if the request is inconsistent
     */
    void prepareWriteRecord(const uint8_t sfi,
                            const uint8_t recordNumber,
                            const std::vector<uint8_t>& recordData);

    /**
     * Builds a Increase command and add it to the list of commands to be sent with the next process
     * command
     * <p>
     *
     * @param counterNumber &gt;= 01h: Counters file, number of the counter. 00h: Simulated Counter
     *        file.
     * @param sfi SFI of the file to select or 00h for current EF
     * @param incValue Value to add to the counter (defined as a positive int &lt;= 16777215
     *        [FFFFFFh])
     * @throw IllegalArgumentException - if the decrement value is out of range
     * @throw IllegalArgumentException - if the command is inconsistent
     */
    void prepareIncreaseCounter(const uint8_t sfi, const uint8_t counterNumber, const int incValue);

    /**
     * Builds a Decrease command and add it to the list of commands to be sent with the next process
     * command
     * <p>
     *
     * @param counterNumber &gt;= 01h: Counters file, number of the counter. 00h: Simulated Counter
     *        file.
     * @param sfi SFI of the file to select or 00h for current EF
     * @param decValue Value to subtract to the counter (defined as a positive int &lt;= 16777215
     *        [FFFFFFh])
     * @throw IllegalArgumentException - if the decrement value is out of range
     * @throw IllegalArgumentException - if the command is inconsistent
     */
    void prepareDecreaseCounter(const uint8_t sfi, const uint8_t counterNumber, const int decValue);

    /**
     * Builds a VerifyPin command without PIN presentation in order to get the attempt counter.<br>
     * The PIN status will made available in CalypsoPo after the execution of process command.<br>
     * Adds it to the list of commands to be sent with the next process command.
     *
     * See {@link CalypsoPo#isPinBlocked} and {@link CalypsoPo#getPinAttemptRemaining} methods.
     *
     * @throw CalypsoPoTransactionIllegalStateException if the PIN feature is not available for
     *         this PO.
     */
    void prepareCheckPinStatus();

    /**
     * Prepares an SV operation or simply retrieves the current SV status
     *
     * @param svOperation informs about the nature of the intended operation: debit or reload
     * @param svAction the type of action: DO a debit or a positive reload, UNDO an undebit or a
     *        negative reload
     * @throws CalypsoPoTransactionIllegalStateException if the SV feature is not available for this
     *         PO.
     */
    void prepareSvGet(const SvSettings::Operation& svOperation, const SvSettings::Action& svAction);

    /**
     * Prepares an SV reload (increasing the current SV balance)
     * <p>
     * Note: the key used is the reload key
     *
     * @param amount the value to be reloaded, positive or negative integer in the range
     *        -8388608..8388607
     * @param date 2-byte free value
     * @param time 2-byte free value
     * @param free 2-byte free value
     * @throw CalypsoPoTransactionIllegalStateException if the SV feature is not available for this
     *         PO.
     */
    void prepareSvReload(const int amount,
                         const std::vector<uint8_t>& date,
                         const std::vector<uint8_t>& time,
                         const std::vector<uint8_t>& free);

    /**
     * Prepares an SV reload (increasing the current SV balance)
     * <p>
     * Note: the key used is the reload key
     *
     * @param amount the value to be reloaded, positive integer in the range 0..8388607 for a DO
     *        action, in the range 0..8388608 for an UNDO action.
     * @throw CalypsoPoTransactionIllegalStateException if the SV feature is not available for this
     *         PO.
     */
    void prepareSvReload(const int amount);

    /**
     * Prepares an SV debit or Undebit (partially or totally cancels the last SV debit command).
     * <p>
     * It consists in decreasing the current balance of the SV by a certain amount or canceling a
     * previous debit. <br>
     * Note: the key used is the debit key
     *
     * @param amount the amount to be subtracted or added, positive integer in the range 0..32767
     *        when subtracted and 0..32768 when added.
     * @param date 2-byte free value
     * @param time 2-byte free value
     */
    void prepareSvDebit(const int amount,
                        const std::vector<uint8_t>& date,
                        const std::vector<uint8_t>& time);

    /**
     * Prepares an SV debit or Undebit (partially or totally cancels the last SV debit command).
     * <p>
     * It consists in decreasing the current balance of the SV by a certain amount or canceling a
     * previous debit. <br>
     * The information fields such as date and time are set to 0. The extraInfo field propagated in
     * Logs are automatically generated with the type of transaction and amount. <br>
     * Operations that would result in a negative balance are forbidden (SV Exception raised). <br>
     * Note: the key used is the debit key
     *
     * @param amount the amount to be subtracted or added, positive integer in the range 0..32767
     *        when subtracted and 0..32768 when added.
     */
    void prepareSvDebit(const int amount);

    /**
     * Prepare the reading of all SV log records
     * <p>
     * The SV transaction logs are contained in two files with fixed identifiers.<br>
     * The file whose SFI is 0x14 contains 1 record containing the unique reload log.<br>
     * The file whose SFI is 0x15 contains 3 records containing the last three debit logs.<br>
     * At the end of this reading operation, the data will be accessible in CalypsoPo in raw format
     * via the standard commands for accessing read files or in the form of dedicated objects (see
     * {@link CalypsoPo#getSvLoadLogRecord()} and {@link CalypsoPo#getSvDebitLogAllRecords()})
     */
    void prepareSvReadAllLogs();

    /**
     * Prepare the invalidation of the PO<br>
     * This command is usually executed within a secure session with the SESSION_LVL_DEBIT key
     * (depends on the access rights given to this command in the file structure of the PO).
     *
     * @throws CalypsoPoTransactionIllegalStateException if the PO is already invalidated
     */
    void prepareInvalidate();

    /**
     * Prepare the rehabilitation of the PO<br>
     * This command is usually executed within a secure session with the SESSION_LVL_PERSO key
     * (depends on the access rights given to this command in the file structure of the PO).
     *
     * @throws CalypsoPoTransactionIllegalStateException if the PO is not invalidated
     */
    void prepareRehabilitate();



    /**
     * Prepare to close the PO channel.<br>
     * If this command is called before a "process" command (except for processOpening) then the
     * last transmission to the PO will be associated with the indication CLOSE_AFTER in order to
     * close the PO channel.<br>
     * Important: this command must imperatively be called at the end of any transaction, whether it
     * ended normally or not.<br>
     * In case the transaction was interrupted (exception), an additional call to processPoCommands
     * must be made to effectively close the channel.
     *
     * C++ vs. Java: private in Java
     */
    void prepareReleasePoChannel();

private:

    /**
     *
     */
    friend std::ostream& operator<<(std::ostream& os, const SessionState& ss);

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
    SessionSetting::AccessLevel mCurrentAccessLevel;

    /**
     * Modifications counter management
     */
    int mModificationsCounter;

    /**
     * The object for managing PO commands
     */
    std::shared_ptr<PoCommandManager> mPoCommandManager;

    /**
     * The current Store Value action
     */
    SvSettings::Action mSvAction;

    /**
     * The keyple::core::seproxy::message::ChannelControl action
     */
    ChannelControl mChannelControl;

    /**
     *
     */
    static const std::shared_ptr<ApduResponse> RESPONSE_OK;

    /**
     *
     */
    static const std::shared_ptr<ApduResponse> RESPONSE_OK_POSTPONED;

    /**
     * Process all prepared PO commands (outside a Secure Session).
     * <ul>
     * <li>On the PO reader, generates a SeRequest with channelControl set to the provided value and
     * ApduRequests containing the PO commands.</li>
     * <li>The result of the commands is placed in CalypsoPo.</li>
     * </ul>
     *
     * @param channelControl indicates if the SE channel of the PO reader must be closed after the
     *        last command
     * @throws CalypsoPoTransactionException if a functional error occurs (including PO and SAM IO
     *         errors)
     * @throws CalypsoPoCommandException if a response from the PO was unexpected
     */
    void processPoCommandsOutOfSession(const ChannelControl channelControl);

    /**
     * Process all prepared PO commands in a Secure Session.
     * <ul>
     * <li>On the PO reader, generates a SeRequest with channelControl set to KEEP_OPEN, and
     * ApduRequests containing the PO commands.</li>
     * <li>In case the secure session is active, the "cache" of SAM commands is completed with the
     * corresponding Digest Update commands.</li>
     * <li>The result of the commands is placed in CalypsoPo.</li>
     * </ul>
     *
     * @throws CalypsoPoTransactionException if a functional error occurs (including PO and SAM IO
     *         errors)
     * @throws CalypsoPoCommandException if a response from the PO was unexpected
     * @throws CalypsoSamCommandException if a response from the SAM was unexpected
     */
    void processPoCommandsInSession();


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
        const SessionSetting::AccessLevel& accessLevel,
        std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>>&
            poCommands);

    /**
     * c++ vs. Java: Function overload to avoid declaring empty vectors here and
     *               there in the code. Better encapsulated that hask into a
     *               function.
     */
    void processAtomicOpening(const SessionSetting::AccessLevel& accessLevel);

    /**
     * Create an ApduRequest List from a AbstractPoCommandBuilder List.
     *
     * @param poCommands a list of PO commands
     * @return the ApduRequest list
     */
    std::vector<std::shared_ptr<ApduRequest>> getApduRequests(
        const std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>>&
            poCommands) const;

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
     * @throw CalypsoPoTransactionException if a functional error occurs
     *        (including PO and SAM IO errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    void processAtomicPoCommands(
        const std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>>&
            poCommands,
        const ChannelControl channelControl);

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
     * <li>Then, on the PO reader, a SeRequest is transmitted with a keyple::core::seproxy::message::ChannelControl set to
     * CLOSE_AFTER or KEEP_OPEN depending on whether or not prepareReleasePoChannel was called, and
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
     * @param ratificationMode the ratification mode tells if the session is closed ratified or not
     * @param channelControl indicates if the SE channel of the PO reader must be closed after the
     *        last command
     * @throw CalypsoPoTransactionException if a functional error occurs (including PO and SAM IO
     *         errors)
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     * @throw CalypsoSamCommandException if a response from the SAM was unexpected
     */
    void processAtomicClosing(
        const std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>>&
            poModificationCommands,
        const std::vector<std::shared_ptr<ApduResponse>>& poAnticipatedResponses,
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
            AbstractPoResponseParser>>>& poCommands,
        const SessionSetting::RatificationMode ratificationMode,
        const ChannelControl channelControl);

    /**
     * C++ vs. Java: Added constructor with no poCommands (null in Java) to
     *               avoid declaring dummy empty vectors here and there in the
     *               code. The hack is encapsulated in the function.
     *
     * Advanced variant of processAtomicClosing in which the list of expected
     * responses is determined from previous reading operations.
     *
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
    void processAtomicClosing(const SessionSetting::RatificationMode ratificationMode,
                              const ChannelControl channelControl);

    /**
     * Gets the value of the designated counter
     *
     * @param sfi the SFI of the EF containing the counter
     * @param counter the number of the counter
     * @return the value of the counter
     */
    int getCounterValue(const uint8_t sfi, const int counter) const;

    /**
     * Create an anticipated response to an Increase/Decrease command
     *
     * @param newCounterValue the anticipated counter value
     * @return an {@link ApduResponse} containing the expected bytes
     */
    std::unique_ptr<ApduResponse> createIncreaseDecreaseResponse(const int newCounterValue) const;

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
        const std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>>&
            poCommands) const;

    /**
     *
     */
    std::shared_ptr<SeResponse> safePoTransmit(const std::shared_ptr<SeRequest> poSeRequest,
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
     * @throw CalypsoDesynchronizedExchangesException if the test failed
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
        std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
            builder,
        std::atomic<bool>& overflow,
        std::atomic<int>& neededSessionBufferSpace);

    /**
     * Checks whether the requirement for the modifications buffer of the command provided in
     * argument is compatible with the current usage level of the buffer.
     * <p>
     * If it is compatible, the requirement is subtracted from the current level and the method
     * returns false. If this is not the case, the method returns true and the current level is left
     * unchanged.
     *
     * @param sessionBufferSizeConsumed session buffer requirement
     * @return true or false
     */
    bool isSessionBufferOverflowed(const int sessionBufferSizeConsumed);

    /**
     * Initialized the modifications buffer counter to its maximum value for the current PO
     */
    void resetModificationsBufferCounter();

    /**
     * Prepares an SV debit.
     * <p>
     * It consists in decreasing the current balance of the SV by a certain amount. <br>
     * Note: the key used is the debit key
     *
     * @param amount the amount to be subtracted, positive integer in the range 0..32767
     * @param date 2-byte free value
     * @param time 2-byte free value
     */
    void prepareSvDebitPriv(const int amount,
                            const std::vector<uint8_t>& date,
                            const std::vector<uint8_t>& time);

    /**
     * Prepares an SV Undebit (partially or totally cancels the last SV debit command).
     * <p>
     * It consists in canceling a previous debit. <br>
     * Note: the key used is the debit key
     *
     * @param amount the amount to be subtracted, positive integer in the range 0..32767
     * @param date 2-byte free value
     * @param time 2-byte free value
     */
    void prepareSvUndebitPriv(const int amount,
                              const std::vector<uint8_t>& date,
                              const std::vector<uint8_t>& time);
};

using Operation = PoTransaction::SvSettings::Operation;

KEYPLECALYPSO_API std::ostream& operator<<(std::ostream& os, const Operation& o);

}
}
}

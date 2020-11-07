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

#include <vector>
#include <memory>

/* Calypso */
#include "AbstractMatchingSe.h"
#include "DirectoryHeader.h"
#include "ElementaryFile.h"
#include "PoRevision.h"
#include "PoClass.h"
#include "SvDebitLogRecord.h"
#include "SvLoadLogRecord.h"

/* Core */
#include "TransmissionMode.h"

/* Common */
#include "Byte.h"
#include "KeypleCalypsoExport.h"
#include "LoggerFactory.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy::message;
using namespace keyple::common;
using namespace keyple::core::seproxy::protocol;

/**
 * The CalypsoPo class gathers all the information about the current PO retrieved from the response
 * to the select application command.
 * <p>
 * An instance of CalypsoPo can be obtained by casting the AbstractMatchingSe object from the
 * selection process (e.g. (CalypsoPo) matchingSelection.getMatchingSe())
 * <p>
 * The various information contained in CalypsoPo is accessible by getters and includes:
 * <ul>
 * <li>The application identification fields (revision/version, class, DF name, serial number, ATR,
 * issuer)
 * <li>The indication of the presence of optional features (Stored Value, PIN, Rev3.2 mode,
 * ratification management)
 * <li>The management information of the modification buffer
 * <li>The invalidation status
 * </ul>
 */
class KEYPLECALYPSO_API CalypsoPo final : public AbstractMatchingSe {
public:
    /**
     * Constructor.
     *
     * @param selectionResponse the response to the selection application
     *        command
     * @param transmissionMode the current {@link TransmissionMode} (contacts or
     *        contactless)
     */
    CalypsoPo(std::shared_ptr<SeResponse> selectionResponse,
              const TransmissionMode& transmissionMode);

    /**
     * The PO revision indicates the generation of the product presented.
     * <p>
     * It will also have an impact on the internal construction of some commands
     * to take into account the specificities of the different POs.
     *
     * @return an enum giving the identified PO revision
     */
    PoRevision getRevision() const;

    /**
     * The DF name is the name of the application DF as defined in ISO/IEC
     * 7816-4.
     * <p>
     * It also corresponds to the complete representation of the target covered
     * by the AID value provided in the selection command.
     * <p>
     * The AID selects the application by specifying all or part of the targeted
     * DF Name (5 bytes minimum).
     *
     * @return a byte array containing the DF Name bytes (5 to 16 bytes)
     */
    const std::vector<uint8_t>& getDfNameBytes() const;

    /**
     * @return the DF name as an HEX string (see getDfNameBytes)
     */
    const std::string getDfName() const;

    /**
     * The serial number for the application, is unique ID for the PO.
     *
     * @return a byte array containing the Application Serial Number (8 bytes)
     */
    const std::vector<uint8_t> getApplicationSerialNumberBytes() const;

    /**
     * The Answer To Reset is sent by the PO is ISO7816-3 mode and in
     * contactless mode for PC/SC readers.
     * <p>
     * When the ATR is obtained in contactless mode, it is in fact reconstructed
     * by the reader from information obtained from the lower communication
     * layers. Therefore, it may differ from one reader to another depending on
     * the interpretation that has been made by the manufacturer of the PC/SC
     * standard.
     * <p>
     * This field is not interpreted in the Calypso module.
     *
     * @return an HEX chain representing the ATR
     * @throw IllegalStateException if the ATR is not available (see {@code
     *        hasAtr()} method)
     * @since 0.9
     */
    const std::string getAtr() const;

    /**
     * The platform identification byte is the reference of the chip
     *
     * @return the platform identification byte
     */
    uint8_t getPlatform() const;

    /**
     * The Application Type byte determines the Calypso Revision and various
     * options
     *
     * @return the Application Type byte
     */
    uint8_t getApplicationType() const;

    /**
     * Indicates whether the Confidential Session Mode is supported or not
     * (since rev 3.2).
     * <p>
     * This boolean is interpreted from the Application Type byte
     *
     * @return true if the Confidential Session Mode is supported
     */
    bool isConfidentialSessionModeSupported() const;

    /**
     * Indicates if the ratification is done on deselect (ratification command
     * not necessary)
     * <p>
     * This boolean is interpreted from the Application Type byte
     *
     * @return true if the ratification command is required
     */
    bool isDeselectRatificationSupported() const;

    /**
     * Indicates whether the PO has the Calypso Stored Value feature.
     * <p>
     * This boolean is interpreted from the Application Type byte
     *
     * @return true if the PO has the Stored Value feature
     */
    bool isSvFeatureAvailable() const;

    /**
     * Indicates whether the PO has the Calypso PIN feature.
     * <p>
     * This boolean is interpreted from the Application Type byte
     *
     * @return true if the PO has the PIN feature
     */
    bool isPinFeatureAvailable() const;

    /**
     * Indicates whether the Public Authentication is supported or not (since
     * rev 3.3).
     * <p>
     * This boolean is interpreted from the Application Type byte
     *
     * @return true if the Public Authentication is supported
     */
    bool isPublicAuthenticationSupported() const;

    /**
     * The Application Subtype indicates to the terminal a reference to the file
     * structure of the Calypso DF.
     *
     * @return the Application Subtype byte
     */
    uint8_t getApplicationSubtype() const;

    /**
     * The Software Issuer byte indicates the entity responsible for the
     * software of the selected application.
     *
     * @return the Software Issuer byte
     */
    uint8_t getSoftwareIssuer() const;

    /**
     * The Software Version field may be set to any fixed value by the Software
     * Issuer of the Calypso application.
     *
     * @return the Software Version byte
     */
    uint8_t getSoftwareVersion() const;

    /**
     * The Software Revision field may be set to any fixed value by the Software
     * Issuer of the Calypso application.
     *
     * @return the Software Revision byte
     */
    uint8_t getSoftwareRevision() const;

    /**
     * Depending on the type of PO, the session modification byte indicates the
     * maximum number of bytes that can be modified or the number of possible
     * write commands in a session.
     *
     * @return the Session Modifications byte
     */
    uint8_t getSessionModification() const;

    /**
     * Indicated whether the PO has been invalidated or not.
     * <p>
     * An invalidated PO has 6283 as status word in response to the Select
     * Application command.
     *
     * @return true if the PO has been invalidated.
     */
    bool isDfInvalidated() const;

    /**
     * Indicated whether the last session with this PO has been ratified or not.
     * <p>
     *
     * @return true if the PO has been ratified.
     * @throw IllegalStateException if these methods is call when no session has
     *        been opened
     */
    bool isDfRatified() const;

    /**
     * (package-private)<br>
     * Sets the Stored Value data from the SV Get command
     *
     * @param svBalance the current SV balance
     * @param svLastTNum the last SV transaction number
     * @param svLoadLogRecord the SV load log record (may be null if not available)
     * @param svDebitLogRecord the SV debit log record (may be null if not available)
     */
    void setSvData(const int svBalance,
                   const int svLastTNum,
                   const std::shared_ptr<SvLoadLogRecord> svLoadLogRecord,
                   const std::shared_ptr<SvDebitLogRecord> svDebitLogRecord);

    /**
     * Gets the current SV balance
     *
     * @return the current SV balance value
     * @throw IllegalStateException if no SV Get command has been executed
     * @since 0.9
     */
    int getSvBalance() const;

    /**
     * Gets the last SV transaction number
     *
     * @return the last SV transaction number value
     * @throw IllegalStateException if no SV Get command has been executed
     * @since 0.9
     */
    int getSvLastTNum() const;

    /**
     * Gets the last SV load log record
     *
     * @return a last SV load log record object or null if not available
     * @throw NoSuchElementException if requested log is not found.
     * @since 0.9
     */
    const std::shared_ptr<SvLoadLogRecord> getSvLoadLogRecord();

    /**
     * Gets the last SV debit log record
     *
     * @return a last SV debit log record object or null if not available
     * @throw NoSuchElementException if requested log is not found.
     * @since 0.9
     */
    const std::shared_ptr<SvDebitLogRecord> getSvDebitLogLastRecord();

    /**
     * Gets all available SV debit log records
     *
     * @return a list of SV debit log record objects or null if not available
     * @throw NoSuchElementException if requested log is not found.
     * @since 0.9
     */
    const std::vector<std::shared_ptr<SvDebitLogRecord>> getSvDebitLogAllRecords() const;

    /**
     * (package-private)<br>
     * Set the ratification status
     *
     * @param dfRatified true if the session was ratified
     * @since 0.9
     */
    void setDfRatified(const bool dfRatified);

    /**
     * The serial number for the application, is unique ID for the PO.
     *
     * @return a String representing the Application Serial Number (8 hex bytes)
     */
    const std::string getApplicationSerialNumber() const;

    /**
     * @return the startup info field from the FCI as an HEX string
     * @since 0.9
     */
    const std::string getStartupInfo() const;

    /**
     * Gets the DF metadata.
     *
     * @return null if is not set.
     * @since 0.9
     */
    const std::shared_ptr<DirectoryHeader> getDirectoryHeader() const;

    /**
     * (package-private)<br>
     * Sets the DF metadata.
     *
     * @param directoryHeader the DF metadata (should be not null)
     * @return the current instance.
     */
    CalypsoPo* setDirectoryHeader(
        const std::shared_ptr<DirectoryHeader> directoryHeader);

    /**
     * Gets a reference to the {@link ElementaryFile} that has the provided SFI
     * value.<br>
     * Note that if a secure session is actually running, then the object
     * contains all session modifications, which can be canceled if the secure
     * session fails.
     *
     * @param sfi the SFI to search
     * @return a not null reference.
     * @throw NoSuchElementException if requested EF is not found.
     * @since 0.9
     */
    const std::shared_ptr<ElementaryFile> getFileBySfi(const uint8_t sfi) const;

    /**
     * Gets a reference to the {@link ElementaryFile} that has the provided LID
     * value.<br>
     * Note that if a secure session is actually running, then the object
     * contains all session modifications, which can be canceled if the secure
     * session fails.
     *
     * @param lid the LID to search
     * @return a not null reference.
     * @throw NoSuchElementException if requested EF is not found.
     * @since 0.9
     */
    const std::shared_ptr<ElementaryFile> getFileByLid(const uint16_t lid);

    /**
     * Gets a reference to a map of all known Elementary Files by their
     * associated SFI.<br>
     * Note that if a secure session is actually running, then the map contains
     * all session modifications, which can be canceled if the secure session
     * fails.
     *
     * @return a not null reference (may be empty if no one EF is set).
     * @since 0.9
     */
    const std::map<uint8_t, std::shared_ptr<ElementaryFile>>& getAllFiles()
        const;

    /**
     * (package-private)<br>
     * Sets the provided {@link FileHeader} to the EF having the provided SFI.
     * <br>
     * If EF does not exist, then it is created.
     *
     * @param sfi the SFI
     * @param header the file header (should be not null)
     */
    void setFileHeader(const uint8_t sfi,
                       const std::shared_ptr<FileHeader> header);

    /**
     * (package-private)<br>
     * Set or replace the entire content of the specified record #numRecord of
     * the provided SFI by the provided content.<br>
     * If EF does not exist, then it is created.
     *
     * @param sfi the SFI
     * @param numRecord the record number (should be {@code >=} 1)
     * @param content the content (should be not empty)
     */
    void setContent(const uint8_t sfi,
                    const int numRecord,
                    const std::vector<uint8_t>& content);

    /**
     * (package-private)<br>
     * Sets a counter value in record #1 of the provided SFI.<br>
     * If EF does not exist, then it is created.
     *
     * @param sfi the SFI
     * @param numCounter the counter number (should be {@code >=} 1)
     * @param content the counter value (should be not null and 3 bytes length)
     */
    void setCounter(const uint8_t sfi,
                    const int numCounter,
                    const std::vector<uint8_t>& content);

    /**
     * (package-private)<br>
     * Set or replace the content at the specified offset of record #numRecord
     * of the provided SFI by a copy of the provided content.<br>
     * If EF does not exist, then it is created.<br>
     * If actual record content is not set or has a size {@code <} offset, then
     * missing data will be padded with 0.
     *
     * @param sfi the SFI
     * @param numRecord the record number (should be {@code >=} 1)
     * @param content the content (should be not empty)
     * @param offset the offset (should be {@code >=} 0)
     */
    void setContent(const uint8_t sfi,
                    const int numRecord,
                    const std::vector<uint8_t>& content,
                    const int offset);

    /**
     * (package-private)<br>
     * Fill the content of the specified #numRecord of the provided SFI using a
     * binary OR operation with the provided content.<br>
     * If EF does not exist, then it is created.<br>
     * If actual record content is not set or has a size {@code <} content size,
     * then missing data will be completed by the provided content.
     *
     * @param sfi the SFI
     * @param numRecord the record number (should be {@code >=} 1)
     * @param content the content (should be not empty)
     */
    void fillContent(const uint8_t sfi,
                     const int numRecord,
                     std::vector<uint8_t>& content);

    /**
     * (package-private)<br>
     * Add cyclic content at record #1 by rolling previously all actual records
     * contents (record #1 -> record #2, record #2 -> record #3,...) of the
     * provided SFI.<br>
     * This is useful for cyclic files. Note that records are infinitely
     * shifted.<br>
     * <br>
     * If EF does not exist, then it is created.
     *
     * @param sfi the SFI
     * @param content the content (should be not empty)
     */
    void addCyclicContent(const uint8_t sfi,
                          const std::vector<uint8_t>& content);

    /**
     * (package-private)<br>
     * Make a backup of the Elementary Files.<br>
     * This method should be used before starting a PO secure session.
     */
    void backupFiles();

    /**
     * (package-private)<br>
     * Restore the last backup of Elementary Files.<br>
     * This method should be used when SW of the PO close secure session command
     * is unsuccessful or if secure session is aborted.
     */
    void restoreFiles();

    /**
     * Indicates the maximum number of changes allowed in session.
     * <p>
     * This number can be a number of operations or a number of commands (see
     * isModificationsCounterInBytes)
     *
     * @return the maximum number of modifications allowed
     */
    int getModificationsCounter() const;

    /**
     * The PO class is the ISO7816 class to be used with the current PO.
     * <p>
     * It determined from the PO revision
     * <p>
     * Two classes are possible: LEGACY and ISO.
     *
     * @return the PO class determined from the PO revision
     */
    const PoClass& getPoClass() const;

    /**
     * Specifies whether the change counter allowed in session is established in
     * number of operations or number of bytes modified.
     * <p>
     * This varies depending on the revision of the PO.
     *
     * @return true if the counter is number of bytes
     */
    bool isModificationsCounterInBytes() const;

    /**
     * @return the maximum length of data that an APDU in this PO can carry
     * @since 0.9
     */
    int getPayloadCapacity() const;

    /**
     * Indicates if the PIN is blocked. The maximum number of incorrect PIN submissions has been
     * reached.
     *
     * @return true if the PIN status is blocked
     * @throw IllegalStateException if the PIN has not been checked
     * @since 0.9
     */
    bool isPinBlocked() const;

    /**
     * Gives the number of erroneous PIN presentations remaining before blocking.
     *
     * @return the number of remaining attempts
     * @throw IllegalStateException if the PIN has not been checked
     * @since 0.9
     */
    int getPinAttemptRemaining() const;

    /**
     * (package-private)<br>
     * Sets the PIN attempts counter.<br>
     * The PIN attempt counter is interpreted to give the results of the methods
     * {@link #isPinBlocked} and {@link #getPinAttemptRemaining}.
     *
     * @param pinAttemptCounter the number of remaining attempts to present the PIN code
     */
    void setPinAttemptRemaining(const int pinAttemptCounter);

protected:
    /**
     * The serial number to be used as diversifier for key derivation.<br>
     * This is the complete number returned by the PO in its response to the
     * Select command.
     *
     * @return a byte array containing the Calypso Serial Number (8 bytes)
     */
    const std::vector<uint8_t>& getCalypsoSerialNumber() const;

    /**
     *
     */
    bool isSerialNumberExpiring() const;

    /**
     *
     */
    const std::vector<uint8_t>& getSerialNumberExpirationBytes() const;

private:
    /**
     *
     */
    bool mIsConfidentialSessionModeSupported;

    /**
     *
     */
    bool mIsDeselectRatificationSupported;

    /**
     *
     */
    bool mIsSvFeatureAvailable;

    /**
     *
     */
    bool mIsPinFeatureAvailable;

    /**
     *
     */
    bool mIsPublicAuthenticationSupported;

    /**
     *
     */
    bool mIsDfInvalidated;

    /**
     *
     */
    std::shared_ptr<PoClass> mPoClass;

    /**
     *
     */
    std::vector<uint8_t> mCalypsoSerialNumber;

    /**
     *
     */
    std::vector<uint8_t> mStartupInfo;

    /**
     *
     */
    PoRevision mRevision;

    /**
     *
     */
    std::vector<uint8_t> mDfName;

    /**
     *
     */
    static const int PO_REV1_ATR_LENGTH;

    /**
     *
     */
    static const int
        REV1_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION;

    /**
     *
     */
    static const int
        REV2_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION;

    /**
     *
     */
    static const int SI_BUFFER_SIZE_INDICATOR;
    static const int SI_PLATFORM;
    static const int SI_APPLICATION_TYPE;
    static const int SI_APPLICATION_SUBTYPE;
    static const int SI_SOFTWARE_ISSUER;
    static const int SI_SOFTWARE_VERSION;
    static const int SI_SOFTWARE_REVISION;

    /**
     * Application type bitmasks features
     */
    static const uint8_t APP_TYPE_WITH_CALYPSO_PIN;
    static const uint8_t APP_TYPE_WITH_CALYPSO_SV;
    static const uint8_t APP_TYPE_RATIFICATION_COMMAND_REQUIRED;
    static const uint8_t APP_TYPE_CALYPSO_REV_32_MODE;
    static const uint8_t APP_TYPE_WITH_PUBLIC_AUTHENTICATION;

    /**
     * Buffer indicator to buffer size lookup table
     */
    static const std::vector<int> BUFFER_SIZE_INDICATOR_TO_BUFFER_SIZE;

    /**
     *
     */
    int mModificationsCounterMax = 0;

    /**
     *
     */
    bool mModificationCounterIsInBytes = true;

    /**
     *
     */
    std::shared_ptr<DirectoryHeader> mDirectoryHeader;

    /**
     *
     */
    std::map<uint8_t, std::shared_ptr<ElementaryFile>> mEfBySfi;

    /**
     *
     */
    std::map<uint8_t, std::shared_ptr<ElementaryFile>> mEfBySfiBackup;

    /**
     *
     */
    std::map<uint16_t, uint8_t> mSfiByLid;

    /**
     *
     */
    std::map<uint16_t, uint8_t> mSfiByLidBackup;

    /**
     *
     */
    bool mIsDfRatified;

    /**
     *
     */
    std::shared_ptr<int> mPinAttemptCounter;

    /**
     *
     */
    std::shared_ptr<int> mSvBalance;

    /**
     *
     */
    int mSvLastTNum;

    /**
     *
     */
    std::shared_ptr<SvLoadLogRecord> mSvLoadLogRecord;

    /**
     *
     */
    std::shared_ptr<SvDebitLogRecord> mSvDebitLogRecord;

    /**
     * Resolve the PO revision from the application type byte
     *
     * <ul>
     * <li>if <code>%1-------</code>&nbsp;&nbsp;&rarr;&nbsp;&nbsp;CLAP&nbsp;
     * &nbsp;&rarr;&nbsp;&nbsp; REV3.1</li>
     * <li>if <code>%00101---</code>&nbsp;&nbsp;&rarr;&nbsp;&nbsp;REV3.2</li>
     * <li>if <code>%00100---</code>&nbsp;&nbsp;&rarr;&nbsp;&nbsp;REV3.1</li>
     * <li>otherwise&nbsp;&nbsp;&rarr;&nbsp;&nbsp;REV2.4</li>
     * </ul>
     *
     * @param applicationType the application type (field of startup info)
     * @return the {@link PoRevision}
     */
    PoRevision determineRevision(const uint8_t applicationType) const;

    /**
     * (private)<br>
     * Gets or creates the EF having the provided SFI.
     *
     * @param sfi the SFI
     * @return a not null reference.
     */
    std::shared_ptr<ElementaryFile> getOrCreateFile(const uint8_t sfi);

    /**
     * (private)<br>
     * Copy a map of ElementaryFile by SFI to another one by cloning each
     * element.
     *
     * @param src the source (should be not null)
     * @param dest the destination (should be not null)
     */
    static void copyMapFiles(
        const std::map<uint8_t, std::shared_ptr<ElementaryFile>>& src,
        std::map<uint8_t, std::shared_ptr<ElementaryFile>>& dest);

    /**
     * (private)<br>
     * Copy a map of SFI by LID to another one by cloning each element.
     *
     * @param src the source (should be not null)
     * @param dest the destination (should be not null)
     */
    static void copyMapSfi(const std::map<uint16_t, uint8_t>& src,
                           std::map<uint16_t, uint8_t>& dest);
};

}
}
}

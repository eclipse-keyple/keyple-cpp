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

#include <cstdint>
#include <vector>

/* Core */
#include "SeResource.h"

/* Calypso */
#include "AbstractSamCommandBuilder.h"
#include "CalypsoSam.h"
#include "PoTransaction.h"
#include "SvDebitCmdBuild.h"
#include "SvReloadCmdBuild.h"
#include "SvUndebitCmdBuild.h"

/* Forward declarations */
namespace keyple { namespace calypso { namespace transaction { class PoSecuritySettings; } } }
namespace keyple { namespace calypso { namespace transaction { class CalypsoPo; } } }

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::po::builder::storedvalue;
using namespace keyple::core::selection;

using AccessLevel = PoTransaction::SessionSetting::AccessLevel;

/**
 * The SamCommandProcessor class is dedicated to the management of commands sent to the SAM.
 * <p>
 * In particular, it manages the cryptographic computations related to the secure session (digest
 * computation).
 * <p>
 * It also will integrate the SAM commands used for Stored Value and PIN/key management. In session,
 * these commands need to be carefully synchronized with the digest calculation.
 */
class SamCommandProcessor final {
public:
    /**
     * Constructor
     *
     * @param poResource the PO resource containing the PO reader and the Calypso PO information
     * @param poSecuritySettings the security settings from the application layer
     */
    SamCommandProcessor(const std::shared_ptr<SeResource<CalypsoPo>> poResource,
                        const std::shared_ptr<PoSecuritySettings> poSecuritySettings);

    /**
     * Gets the terminal challenge
     * <p>
     * Performs key diversification if necessary by sending the SAM Select
     * Diversifier command prior to the Get Challenge command. The
     * diversification flag is set to avoid further unnecessary diversification
     * operations.
     * <p>
     * If the key diversification is already done, the Select Diversifier
     * command is omitted.
     * <p>
     * The length of the challenge varies from one PO revision to another. This
     * information can be found in the SeResource class field.
     *
     * @return the terminal challenge as an array of bytes
     * @throw CalypsoSamIOException if the communication with the SAM has failed.
     * @throw CalypsoDesynchronizedExchangesException if the APDU SAM exchanges
     *        are out of sync
     * @throw CalypsoSamCommandException if the SAM has responded with an error
     *        status
     */
    const std::vector<uint8_t> getSessionTerminalChallenge();

    /**
     * Initializes the digest computation process
     * <p>
     * Resets the digest data cache, then fills a first packet with the provided
     * data (from open secure session).
     * <p>
     * Keeps the session parameters, sets the KIF if not defined
     * <p>
     * Note: there is no communication with the SAM here.
     *
     * @param accessLevel the Acces Level
     * @param sessionEncryption true if the session is encrypted
     * @param verificationMode true if the verification mode is active
     * @param poKif the PO KIF
     * @param poKVC the PO KVC
     * @param digestData a first packet of data to digest.
     */
    void initializeDigester(const AccessLevel& accessLevel,
                            const bool sessionEncryption,
                            const bool verificationMode,
                            const uint8_t poKif,
                            const uint8_t poKVC,
                            const std::vector<uint8_t>& digestData);

    /**
     * Appends a list full PO exchange (request and response) to the digest data
     * cache.<br>
     * The startIndex argument makes it possible not to include the beginning of
     * the list when necessary.
     *
     * @param requests PO request list
     * @param responses PO response list
     * @param startIndex starting point in the list
     */
    void pushPoExchangeDataList(
        const std::vector<std::shared_ptr<ApduRequest>>& requests,
        const std::vector<std::shared_ptr<ApduResponse>>& responses,
        const int startIndex);

    /**
     * Gets the terminal signature from the SAM
     * <p>
     * All remaining data in the digest cache is sent to the SAM and the Digest
     * Close command is executed.
     *
     * @return the terminal signature
     * @throw CalypsoSamIOException if the communication with the SAM has
     *        failed.
     * @throw CalypsoDesynchronizedExchangesException if the APDU SAM exchanges
     *        are out of sync
     * @throw CalypsoSamCommandException if the SAM has responded with an error
     *        status
     */
    const std::vector<uint8_t> getTerminalSignature();

    /**
     * Authenticates the signature part from the PO
     * <p>
     * Executes the Digest Authenticate command with the PO part of the
     * signature.
     *
     * @param poSignatureLo the PO part of the signature
     * @throw CalypsoSamIOException if the communication with the SAM has
     *        failed.
     * @throw CalypsoDesynchronizedExchangesException if the APDU SAM exchanges
     *        are out of sync
     * @throw CalypsoSamCommandException if the SAM has responded with an error
     *        status
     */
    void authenticatePoSignature(const std::vector<uint8_t>& poSignatureLo);

    /**
     * Compute the PIN ciphered data for the encrypted PIN verification or PIN update commands
     *
     * @param poChallenge the challenge from the PO
     * @param currentPin the current PIN value
     * @param newPin the new PIN value (set to null if the operation is a PIN presentation)
     * @return the PIN ciphered data
     * @throw CalypsoSamIOException if the communication with the SAM has failed.
     * @throw CalypsoDesynchronizedExchangesException if the APDU SAM exchanges are out of sync
     * @throw CalypsoSamCommandException if the SAM has responded with an error status
     */
    const std::vector<uint8_t> getCipheredPinData(const std::vector<uint8_t>& poChallenge,
                                                  const std::vector<uint8_t>& currentPin,
                                                  const std::vector<uint8_t>& newPin);

    /**
     * Computes the cryptographic data required for the SvReload command.
     * <p>
     * Use the data from the SvGet command and the partial data from the SvReload command for this
     * purpose.
     * <p>
     * The returned data will be used to finalize the PO SvReload command.
     *
     * @param svReloadCmdBuild the SvDebit builder providing the SvReload partial data
     * @param svGetHeader the SV Get command header
     * @param svGetData the SV Get command response data
     * @return the complementary security data to finalize the SvReload PO command (sam ID + SV
     *         prepare load output)
     * @throw CalypsoSamIOException if the communication with the SAM has failed.
     */
    const std::vector<uint8_t> getSvReloadComplementaryData(
        std::shared_ptr<SvReloadCmdBuild> svReloadCmdBuild,
        const std::vector<uint8_t>& svGetHeader,
        const std::vector<uint8_t>& svGetData);

    /**
     * Computes the cryptographic data required for the SvDebit command.
     * <p>
     * Use the data from the SvGet command and the partial data from the SvDebit command for this
     * purpose.
     * <p>
     * The returned data will be used to finalize the PO SvDebit command.
     *
     * @param svDebitCmdBuild the SV Debit command builder
     * @param svGetHeader the SV Get command header
     * @param svGetData the SV Get command response data
     * @return the complementary security data to finalize the SvDebit PO command (sam ID + SV
     *         prepare load output)
     * @throw CalypsoSamIOException if the communication with the SAM has failed.
     */
    const std::vector<uint8_t> getSvDebitComplementaryData(
        std::shared_ptr<SvDebitCmdBuild> svDebitCmdBuild,
        const std::vector<uint8_t>& svGetHeader,
        const std::vector<uint8_t>& svGetData);

    /**
     * Computes the cryptographic data required for the SvUndebit command.
     * <p>
     * Use the data from the SvGet command and the partial data from the SvUndebit command for this
     * purpose.
     * <p>
     * The returned data will be used to finalize the PO SvUndebit command.
     *
     * @param svUndebitCmdBuild the SV Undebit command builder
     * @param svGetHeader the SV Get command header
     * @param svGetData the SV Get command response data
     * @return the complementary security data to finalize the SvUndebit PO command (sam ID + SV
     *         prepare load output)
     * @throw CalypsoSamIOException if the communication with the SAM has failed.
     */
    const std::vector<uint8_t> getSvUndebitComplementaryData(
        std::shared_ptr<SvUndebitCmdBuild> svUndebitCmdBuild,
        const std::vector<uint8_t>& svGetHeader,
        const std::vector<uint8_t>& svGetData);

    /**
     * Checks the status of the last SV operation
     * <p>
     * The PO signature is compared by the SAM with the one it has computed on its side.
     *
     * @param svOperationResponseData the data of the SV operation performed
     * @throw CalypsoSamIOException if the communication with the SAM has failed.
     * @throw CalypsoSamCommandException if the SAM has responded with an error status
     */
    void checkSvStatus(const std::vector<uint8_t>& svOperationResponseData);

private:
    /**
     *
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(SamCommandProcessor));

    /**
     *
     */
    static const uint8_t KIF_UNDEFINED;

    /**
     *
     */
    static const uint8_t CHALLENGE_LENGTH_REV_INF_32;
    static const uint8_t CHALLENGE_LENGTH_REV32;
    static const uint8_t SIGNATURE_LENGTH_REV_INF_32;
    static const uint8_t SIGNATURE_LENGTH_REV32;

    /**
     * The SAM resource
     */
    const std::shared_ptr<SeResource<CalypsoSam>> mSamResource;

    /**
     * The Proxy reader to communicate with the SAM
     */
    const std::shared_ptr<ProxyReader> mSamReader;

    /**
     * The PO resource
     */
    const std::shared_ptr<SeResource<CalypsoPo>> mPoResource;

    /**
     * The security settings
     */
    const std::shared_ptr<PoSecuritySettings> mPoSecuritySettings;

    /*
     * The digest data cache stores all PO data to be send to SAM during a
     * Secure Session. The 1st buffer is the data buffer to be provided with
     * Digest Init. The following buffers are PO command/response pairs
     */
    std::vector<std::vector<uint8_t>> mPoDigestDataCache;

    /**
     *
     */
    bool mSessionEncryption;

    /**
     *
     */
    bool mVerificationMode;

    /**
     *
     */
    uint8_t mWorkKeyRecordNumber;

    /**
     *
     */
    uint8_t mWorkKeyKif;

    /**
     *
     */
    uint8_t mWorkKeyKVC;

    /**
     *
     */
    bool mIsDiversificationDone;

    /**
     *
     */
    bool mIsDigestInitDone;

    /**
     *
     */
    bool mIsDigesterInitialized;

    /**
     * Determine the work KIF from the value returned by the PO and the session
     * access level.
     * <p>
     * If the value provided by the PO undetermined (FFh), the actual value of
     * the work KIF is found in the PoSecuritySettings according to the session
     * access level.
     * <p>
     * If the value provided by the PO is not undetermined, the work KIF is set
     * to this value.
     *
     * @param poKif the KIF value from the PO
     * @param accessLevel the session access level
     * @return the work KIF value byte
     */
    uint8_t determineWorkKif(const uint8_t poKif,
                             const AccessLevel& accessLevel) const;

     /**
     * Appends a full PO exchange (request and response) to the digest data
     * cache.
     *
     * @param request PO request
     * @param response PO response
     */
    void pushPoExchangeData(const std::shared_ptr<ApduRequest> request,
                            const std::shared_ptr<ApduResponse> response);


    /**
     * Gets a single SAM request for all prepared SAM commands.
     * <p>
     * Builds all pending SAM commands related to the digest calculation process
     * of a secure session
     * <ul>
     * <li>Starts with a Digest Init command if not already done,
     * <li>Adds as many Digest Update commands as there are packages in the
     * cache,
     * <li>Appends a Digest Close command if the addDigestClose flag is set to
     * true.
     * </ul>
     *
     * @param addDigestClose indicates whether to add the Digest Close command
     * @return a list of commands to send to the SAM
     */
    const std::vector<std::shared_ptr<AbstractSamCommandBuilder<AbstractSamResponseParser>>>
        getPendingSamCommands(const bool addDigestClose);

    /**
     * Create an ApduRequest List from a AbstractSamCommandBuilder List.
     *
     * @param samCommands a list of SAM commands
     * @return the ApduRequest list
     */
    const std::vector<std::shared_ptr<ApduRequest>> getApduRequests(
        const std::vector<std::shared_ptr<AbstractSamCommandBuilder<AbstractSamResponseParser>>>&
            samCommands);

    /**
     * Generic method to get the complementary data from SvPrepareLoad/Debit/Undebit commands
     * <p>
     * Executes the SV Prepare SAM command to prepare the data needed to complete the PO SV command.
     * <p>
     * This data comprises:
     * <ul>
     * <li>The SAM identifier (4 bytes)
     * <li>The SAM challenge (3 bytes)
     * <li>The SAM transaction number (3 bytes)
     * <li>The SAM part of the SV signature (5 or 10 bytes depending on PO mode)
     * </ul>
     *
     * @param svPrepareCmdBuild the prepare command builder (can be prepareSvReload/Debit/Undebit)
     * @return a byte array containing the complementary data
     * @throw CalypsoSamIOException if the communication with the SAM has failed.
     */
    const std::vector<uint8_t> getSvComplementaryData(
        std::shared_ptr<AbstractSamCommandBuilder<AbstractSamResponseParser>> svPrepareCmdBuild);
};

}
}
}

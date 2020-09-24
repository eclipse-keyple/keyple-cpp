/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

/* Core */
#include "ApduResponse.h"
#include "AbstractApduResponseParser.h"

/* Calypso */
#include "AbstractOpenSessionCmdBuild.h"
#include "AbstractPoResponseParser.h"
#include "PoRevision.h"

/* Common */
#include "Byte.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::core::seproxy::message;

/**
 * Open session response parser. See specs: Calypso / page 100 / 9.5.1 - Open
 * secure session
 *
 */
class KEYPLECALYPSO_API AbstractOpenSessionRespPars
: public AbstractPoResponseParser {
public:
    /**
     * The Class SecureSession. A secure session is returned by a open secure
     * session command
     */
    class KEYPLECALYPSO_API SecureSession {
    public:
        /**
         * Instantiates a new SecureSession for a Calypso application revision 3
         *
         * @param challengeTransactionCounter Challenge transaction counter
         * @param challengeRandomNumber Challenge random number
         * @param previousSessionRatified the previous session ratified
         * @param manageSecureSessionAuthorized the manage secure session
         *        authorized
         * @param kif the KIF from the response of the open secure session APDU
         *        command
         * @param kvc the KVC from the response of the open secure session APDU
         *        command
         * @param originalData the original data from the response of the open
         *        secure session APDU command
         * @param secureSessionData the secure session data from the response of
         *        open secure session APDU command
         */
        SecureSession(const std::vector<uint8_t>& challengeTransactionCounter,
                      const std::vector<uint8_t>& challengeRandomNumber,
                      const bool previousSessionRatified,
                      const bool manageSecureSessionAuthorized,
                      const uint8_t kif,
                      std::shared_ptr<Byte> kvc,
                      const std::vector<uint8_t>& originalData,
                      const std::vector<uint8_t>& secureSessionData);

        /**
         * Instantiates a new SecureSession for a Calypso application revision
         * 2.4
         *
         * @param challengeTransactionCounter Challenge transaction counter
         * @param challengeRandomNumber Challenge random number
         * @param previousSessionRatified the previous session ratified
         * @param manageSecureSessionAuthorized the manage secure session
         *        authorized
         * @param kvc the KVC from the response of the open secure session APDU
         *        command
         * @param originalData the original data from the response of the open
         *        secure session APDU command
         * @param secureSessionData the secure session data from the response of
         *        open secure session APDU command
         */
        SecureSession(const std::vector<uint8_t>& challengeTransactionCounter,
                      const std::vector<uint8_t>& challengeRandomNumber,
                      const bool previousSessionRatified,
                      const bool manageSecureSessionAuthorized,
                      const std::shared_ptr<Byte> kvc,
                      const std::vector<uint8_t>& originalData,
                      const std::vector<uint8_t>& secureSessionData);

        /**
         *
         */
        virtual ~SecureSession() = default;

        /**
         *
         */
        virtual const std::vector<uint8_t>& getChallengeTransactionCounter()
            const;

        /**
         *
         */
        virtual const std::vector<uint8_t>& getChallengeRandomNumber() const;

        /**
         * Checks if is previous session ratified.
         *
         * @return the boolean
         */
        virtual bool isPreviousSessionRatified() const;

        /**
         * Checks if is manage secure session authorized.
         *
         * @return the boolean
         */
        virtual bool isManageSecureSessionAuthorized() const;

        /**
         * Gets the kif.
         *
         * @return the kif
         */
        virtual uint8_t getKIF() const;

        /**
         * Gets the kvc.
         *
         * @return the kvc
         */
        virtual std::shared_ptr<Byte> getKVC() const;

        /**
         * Gets the original data.
         *
         * @return the original data
         */
        virtual const std::vector<uint8_t>& getOriginalData() const;

        /**
         * Gets the secure session data.
         *
         * @return the secure session data
         */
        virtual const std::vector<uint8_t>& getSecureSessionData() const;

    private:
        /**
         * Challenge transaction counter
         */
        const std::vector<uint8_t> mChallengeTransactionCounter;

        /**
         * Challenge random number
         */
        const std::vector<uint8_t> mChallengeRandomNumber;

        /**
         * The previous session ratified boolean
         */
        const bool mPreviousSessionRatified;

        /**
         * The manage secure session authorized boolean
         */
        const bool mManageSecureSessionAuthorized;

        /**
         * The kif
         */
        const uint8_t mKif;

        /**
         * The kvc (may be null if it doesn't exist in the considered PO [rev
         * 1.0])
         */
        const std::shared_ptr<Byte> mKvc;

        /**
         * The original data
         */
        const std::vector<uint8_t> mOriginalData;

        /**
         * The secure session data
         */
        const std::vector<uint8_t> mSecureSessionData;
    };

    /**
     * The secure session
     */
    std::shared_ptr<SecureSession> mSecureSession;

    /**
     * Instantiates a new AbstractOpenSessionRespPars.
     *
     * @param response the response from Open secure session APDU command
     * @param builder the reference to the builder that created this parser
     * @param revision the revision of the PO
     */
    AbstractOpenSessionRespPars(
        std::shared_ptr<ApduResponse> response,
        AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>* builder,
        const PoRevision revision);

    /**
     *
     */
    virtual ~AbstractOpenSessionRespPars() = default;

    /**
     *
     */
    std::shared_ptr<AbstractOpenSessionRespPars> create(
        std::shared_ptr<ApduResponse> response, const PoRevision revision);

    /*
     * C++: This method is called from the class constructor. It *cannot* be
     * pure virtual. Moved it to derived class constructors.
     */
    virtual std::shared_ptr<SecureSession> toSecureSession(
                  const std::vector<uint8_t>& apduResponseData);

    /**
     *
     */
    virtual const std::vector<uint8_t>& getPoChallenge() const;

    /**
     *
     */
    virtual int getTransactionCounterValue() const;

    /**
     *
     */
    virtual bool wasRatified() const;

    /**
     *
     */
    virtual bool isManageSecureSessionAuthorized() const;

    /**
     *
     */
    virtual uint8_t getSelectedKif() const;

    /**
     *
     */
    virtual std::shared_ptr<Byte> getSelectedKvc() const;

    /**
     *
     */
    virtual const std::vector<uint8_t>& getRecordDataRead() const;

protected:
    /**
     *
     */
    const std::map<int, std::shared_ptr<StatusProperties>>& getStatusTable()
        const override;

private:
    /**
     *
     */
    static const std::map<int, std::shared_ptr<StatusProperties>> STATUS_TABLE;
};

}
}
}
}
}
}

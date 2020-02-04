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

/* Core */
#include "ApduResponse.h"
#include "AbstractApduResponseParser_Import.h"

/* Calypso */
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
using namespace keyple::core::seproxy::message;

/**
 * Open session response parser. See specs: Calypso / page 100 / 9.5.1 - Open
 * secure session
 *
 */
class AbstractOpenSessionRespPars : public AbstractPoResponseParser {
public:
    /**
     * The Class SecureSession. A secure session is returned by a open secure
     * session command
     */
    class SecureSession : public std::enable_shared_from_this<SecureSession> {
    private:
        /**
         * Challenge transaction counter
         */
        const std::vector<uint8_t> challengeTransactionCounter;

        /**
         * Challenge random number
         */
        const std::vector<uint8_t> challengeRandomNumber;

        /**
         * The previous session ratified boolean
         */
        const bool previousSessionRatified;

        /**
         * The manage secure session authorized boolean
         */
        const bool manageSecureSessionAuthorized;

        /**
         * The kif
         */
        const uint8_t kif;

        /**
         * The kvc (may be null if it doesn't exist in the considered PO [rev
         * 1.0])
         */
        const std::shared_ptr<Byte> kvc;

        /**
         * The original data
         */
        const std::vector<uint8_t> originalData;

        /**
         * The secure session data
         */
        const std::vector<uint8_t> secureSessionData;

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
        SecureSession(const std::vector<uint8_t> &challengeTransactionCounter,
                      const std::vector<uint8_t> &challengeRandomNumber,
                      bool previousSessionRatified,
                      bool manageSecureSessionAuthorized, uint8_t kif,
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
                      bool previousSessionRatified,
                      bool manageSecureSessionAuthorized,
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
         * @param secureSessionData the secure session data from the response of
         *        open secure session APDU command
         */
        SecureSession(const std::vector<uint8_t>& challengeTransactionCounter,
                      const std::vector<uint8_t>& challengeRandomNumber,
                      bool previousSessionRatified,
                      bool manageSecureSessionAuthorized,
                      std::shared_ptr<Byte> kvc,
                      const std::vector<uint8_t>& secureSessionData);

        /**
         *
         */
        virtual ~SecureSession() {}

        /**
         *
         */
        virtual const std::vector<uint8_t>& getChallengeTransactionCounter();

        /**
         *
         */
        virtual const std::vector<uint8_t>& getChallengeRandomNumber();

        /**
         * Checks if is previous session ratified.
         *
         * @return the boolean
         */
        virtual bool isPreviousSessionRatified();

        /**
         * Checks if is manage secure session authorized.
         *
         * @return the boolean
         */
        virtual bool isManageSecureSessionAuthorized();

        /**
         * Gets the kif.
         *
         * @return the kif
         */
        virtual uint8_t getKIF();

        /**
         * Gets the kvc.
         *
         * @return the kvc
         */
        virtual std::shared_ptr<Byte> getKVC();

        /**
         * Gets the original data.
         *
         * @return the original data
         */
        virtual const std::vector<uint8_t>& getOriginalData();

        /**
         * Gets the secure session data.
         *
         * @return the secure session data
         */
        virtual const std::vector<uint8_t>& getSecureSessionData();
    };

private:
    /**
     *
     */
    static std::unordered_map<int, std::shared_ptr<
               AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

    /**
     *
     */
    class StaticConstructor
    : public std::enable_shared_from_this<StaticConstructor> {
    public:
        StaticConstructor();
    };

    /**
     *
     */
    static AbstractOpenSessionRespPars::StaticConstructor staticConstructor;

protected:
    /**
     *
     */
    std::unordered_map<int, std::shared_ptr<
        AbstractApduResponseParser::StatusProperties>> getStatusTable() const
        override;

private:
    /**
     *
     */
    //const PoRevision revision;

public:
    /**
     * The secure session
     */
    std::shared_ptr<SecureSession> secureSession;

    /**
     * Instantiates a new AbstractOpenSessionRespPars.
     *
     * @param response the response from Open secure session APDU command
     * @param revision the revision of the PO
     */
    AbstractOpenSessionRespPars(std::shared_ptr<ApduResponse> response,
                                PoRevision revision);

    /**
     *
     */
    virtual ~AbstractOpenSessionRespPars() {}

    /**
     *
     */
    static std::shared_ptr<AbstractOpenSessionRespPars> create(
               std::shared_ptr<ApduResponse> response, PoRevision revision);

    /*
     * C++: This method is called from the class constructor. It *cannot* be
     * pure virtual. Moved it to derived class constructors.
     */
    //virtual std::shared_ptr<SecureSession> toSecureSession(
    //              std::vector<char> &apduResponseData) = 0;


    /**
     *
     */
    virtual const std::vector<uint8_t>& getPoChallenge();

    /**
     *
     */
    virtual int getTransactionCounterValue();

    /**
     *
     */
    virtual bool wasRatified();

    /**
     *
     */
    virtual bool isManageSecureSessionAuthorized();

    /**
     *
     */
    virtual char getSelectedKif();

    /**
     *
     */
    virtual std::shared_ptr<Byte> getSelectedKvc();

    /**
     *
     */
    virtual const std::vector<uint8_t>& getRecordDataRead();

protected:
    /**
     *
     */
    std::shared_ptr<AbstractOpenSessionRespPars> shared_from_this()
    {
        return std::static_pointer_cast<AbstractOpenSessionRespPars>(
                   AbstractApduResponseParser::shared_from_this());
    }
};

}
}
}
}
}
}

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
#include <stdexcept>
#include <memory>

/* Core */
#include "AbstractApduResponseParser_Import.h"

/* Calypso */
#include "PoRevision.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {
                            namespace security {

                                using PoRevision                 = org::eclipse::keyple::calypso::command::po::PoRevision;
                                using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                                using ApduResponse               = org::eclipse::keyple::seproxy::message::ApduResponse;

                                /**
                                 * Open security response parser. See specs: Calypso / page 100 / 9.5.1 - Open secure security
                                 *
                                 */
                                class AbstractOpenSessionRespPars : public AbstractApduResponseParser {

                                    /**
                                     * The Class SecureSession. A secure security is returned by a open secure security command
                                     */
                                  public:
                                    class SecureSession : public std::enable_shared_from_this<SecureSession> {

                                        /** Challenge transaction counter */
                                      private:
                                        std::vector<char> const challengeTransactionCounter;

                                        /** Challenge random number */
                                        std::vector<char> const challengeRandomNumber;

                                        /** The previous security ratified boolean. */
                                        const bool previousSessionRatified;

                                        /** The manage secure security authorized boolean. */
                                        const bool manageSecureSessionAuthorized;

                                        /** The kif. */
                                        const char kif;

                                        /** The kvc (may be null if it doesn't exist in the considered PO [rev 1.0]). */
                                        const char kvc;

                                        /** The original data. */
                                        std::vector<char> const originalData;

                                        /** The secure security data. */
                                        std::vector<char> const secureSessionData;

                                        /**
                                         * Instantiates a new SecureSession for a Calypso application revision 3
                                         *
                                         * @param challengeTransactionCounter Challenge transaction counter
                                         * @param challengeRandomNumber Challenge random number
                                         * @param previousSessionRatified the previous security ratified
                                         * @param manageSecureSessionAuthorized the manage secure security authorized
                                         * @param kif the KIF from the response of the open secure security APDU command
                                         * @param kvc the KVC from the response of the open secure security APDU command
                                         * @param originalData the original data from the response of the open secure security APDU
                                         *        command
                                         * @param secureSessionData the secure security data from the response of open secure security
                                         *        APDU command
                                         */
                                      public:
                                        SecureSession(std::vector<char> &challengeTransactionCounter, std::vector<char> &challengeRandomNumber, bool previousSessionRatified, bool manageSecureSessionAuthorized, char kif, char kvc, std::vector<char> &originalData,
                                                      std::vector<char> &secureSessionData);

                                        /**
                                         * Instantiates a new SecureSession for a Calypso application revision 2.4
                                         * 
                                         * @param challengeTransactionCounter Challenge transaction counter
                                         * @param challengeRandomNumber Challenge random number
                                         * @param previousSessionRatified the previous security ratified
                                         * @param manageSecureSessionAuthorized the manage secure security authorized
                                         * @param kvc the KVC from the response of the open secure security APDU command
                                         * @param originalData the original data from the response of the open secure security APDU
                                         *        command
                                         * @param secureSessionData the secure security data from the response of open secure security
                                         *        APDU command
                                         */
                                        SecureSession(std::vector<char> &challengeTransactionCounter, std::vector<char> &challengeRandomNumber, bool previousSessionRatified, bool manageSecureSessionAuthorized, char kvc, std::vector<char> &originalData, std::vector<char> &secureSessionData);

                                        virtual std::vector<char> getChallengeTransactionCounter();

                                        virtual std::vector<char> getChallengeRandomNumber();

                                        /**
                                         * Checks if is previous security ratified.
                                         *
                                         * @return the boolean
                                         */
                                        virtual bool isPreviousSessionRatified();

                                        /**
                                         * Checks if is manage secure security authorized.
                                         *
                                         * @return the boolean
                                         */
                                        virtual bool isManageSecureSessionAuthorized();

                                        /**
                                         * Gets the kif.
                                         *
                                         * @return the kif
                                         */
                                        virtual char getKIF();

                                        /**
                                         * Gets the kvc.
                                         *
                                         * @return the kvc
                                         */
                                        virtual char getKVC();

                                        /**
                                         * Gets the original data.
                                         *
                                         * @return the original data
                                         */
                                        virtual std::vector<char> getOriginalData();

                                        /**
                                         * Gets the secure security data.
                                         *
                                         * @return the secure security data
                                         */
                                        virtual std::vector<char> getSecureSessionData();
                                    };

                                  private:
                                    static std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

                                  private:
                                    class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
                                      public:
                                        StaticConstructor();
                                    };

                                  private:
                                    static AbstractOpenSessionRespPars::StaticConstructor staticConstructor;

                                  protected:
                                    std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> getStatusTable() override;

                                  private:
                                    const PoRevision revision;

                                    /** The secure security. */
                                  public:
                                    std::shared_ptr<SecureSession> secureSession;

                                    /**
                                     * Instantiates a new AbstractOpenSessionRespPars.
                                     *
                                     * @param response the response from Open secure security APDU command
                                     * @param revision the revision of the PO
                                     */
                                    AbstractOpenSessionRespPars(std::shared_ptr<ApduResponse> response, PoRevision revision);

                                    static std::shared_ptr<AbstractOpenSessionRespPars> create(std::shared_ptr<ApduResponse> response, PoRevision revision);

                                    virtual std::shared_ptr<SecureSession> toSecureSession(std::vector<char> &apduResponseData)
                                    {
                                        (void)apduResponseData;
                                        return nullptr;
                                    }

                                    virtual std::vector<char> getPoChallenge();

                                    virtual int getTransactionCounterValue();

                                    virtual bool wasRatified();

                                    virtual bool isManageSecureSessionAuthorized();

                                    virtual char getSelectedKif();

                                    virtual char getSelectedKvc();

                                    virtual std::vector<char> getRecordDataRead();

                                  protected:
                                    std::shared_ptr<AbstractOpenSessionRespPars> shared_from_this()
                                    {
                                        return std::static_pointer_cast<AbstractOpenSessionRespPars>(AbstractApduResponseParser::shared_from_this());
                                    }
                                };

                            } // namespace security
                        }     // namespace parser
                    }         // namespace po
                }             // namespace command
            }                 // namespace calypso
        }                     // namespace keyple
    }                         // namespace eclipse
} // namespace org

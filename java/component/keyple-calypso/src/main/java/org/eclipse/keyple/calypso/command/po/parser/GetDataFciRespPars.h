#pragma once

#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduResponseParser.h"
#include <unordered_map>
#include <vector>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace command { class AbstractApduResponseParser; } } } }
namespace org { namespace eclipse { namespace keyple { namespace command { class StatusProperties; } } } }
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace command { namespace po { namespace parser { class FCI; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ApduResponse; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace command { namespace po { namespace parser { class StartupInformation; } } } } } } }

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
                namespace command {
                    namespace po {
                        namespace parser {

                            using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                            using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;

                            /**
                             * This class provides status code properties and the getters to access to the structured fields of
                             * data from response of a Get Data response.
                             */
                            class GetDataFciRespPars final : public AbstractApduResponseParser {


                            private:
                                static const std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

                                                        private:
                                                            class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
                                                            public:
                                                                StaticConstructor();
                                                            };

                                                        private:
                                                            static GetDataFciRespPars::StaticConstructor staticConstructor;


                                static std::vector<int> const bufferSizeIndicatorToBufferSize;

                            protected:
                                std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> getStatusTable() override;

                                /** The fci. */
                            private:
                                const std::shared_ptr<FCI> fci;

                                /**
                                 * Instantiates a new PoFciRespPars.
                                 *
                                 * @param response the response from Get Data APDU commmand
                                 */
                            public:
                                GetDataFciRespPars(std::shared_ptr<ApduResponse> response);

                                std::vector<char> getDfName();

                                std::vector<char> getApplicationSerialNumber();

                                char getBufferSizeIndicator();

                                int getBufferSizeValue();

                                char getPlatformByte();

                                char getApplicationTypeByte();

                                bool isRev3Compliant();

                                bool isRev3_2ModeAvailable();

                                bool isRatificationCommandRequired();

                                bool hasCalypsoStoredValue();

                                bool hasCalypsoPin();

                                char getApplicationSubtypeByte();

                                char getSoftwareIssuerByte();

                                char getSoftwareVersionByte();

                                char getSoftwareRevisionByte();

                                bool isDfInvalidated();

                                /**
                                 * The Class FCI. FCI: file control information
                                 */
                            public:
                                class FCI : public std::enable_shared_from_this<FCI> {

                                    /** The DF Name. */
                                private:
                                    std::vector<char> const dfName;

                                    /** The fci proprietary template. */
                                    std::vector<char> const fciProprietaryTemplate;

                                    /** The fci issuer discretionary data. */
                                    std::vector<char> const fciIssuerDiscretionaryData;

                                    /** The application SN. */
                                    std::vector<char> const applicationSN;

                                    /** The startup information. */
                                    const std::shared_ptr<StartupInformation> startupInformation;

                                    /**
                                     * Instantiates a new FCI.
                                     *
                                     * @param dfName the df name
                                     * @param fciProprietaryTemplate the fci proprietary template
                                     * @param fciIssuerDiscretionaryData the fci issuer discretionary data
                                     * @param applicationSN the application SN
                                     * @param startupInformation the startup information
                                     */
                                public:
                                    FCI(std::vector<char> &dfName, std::vector<char> &fciProprietaryTemplate, std::vector<char> &fciIssuerDiscretionaryData, std::vector<char> &applicationSN, std::shared_ptr<StartupInformation> startupInformation);

                                    /**
                                     * Gets the fci proprietary template.
                                     *
                                     * @return the fci proprietary template
                                     */
                                    virtual std::vector<char> getFciProprietaryTemplate();

                                    /**
                                     * Gets the fci issuer discretionary data.
                                     *
                                     * @return the fci issuer discretionary data
                                     */
                                    virtual std::vector<char> getFciIssuerDiscretionaryData();

                                    /**
                                     * Gets the application SN.
                                     *
                                     * @return the application SN
                                     */
                                    virtual std::vector<char> getApplicationSN();

                                    /**
                                     * Gets the startup information.
                                     *
                                     * @return the startup information
                                     */
                                    virtual std::shared_ptr<StartupInformation> getStartupInformation();

                                    /**
                                     * Gets the DF Name.
                                     *
                                     * @return the DF name
                                     */
                                    virtual std::vector<char> getDfName();

                                };

                                /**
                                 * The Class StartupInformation. The Calypso applications return the Startup Information in the
                                 * answer to the Select Application command. The Startup Information contains several data
                                 * fields (applicationType,software issuer...)
                                 */
                            public:
                                class StartupInformation : public std::enable_shared_from_this<StartupInformation> {

                                    /** The buffer size. */
                                public:
                                    const char bufferSizeIndicator;

                                    /** The platform. */
                                    const char platform;

                                    /** The application type. */
                                    const char applicationType;

                                    /** The application subtype. */
                                    const char applicationSubtype;

                                    /** The software issuer. */
                                    const char softwareIssuer;

                                    /** The software version. */
                                    const char softwareVersion;

                                    /** The software revision. */
                                    const char softwareRevision;

                                    /**
                                     * Instantiates a new StartupInformation.
                                     *
                                     * @param bufferSizeIndicator the buffer size indicator
                                     * @param platform the platform
                                     * @param applicationType the application type
                                     * @param applicationSubtype the application subtype
                                     * @param softwareIssuer the software issuer
                                     * @param softwareVersion the software version
                                     * @param softwareRevision the software revision
                                     */
                                    StartupInformation(char bufferSizeIndicator, char platform, char applicationType, char applicationSubtype, char softwareIssuer, char softwareVersion, char softwareRevision);

                                    StartupInformation(std::vector<char> &buffer);

                                    static std::shared_ptr<StartupInformation> empty();

                                    int hashCode() override;

                                    bool equals(std::shared_ptr<void> obj) override;

                                    /**
                                     * Gets the buffer size.
                                     *
                                     * @return the buffer size
                                     */
                                    virtual char getBufferSizeIndicator();

                                    /**
                                     * Gets the platform.
                                     *
                                     * @return the platform
                                     */
                                    virtual char getPlatform();

                                    /**
                                     * Gets the application type.
                                     *
                                     * @return the application type
                                     */
                                    virtual char getApplicationType();

                                    /**
                                     * Gets the application subtype.
                                     *
                                     * @return the application subtype
                                     */
                                    virtual char getApplicationSubtype();

                                    /**
                                     * Gets the software issuer.
                                     *
                                     * @return the software issuer
                                     */
                                    virtual char getSoftwareIssuer();

                                    /**
                                     * Gets the software version.
                                     *
                                     * @return the software version
                                     */
                                    virtual char getSoftwareVersion();

                                    /**
                                     * Gets the software revision.
                                     *
                                     * @return the software revision
                                     */
                                    virtual char getSoftwareRevision();

                                    virtual bool hasCalypsoPin();

                                    virtual bool hasCalypsoStoreValue();

                                    virtual bool hasRatificationCommandRequired();

                                    virtual bool hasCalypsoRev32modeAvailable();

                                };

                                /**
                                 * Method to get the FCI from the response.
                                 *
                                 * @param apduResponse the apdu response
                                 * @return the FCI template TODO we should check here if the provided FCI data matches an
                                 *         Calypso PO FCI and return null if not
                                 */
                            public:
                                static std::shared_ptr<FCI> toFCI(std::vector<char> &apduResponse);

protected:
                                std::shared_ptr<GetDataFciRespPars> shared_from_this() {
                                    return std::static_pointer_cast<GetDataFciRespPars>(org.eclipse.keyple.command.AbstractApduResponseParser::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}

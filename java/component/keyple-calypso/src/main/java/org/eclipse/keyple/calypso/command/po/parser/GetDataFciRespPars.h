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

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

/* Calypso */
#include "AbstractApduResponseParser_Import.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {

                            using AbstractPoResponseParser = org::eclipse::keyple::calypso::command::po::AbstractPoResponseParser;
                            using ApduResponse             = org::eclipse::keyple::core::seproxy::message::ApduResponse;
                            using Tag                      = org::eclipse::keyple::core::util::bertlv::Tag;
                            using Logger                   = org::eclipse::keyple::common::Logger;
                            using LoggerFactory            = org::eclipse::keyple::common::LoggerFactory;

                            /**
                             * Extracts information from the FCI data returned is response to the selection application command.
                             * <p>
                             * Provides getter methods for all relevant information.
                             */
                            class GetDataFciRespPars final : public AbstractPoResponseParser {
                            protected:
                                const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(GetDataFciRespPars));

                            private:
                                static std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

                            private:
                                class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
                                public:
                                    StaticConstructor();
                                };

                            private:
                                static GetDataFciRespPars::StaticConstructor staticConstructor;


                            protected:
                                std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> getStatusTable() override;

                                /* buffer indicator to buffer size lookup table */
                            private:
                                static std::vector<int> const BUFFER_SIZE_INDICATOR_TO_BUFFER_SIZE;

                                /* BER-TLV tags definitions */
                                /* FCI Template: application class, constructed, tag number Fh => tag field 6Fh */
                                static const std::shared_ptr<Tag> TAG_FCI_TEMPLATE;
                                /* DF Name: context-specific class, primitive, tag number 4h => tag field 84h */
                                static const std::shared_ptr<Tag> TAG_DF_NAME;
                                /*
                                 * FCI Proprietary Template: context-specific class, constructed, tag number 5h => tag field A5h
                                 */
                                static const std::shared_ptr<Tag> TAG_FCI_PROPRIETARY_TEMPLATE;
                                /*
                                 * FCI Issuer Discretionary Data: context-specific class, constructed, tag number Ch => tag
                                 * field BF0Ch
                                 */
                                static const std::shared_ptr<Tag> TAG_FCI_ISSUER_DISCRETIONARY_DATA;
                                /* Application Serial Number: private class, primitive, tag number 7h => tag field C7h */
                                static const std::shared_ptr<Tag> TAG_APPLICATION_SERIAL_NUMBER;
                                /* Discretionary Data: application class, primitive, tag number 13h => tag field 53h */
                                static const std::shared_ptr<Tag> TAG_DISCRETIONARY_DATA;

                                /** attributes result of th FCI parsing */
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                                bool isDfInvalidated_Renamed = false;

//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                                bool isValidCalypsoFCI_Renamed = false;
                                std::vector<char> dfName;
                                std::vector<char> applicationSN;
                                char siBufferSizeIndicator = 0;
                                char siPlatform = 0;
                                char siApplicationType = 0;
                                char siApplicationSubtype = 0;
                                char siSoftwareIssuer = 0;
                                char siSoftwareVersion = 0;
                                char siSoftwareRevision = 0;

                                /** Application type bitmasks features */
                                static constexpr char APP_TYPE_WITH_CALYPSO_PIN = 0x01;
                                static constexpr char APP_TYPE_WITH_CALYPSO_SV = 0x02;
                                static constexpr char APP_TYPE_RATIFICATION_COMMAND_REQUIRED = 0x04;
                                static constexpr char APP_TYPE_CALYPSO_REV_32_MODE = 0x08;

                                /**
                                 * Instantiates a new GetDataFciRespPars from the ApduResponse to a selection application
                                 * command.
                                 * <p>
                                 * The expected FCI structure of a Calypso PO follows this scheme: <code>
                                 * T=6F L=XX (C)                FCI Template
                                 *      T=84 L=XX (P)           DF Name
                                 *      T=A5 L=22 (C)           FCI Proprietary Template
                                 *           T=BF0C L=19 (C)    FCI Issuer Discretionary Data
                                 *                T=C7 L=8 (P)  Application Serial Number
                                 *                T=53 L=7 (P)  Discretionary Data (Startup Information)
                                 * </code>
                                 * <p>
                                 * The ApduResponse provided in argument is parsed according to the above expected structure.
                                 * <p>
                                 * DF Name, Application Serial Number and Startup Information are extracted.
                                 * <p>
                                 * The 7-byte startup information field is also split into 7 private field made available
                                 * through dedicated getter methods.
                                 * <p>
                                 * All fields are pre-initialized to handle the case where the parsing fails.
                                 * <p>
                                 * 
                                 * @param selectApplicationResponse the select application response from Get Data APDU command
                                 */
                            public:
                                GetDataFciRespPars(std::shared_ptr<ApduResponse> selectApplicationResponse);

                                bool isValidCalypsoFCI();

                                std::vector<char> getDfName();

                                std::vector<char> getApplicationSerialNumber();

                                char getBufferSizeIndicator();

                                int getBufferSizeValue();

                                char getPlatformByte();

                                char getApplicationTypeByte();


                                bool isRev3_2ModeAvailable();

                                bool isRatificationCommandRequired();

                                bool hasCalypsoStoredValue();

                                bool hasCalypsoPin();

                                char getApplicationSubtypeByte();

                                char getSoftwareIssuerByte();

                                char getSoftwareVersionByte();

                                char getSoftwareRevisionByte();

                                bool isDfInvalidated();

protected:
                                std::shared_ptr<GetDataFciRespPars> shared_from_this() {
                                    return std::static_pointer_cast<GetDataFciRespPars>(AbstractPoResponseParser::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}

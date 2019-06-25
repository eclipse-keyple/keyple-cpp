#pragma once

#include "../../AbstractSamResponseParser.h"
#include <unordered_map>
#include <vector>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace message { class ApduResponse; } } } } } }

/********************************************************************************
 * Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
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
                    namespace sam {
                        namespace parser {
                            namespace security {

                                using AbstractSamResponseParser = org::eclipse::keyple::calypso::command::sam::AbstractSamResponseParser;
                                using ApduResponse = org::eclipse::keyple::core::seproxy::message::ApduResponse;

                                class CardGenerateKeyRespPars : public AbstractSamResponseParser {
                                private:
                                    static const std::unordered_map<Integer, std::shared_ptr<StatusProperties>> STATUS_TABLE;

                                                                private:
                                                                    class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
                                                                    public:
                                                                        StaticConstructor();
                                                                    };

                                                                private:
                                                                    static CardGenerateKeyRespPars::StaticConstructor staticConstructor;


                                protected:
                                    std::unordered_map<Integer, std::shared_ptr<StatusProperties>> getStatusTable() override;

                                    /**
                                     * Instantiates a new CardGenerateKeyRespPars.
                                     *
                                     * @param response from the SAM
                                     */
                                public:
                                    CardGenerateKeyRespPars(std::shared_ptr<ApduResponse> response);

                                    /**
                                     * Gets the 32 bytes of ciphered data.
                                     *
                                     * @return the ciphered data byte array or null if the operation failed
                                     */
                                    virtual std::vector<char> getCipheredData();

protected:
                                    std::shared_ptr<CardGenerateKeyRespPars> shared_from_this() {
                                        return std::static_pointer_cast<CardGenerateKeyRespPars>(org.eclipse.keyple.calypso.command.sam.AbstractSamResponseParser::shared_from_this());
                                    }
                                };

                            }
                        }
                    }
                }
            }
        }
    }
}

#pragma once

#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduResponseParser.h"
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace command { class AbstractApduResponseParser; } } } }
namespace org { namespace eclipse { namespace keyple { namespace command { class StatusProperties; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ApduResponse; } } } } }

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
                            namespace session {


                                using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;

                                /**
                                 * Close Secure Session (008E) response parser. See specs: Calypso / page 104 / 9.5.2 - Close Secure
                                 * Session
                                 */
                                class CloseSessionRespPars final : public AbstractApduResponseParser {

                                private:
                                    static const std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

                                private:
                                    class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
                                    public:
                                        StaticConstructor();
                                    };

                                private:
                                    static CloseSessionRespPars::StaticConstructor staticConstructor;


                                protected:
                                    std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> getStatusTable() override;

                                    /** The signatureLo. */
                                private:
                                    std::vector<char> signatureLo;

                                    /** The postponed data. */
                                    std::vector<char> postponedData;

                                    /**
                                     * Instantiates a new CloseSessionRespPars from the response.
                                     *
                                     * @param response from CloseSessionCmdBuild
                                     */
                                public:
                                    CloseSessionRespPars(std::shared_ptr<ApduResponse> response);

                                private:
                                    void parse(std::vector<char> &response);

                                public:
                                    std::vector<char> getSignatureLo();

                                    std::vector<char> getPostponedData();

protected:
                                    std::shared_ptr<CloseSessionRespPars> shared_from_this() {
                                        return std::static_pointer_cast<CloseSessionRespPars>(AbstractApduResponseParser::shared_from_this());
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

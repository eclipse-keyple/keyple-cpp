#pragma once

#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduResponseParser.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
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
                    namespace sam {
                        namespace parser {
                            namespace session {


                                using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;

                                /**
                                 * Digest update multiple response parser. See specs: Calypso / page 54 / 7.4.2 - Session MAC
                                 * computation
                                 */
                                class DigestUpdateMultipleRespPars : public AbstractApduResponseParser {
                                    /**
                                     * Instantiates a new DigestUpdateMultipleRespPars.
                                     *
                                     * @param response the response
                                     */
                                public:
                                    DigestUpdateMultipleRespPars(std::shared_ptr<ApduResponse> response);

protected:
                                    std::shared_ptr<DigestUpdateMultipleRespPars> shared_from_this() {
                                        return std::static_pointer_cast<DigestUpdateMultipleRespPars>(AbstractApduResponseParser::shared_from_this());
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

#pragma once

#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduResponseParser.h"
#include <vector>
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
                                 * SAM get challenge. See specs: Calypso / Page 108 / 9.5.4 - Get challenge
                                 */
                                class SamGetChallengeRespPars : public AbstractApduResponseParser {
                                    /**
                                     * Instantiates a new SamGetChallengeRespPars .
                                     *
                                     * @param response of the SamGetChallengeCmdBuild
                                     */
                                public:
                                    SamGetChallengeRespPars(std::shared_ptr<ApduResponse> response);

                                    /**
                                     * Gets the challenge.
                                     *
                                     * @return the challenge
                                     */
                                    virtual std::vector<char> getChallenge();

protected:
                                    std::shared_ptr<SamGetChallengeRespPars> shared_from_this() {
                                        return std::static_pointer_cast<SamGetChallengeRespPars>(AbstractApduResponseParser::shared_from_this());
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

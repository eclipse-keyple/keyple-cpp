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

#include <memory>

/* Core */
#include "AbstractApduResponseParser_Import.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace parser {
                            namespace security {

                                using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;

                                /**
                                 * Select diversifier response parser. See specs: Calypso / page 54 / 7.4.2 - Session MAC
                                 * computation
                                 */
                                class SelectDiversifierRespPars : public AbstractApduResponseParser {

                                    /**
                                     * Instantiates a new SelectDiversifierRespPars.
                                     *
                                     * @param response the response
                                     */
                                public:
                                    SelectDiversifierRespPars(std::shared_ptr<ApduResponse> response);

protected:
                                    std::shared_ptr<SelectDiversifierRespPars> shared_from_this() {
                                        return std::static_pointer_cast<SelectDiversifierRespPars>(AbstractApduResponseParser::shared_from_this());
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

#pragma once

#include "AbstractOpenSessionRespPars.h"
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
                    namespace po {
                        namespace parser {
                            namespace session {

                                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;

                                class OpenSession32RespPars final : public AbstractOpenSessionRespPars {

                                public:
                                    OpenSession32RespPars(std::shared_ptr<ApduResponse> response);

                                    /**
                                     * Method to get a Secure Session from the response in revision 3.2 mode.
                                     *
                                     * @param apduResponseData the apdu response data
                                     * @return a SecureSession
                                     */
                                    std::shared_ptr<SecureSession> toSecureSession(std::vector<char> &apduResponseData) override;

                                    static std::shared_ptr<SecureSession> createSecureSession(std::vector<char> &apduResponse);

protected:
                                    std::shared_ptr<OpenSession32RespPars> shared_from_this() {
                                        return std::static_pointer_cast<OpenSession32RespPars>(AbstractOpenSessionRespPars::shared_from_this());
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

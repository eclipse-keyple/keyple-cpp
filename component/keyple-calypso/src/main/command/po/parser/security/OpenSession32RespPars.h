/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "AbstractOpenSessionRespPars.h"
#include "OpenSession32CmdBuild.h"

#include <vector>
#include <memory>

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::core::seproxy::message;

class KEYPLECALYPSO_API OpenSession32RespPars final
: public AbstractOpenSessionRespPars {
public:
    /**
     * Instantiates a new OpenSession32RespPars from the response.
     *
     * @param response from OpenSession32RespPars
     * @param builder the reference to the builder that created this parser
     */
    OpenSession32RespPars(std::shared_ptr<ApduResponse> response,
                          OpenSession32CmdBuild* builder);

    /**
     * Method to get a Secure Session from the response in revision 3.2 mode.
     *
     * @param apduResponseData the apdu response data
     * @return a SecureSession
     */
    std::shared_ptr<SecureSession> toSecureSession(
        const std::vector<uint8_t>& apduResponseData);

    /**
     *
     */
    static std::shared_ptr<SecureSession> createSecureSession(
        const std::vector<uint8_t>& apduResponse);

protected:
    /**
     *
     */
    std::shared_ptr<OpenSession32RespPars> shared_from_this()
    {
        return std::static_pointer_cast<OpenSession32RespPars>(
            AbstractOpenSessionRespPars::shared_from_this());
    }
};

}
}
}
}
}
}

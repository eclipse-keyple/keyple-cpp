/******************************************************************************
 * Copyright (c) 20120Calypso Networks Association                            *
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

#include <memory>
#include <vector>

#include "AbstractOpenSessionRespPars.h"
#include "OpenSession10CmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::core::seproxy::message;

class KEYPLECALYPSO_API OpenSession10RespPars final
: public AbstractOpenSessionRespPars {
public:
    /**
     * Instantiates a new OpenSession10RespPars from the response.
     *
     * @param response from OpenSession10RespPars
     * @param builder the reference to the builder that created this parser
     */
    OpenSession10RespPars(std::shared_ptr<ApduResponse> response);

    /**
     *
     */
    std::shared_ptr<SecureSession> toSecureSession(
        const std::vector<uint8_t>& apduResponseData,
        OpenSession10CmdBuild* builder);

    /**
     *
     */
    static std::shared_ptr<SecureSession> createSecureSession(
        const std::vector<uint8_t>& apduResponseData);

protected:
    /**
     *
     */
    std::shared_ptr<OpenSession10RespPars> shared_from_this()
    {
        return std::static_pointer_cast<OpenSession10RespPars>(
            AbstractOpenSessionRespPars::shared_from_this());
    }
};

}
}
}
}
}
}

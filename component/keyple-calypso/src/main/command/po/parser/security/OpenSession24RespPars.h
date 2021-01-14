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

#include <memory>
#include <vector>

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "AbstractOpenSessionRespPars.h"
#include "OpenSession24CmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::core::seproxy::message;

class KEYPLECALYPSO_API OpenSession24RespPars final
: public AbstractOpenSessionRespPars {
public:
    /**
     * Instantiates a new OpenSession24RespPars from the response.
     *
     * @param response from OpenSession24RespPars
     * @param builder the reference to the builder that created this parser
     */
    OpenSession24RespPars(std::shared_ptr<ApduResponse> response,
                          OpenSession24CmdBuild* builder);

    /**
     *
     */
    std::shared_ptr<SecureSession> toSecureSession(
        const std::vector<uint8_t>& apduResponseData);

    /**
     *
     */
    static std::shared_ptr<SecureSession> createSecureSession(
        const std::vector<uint8_t>& apduResponseData);
};

}
}
}
}
}
}

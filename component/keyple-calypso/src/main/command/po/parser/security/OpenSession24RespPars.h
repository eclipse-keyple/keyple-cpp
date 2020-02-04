/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
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
#include "exceptionhelper.h"

/* Calypso */
#include "AbstractOpenSessionRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::core::seproxy::message;

class OpenSession24RespPars final : public AbstractOpenSessionRespPars {
public:
    /**
     *
     */
    OpenSession24RespPars(std::shared_ptr<ApduResponse> response);

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

protected:
    /**
     *
     */
    std::shared_ptr<OpenSession24RespPars> shared_from_this()
    {
        return std::static_pointer_cast<OpenSession24RespPars>(
                   AbstractOpenSessionRespPars::shared_from_this());
    }
};

}
}
}
}
}
}

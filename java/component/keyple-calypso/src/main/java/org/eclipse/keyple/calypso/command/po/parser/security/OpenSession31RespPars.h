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

#include <vector>
#include <memory>

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "AbstractOpenSessionRespPars.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using ApduResponse = org::eclipse::keyple::core::seproxy::message::ApduResponse;

class OpenSession31RespPars final : public AbstractOpenSessionRespPars {

public:
    OpenSession31RespPars(std::shared_ptr<ApduResponse> response);

    std::shared_ptr<SecureSession> toSecureSession(std::vector<char> &apduResponseData);

protected:
    std::shared_ptr<OpenSession31RespPars> shared_from_this() {
        return std::static_pointer_cast<OpenSession31RespPars>(AbstractOpenSessionRespPars::shared_from_this());
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

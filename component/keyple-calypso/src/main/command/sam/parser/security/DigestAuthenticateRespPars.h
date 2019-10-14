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

/* Calypso */
#include "AbstractSamResponseParser.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using AbstractSamResponseParser  = org::eclipse::keyple::calypso::command::sam::AbstractSamResponseParser;
using ApduResponse               = org::eclipse::keyple::core::seproxy::message::ApduResponse;

/**
 * Digest authentication response parser. See specs: Calypso / page 54 / 7.4.2 - Session MAC
 * computation
 */
class DigestAuthenticateRespPars : public AbstractSamResponseParser {
public:
    /**
     * Instantiates a new DigestAuthenticateRespPars.
     *
     * @param response from the SAM DigestAuthenticateCmdBuild
     */
    DigestAuthenticateRespPars(std::shared_ptr<ApduResponse> response);

    /**
     *
     */
    virtual ~DigestAuthenticateRespPars() {}

protected:
    /**
     *
     */
    std::shared_ptr<DigestAuthenticateRespPars> shared_from_this()
    {
        return std::static_pointer_cast<DigestAuthenticateRespPars>(AbstractSamResponseParser::shared_from_this());
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

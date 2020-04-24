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

/* Core */
#include "AbstractApduResponseParser.h"

/* Calypso */
#include "AbstractSamResponseParser.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::core::seproxy::message;

/**
 * Digest init response parser. See specs: Calypso / page 54 / 7.4.2 -
 * Session MAC computation
 */
class DigestInitRespPars : public AbstractSamResponseParser {
public:
    /**
     * Instantiates a new DigestInitRespPars.
     *
     * @param response from DigestInitCmdBuild
     */
    DigestInitRespPars(std::shared_ptr<ApduResponse> response);

    /**
     *
     */
    virtual ~DigestInitRespPars()
    {
    }

protected:
    /**
     *
     */
    std::shared_ptr<DigestInitRespPars> shared_from_this()
    {
        return std::static_pointer_cast<DigestInitRespPars>(
            AbstractSamResponseParser::shared_from_this());
    }
};

}
}
}
}
}
}

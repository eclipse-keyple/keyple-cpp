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
#include "AbstractSamResponseParser.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using AbstractSamResponseParser = org::eclipse::keyple::calypso::command::sam::AbstractSamResponseParser;
using ApduResponse              = org::eclipse::keyple::core::seproxy::message::ApduResponse;

/**
 * SAM get challenge. See specs: Calypso / Page 108 / 9.5.4 - Get challenge
 */
class SamGetChallengeRespPars : public AbstractSamResponseParser {
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
    std::shared_ptr<SamGetChallengeRespPars> shared_from_this()
    {
        return std::static_pointer_cast<SamGetChallengeRespPars>(AbstractSamResponseParser::shared_from_this());
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

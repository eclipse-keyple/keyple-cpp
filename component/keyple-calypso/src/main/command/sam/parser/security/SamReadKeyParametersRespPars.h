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

/* Calypso */
#include "AbstractSamResponseParser.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using namespace keyple::calypso::command::sam;

/**
 * SAM read key parameters.
 */
class SamReadKeyParametersRespPars : public AbstractSamResponseParser {
public:
    /**
     * Instantiates a new SamReadKeyParametersRespPars.
     *
     * @param response of the SamReadKeyParametersRespPars
     */
    SamReadKeyParametersRespPars(std::shared_ptr<ApduResponse> response);

    /**
     * Gets the key parameters.
     *
     * @return the key parameters
     */
    std::vector<uint8_t> getKeyParameters() const;
};

}
}
}
}
}
}

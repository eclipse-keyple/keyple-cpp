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
 * SAM read event counter.
 */
class SamReadEventCounterRespPars : public AbstractSamResponseParser {
public:
    /**
     * Instantiates a new SamReadEventCounterRespPars.
     *
     * @param response of the SamReadEventCounterRespPars
     */
    SamReadEventCounterRespPars(std::shared_ptr<ApduResponse> response);

    /**
     * Gets the key parameters.
     *
     * @return the counter data (Value or Record)
     */
    std::vector<uint8_t> getCounterData() const;
};

}
}
}
}
}
}

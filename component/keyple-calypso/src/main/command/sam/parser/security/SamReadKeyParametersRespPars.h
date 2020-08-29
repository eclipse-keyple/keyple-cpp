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

#include <map>

/* Calypso */
#include "AbstractSamResponseParser.h"
#include "KeypleCalypsoExport.h"
#include "SamReadKeyParametersCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::builder::security;

/**
 * SAM read key parameters.
 */
class KEYPLECALYPSO_API SamReadKeyParametersRespPars final
: public AbstractSamResponseParser {
public:
    /**
     * Instantiates a new SamReadKeyParametersRespPars.
     *
     * @param response of the SamReadKeyParametersRespPars
     * @param builder the reference to the builder that created this parser
     */
    SamReadKeyParametersRespPars(const std::shared_ptr<ApduResponse> response,
                                 SamReadKeyParametersCmdBuild* builder);

    /**
     * Gets the key parameters.
     *
     * @return the key parameters
     */
    const std::vector<uint8_t> getKeyParameters() const;

protected:
    /**
     *
     */
    const std::map<int, std::shared_ptr<StatusProperties>>&
        getStatusTable() const override;

private:
    /**
     *
     */
    static const std::map<int, std::shared_ptr<StatusProperties>> STATUS_TABLE;
};

}
}
}
}
}
}

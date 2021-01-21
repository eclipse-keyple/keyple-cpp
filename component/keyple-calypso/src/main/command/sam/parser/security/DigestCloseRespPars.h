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
#include "AbstractSamResponseParser.h"

/* Calypso */
#include "DigestCloseCmdBuild.h"
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::core::seproxy::message;

/**
 * Digest close response parser. See specs: Calypso / page 54 / 7.4.2 - Session
 * MAC computation
 */
class KEYPLECALYPSO_API DigestCloseRespPars final
: public AbstractSamResponseParser {
public:
    /**
     * Instantiates a new DigestCloseRespPars.
     *
     * @param response from the DigestCloseCmdBuild
     * @param builder the reference to the builder that created this parser
     */
    DigestCloseRespPars(const std::shared_ptr<ApduResponse> response,
                        DigestCloseCmdBuild* builder);

    /**
     * Gets the sam signature.
     *
     * @return the sam half session signature
     */
    const std::vector<uint8_t> getSignature() const;

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

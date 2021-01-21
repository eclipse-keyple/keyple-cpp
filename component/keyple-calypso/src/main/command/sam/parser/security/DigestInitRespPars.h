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

/* Core */
#include "AbstractApduResponseParser.h"

/* Calypso */
#include "AbstractSamResponseParser.h"
#include "DigestInitCmdBuild.h"
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
 * Digest init response parser. See specs: Calypso / page 54 / 7.4.2 -
 * Session MAC computation
 */
class KEYPLECALYPSO_API DigestInitRespPars final
: public AbstractSamResponseParser {
public:
    /**
     * Instantiates a new DigestInitRespPars.
     *
     * @param response from DigestInitCmdBuild
     * @param builder the reference to the builder that created this parser
     */
    DigestInitRespPars(const std::shared_ptr<ApduResponse> response,
                       DigestInitCmdBuild* builder);

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

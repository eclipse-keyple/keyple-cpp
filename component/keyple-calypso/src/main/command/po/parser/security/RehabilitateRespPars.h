/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

/* Calypso */
#include "AbstractPoResponseParser.h"
#include "RehabilitateCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po::builder::security;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

/**
 * Rehabilitate response parser.
 */
class RehabilitateRespPars final : public AbstractPoResponseParser {
public:
    /**
     * Instantiates a new RehabilitateRespPars.
     *
     * @param response the response from Rehabilitate APDU Command
     * @param builder the reference to the builder that created this parser
     */
    RehabilitateRespPars(std::shared_ptr<ApduResponse> response, RehabilitateCmdBuild* builder);

protected:
    /**
     *
     */
    const std::map<int, std::shared_ptr<StatusProperties>>& getStatusTable() const override;

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

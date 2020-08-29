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

#include "AbstractPoResponseParser.h"
#include "GetDataTraceCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

/**
 * Returns the traceability data obtained from the Get Data command response.
 * <p>
 * Provides getter methods for all relevant information.
 */
class GetDataTraceRespPars final : public AbstractPoResponseParser {
public:
    /**
     * Instantiates a new GetDataTraceRespPars from the ApduResponse to a
     * selection application command.
     *
     * @param response the Traceability Data response from Get Data APDU command
     * @param builder the reference to the builder that created this parser
     */
    GetDataTraceRespPars(std::shared_ptr<ApduResponse> response,
                         GetDataTraceCmdBuild* builder);

protected:
    /**
     *
     */
    const std::map<int, std::shared_ptr<StatusProperties>>& getStatusTable()
        const override;

private:
    /**
     *
     */
    static std::map<
        int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
        STATUS_TABLE;
}

}
}
}
}
}
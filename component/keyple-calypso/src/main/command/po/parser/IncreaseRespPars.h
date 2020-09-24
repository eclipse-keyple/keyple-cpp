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

#include <string>
#include <map>
#include <vector>
#include <memory>

/* Core */
#include "AbstractApduResponseParser.h"

/* Calypso */
#include "AbstractPoResponseParser.h"
#include "IncreaseCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::core::seproxy::message;

/**
 * Increase (0032) response parser. See specs: Calypso / page 85 / 9.4.4
 * Increase
 */
class KEYPLECALYPSO_API IncreaseRespPars final
: public AbstractPoResponseParser {
public:
    /**
     * Constructor to build a parser of the Increase command response.
     *
     * @param response response to parse
     * @param builder the reference to the builder that created this parser
     */
    IncreaseRespPars(std::shared_ptr<ApduResponse> response,
                     IncreaseCmdBuild* builder);

    /**
     * Returns the new counter value as an int between 0
     *
     * @return the new value
     */
    int getNewValue();

protected:
    /**
     *
     */
    const std::map<int, std::shared_ptr<StatusProperties>>& getStatusTable()
        const override;

    /**
     *
     */
    std::shared_ptr<IncreaseRespPars> shared_from_this()
    {
        return std::static_pointer_cast<IncreaseRespPars>(
            AbstractPoResponseParser::shared_from_this());
    }

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

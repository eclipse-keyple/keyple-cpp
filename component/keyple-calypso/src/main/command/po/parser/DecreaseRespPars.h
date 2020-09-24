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

/* Calypso */
#include "AbstractPoResponseParser.h"
#include "DecreaseCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::core::seproxy::message;

/**
 * Decrease (0030) response parser. See specs: Calypso / page 83 /
 * 9.4.2 Decrease
 */
class KEYPLECALYPSO_API DecreaseRespPars final
: public AbstractPoResponseParser {
public:
    /**
     * Instantiates a new DecreaseRespPars.
     *
     * @param response the response from the PO
     * @param builder the reference to the builder that created this parser
     */
    DecreaseRespPars(std::shared_ptr<ApduResponse> response,
                     DecreaseCmdBuild* builder);

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
    std::shared_ptr<DecreaseRespPars> shared_from_this()
    {
        return std::static_pointer_cast<DecreaseRespPars>(
            AbstractApduResponseParser::shared_from_this());
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

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

#include <memory>

#include "AbstractApduResponseParser.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

class AbstractSamResponseParser : public AbstractApduResponseParser {
public:
    /**
     * Constructor to build a parser of the APDU response.
     *
     * @param response response to parse
     */
    AbstractSamResponseParser(std::shared_ptr<ApduResponse> response);

protected:
    /**
     *
     */
    std::shared_ptr<AbstractSamResponseParser> shared_from_this()
    {
        return std::static_pointer_cast<AbstractSamResponseParser>(
            AbstractApduResponseParser::shared_from_this());
    }
};

}
}
}
}

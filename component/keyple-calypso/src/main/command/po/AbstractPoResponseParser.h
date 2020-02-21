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

/* Common */
#include "Export.h"

#include "AbstractApduResponseParser_Import.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

class EXPORT AbstractPoResponseParser : public AbstractApduResponseParser {
public:
    /**
     * the generic abstract constructor to build a parser of the APDU response.
     *
     * @param response response to parse
     */
    AbstractPoResponseParser(std::shared_ptr<ApduResponse> response);

    /**
     *
     */
    virtual ~AbstractPoResponseParser()
    {
    }

protected:
    std::shared_ptr<AbstractPoResponseParser> shared_from_this()
    {
        return std::static_pointer_cast<AbstractPoResponseParser>(AbstractApduResponseParser::shared_from_this());
    }
};

}
}
}
}

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
#include <memory>
#include <string>

#include "AbstractPoResponseParser.h"
#include "ChangeKeyCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po;
using namespace keyple::core::seproxy::message;

class KEYPLECALYPSO_API ChangeKeyRespPars : public AbstractPoResponseParser {
private:
    /**
     *
     */
    static
    std::map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
        STATUS_TABLE;

public:
    /**
     * Instantiates a new ChangeKeyRespPars
     *
     * @param response the response from the PO
     * @param builder the reference to the builder that created this parser
     */
    ChangeKeyRespPars(std::shared_ptr<ApduResponse> response,
                      ChangeKeyCmdBuild* builder);

    /**
     *
     */
    virtual ~ChangeKeyRespPars() = default;

protected:
    /**
     *
     */
    std::map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
        getStatusTable() const override;

protected:
    /**
     *
     */
    std::shared_ptr<ChangeKeyRespPars> shared_from_this()
    {
        return std::static_pointer_cast<ChangeKeyRespPars>(
            AbstractPoResponseParser::shared_from_this());
    }
};

}
}
}
}
}
}

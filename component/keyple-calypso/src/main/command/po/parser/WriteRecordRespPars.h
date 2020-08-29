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

/* Core */
#include "AbstractPoResponseParser.h"
#include "ApduResponse.h"

/* Calypso */
#include "WriteRecordCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::core::command;
using namespace keyple::calypso::command::po;
using namespace keyple::core::seproxy::message;

/**
 * Write Record response parser. See specs: Calypso 3.2 / page 99 / 9.4.13 -
 * Write Record
 */
class KEYPLECALYPSO_API WriteRecordRespPars final
: public AbstractPoResponseParser {
public:
    /**
     * Instantiates a new WriteRecordRespPars
     *
     * @param response the response from the PO
     * @param builder the reference to the builder that created this parser
     */
    WriteRecordRespPars(std::shared_ptr<ApduResponse> response,
                        WriteRecordCmdBuild* builder);

protected:
    /**
     *
     */
    const std::map<int, std::shared_ptr<StatusProperties>>& getStatusTable()
        const override;

    /**
     *
     */
    std::shared_ptr<WriteRecordRespPars> shared_from_this()
    {
        return std::static_pointer_cast<WriteRecordRespPars>(
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

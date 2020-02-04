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

#include <unordered_map>
#include <memory>

/* Core */
#include "AbstractApduResponseParser_Import.h"
#include "AbstractPoResponseParser.h"
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::core::command;
using namespace keyple::calypso::command::po;
using namespace keyple::core::seproxy::message;

/**
 * Update Record response parser. See specs: Calypso / page 96 / 9.4.11 - Update
 * Record
 */
class UpdateRecordRespPars final : public AbstractPoResponseParser {
public:
    /**
     * Instantiates a new UpdateRecordRespPars.
     *
     * @param response the response from the PO
     */
    UpdateRecordRespPars(std::shared_ptr<ApduResponse> response);

protected:
    /**
     *
     */
    std::unordered_map<int, std::shared_ptr<
        AbstractApduResponseParser::StatusProperties>> getStatusTable()
            const override;

    /**
     *
     */
    std::shared_ptr<UpdateRecordRespPars> shared_from_this()
    {
        return std::static_pointer_cast<UpdateRecordRespPars>(
                   AbstractPoResponseParser::shared_from_this());
    }

private:
    /**
     *
     */
    static std::unordered_map<int, std::shared_ptr<
               AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

    /**
     *
     */
    class StaticConstructor
    : public std::enable_shared_from_this<StaticConstructor> {
    public:
        StaticConstructor();
    };

    /**
     *
     */
    static UpdateRecordRespPars::StaticConstructor staticConstructor;

};

}
}
}
}
}

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

#include "AbstractPoResponseParser.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po;
using namespace keyple::core::seproxy::message;

/**
 * Update records response parser. See specs: Calypso / page 96 / 9.4.1.1 -
 * Update record
 */
class AppendRecordRespPars final : public AbstractPoResponseParser {
private:
    /**
     *
     */
    static std::unordered_map<int, std::shared_ptr<
               AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

private:
    /**
     *
     */
    class StaticConstructor
    : public std::enable_shared_from_this<StaticConstructor> {
    public:
        /**
         *
         */
        StaticConstructor();
    };

private:
    /**
     *
     */
    static AppendRecordRespPars::StaticConstructor staticConstructor;

public:
    /**
     * Instantiates a new AppendRecordRespPars.
     *
     * @param response the response from the PO
     */
    AppendRecordRespPars(std::shared_ptr<ApduResponse> response);

protected:
    /**
     * Get the internal status table
     *
     * @return Status table
     */
    std::unordered_map<int, std::shared_ptr<
        AbstractApduResponseParser::StatusProperties>> getStatusTable() const
            override;

    /**
     *
     */
    std::shared_ptr<AppendRecordRespPars> shared_from_this()
    {
        return std::static_pointer_cast<AppendRecordRespPars>(
                   AbstractPoResponseParser::shared_from_this());
    }
};

}
}
}
}
}

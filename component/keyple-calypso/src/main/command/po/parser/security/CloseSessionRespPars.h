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
#include <vector>
#include <stdexcept>
#include <memory>

/* Core */
#include "AbstractApduResponseParser_Import.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

/**
 * Close Secure Session (008E) response parser. See specs: Calypso / page 104 /
 * 9.5.2 - Close Secure Session
 */
class CloseSessionRespPars final : public AbstractApduResponseParser {
public:
    /**
     * Instantiates a new CloseSessionRespPars from the response.
     *
     * @param response from CloseSessionCmdBuild
     */
    CloseSessionRespPars(std::shared_ptr<ApduResponse> response);

    /**
     *
     */
    const std::vector<uint8_t>& getSignatureLo() const;

    /**
     *
     */
    const std::vector<uint8_t>& getPostponedData() const;

protected:
    /**
     *
     */
    std::unordered_map<
        int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
    getStatusTable() const override;

    /**
     *
     */
    std::shared_ptr<CloseSessionRespPars> shared_from_this()
    {
        return std::static_pointer_cast<CloseSessionRespPars>(
            AbstractApduResponseParser::shared_from_this());
    }

private:
    /**
     * The signatureLo
     */
    std::vector<uint8_t> signatureLo;

    /**
     * The postponed data
     */
    std::vector<uint8_t> postponedData;

    /**
     *
     */
    static std::unordered_map<
        int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
        STATUS_TABLE;

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

    /**
     *
     */
    static CloseSessionRespPars::StaticConstructor staticConstructor;

    /**
     *
     */
    void parse(const std::vector<uint8_t>& response);
};

}
}
}
}
}
}

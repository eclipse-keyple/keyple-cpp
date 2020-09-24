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
#include <vector>
#include <memory>

/* Calypso */
#include "AbstractPoResponseParser.h"
#include "CloseSessionCmdBuild.h"
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

using StatusProperties = AbstractApduResponseParser::StatusProperties;
/**
 * Close Secure Session (008E) response parser. See specs: Calypso / page 104 /
 * 9.5.2 - Close Secure Session
 */
class KEYPLECALYPSO_API CloseSessionRespPars final
: public AbstractPoResponseParser {
public:
    /**
     * Instantiates a new CloseSessionRespPars from the response.
     *
     * @param response from CloseSessionCmdBuild
     * @param builder the reference to the builder that created this parser
     */
    CloseSessionRespPars(std::shared_ptr<ApduResponse> response,
                         CloseSessionCmdBuild* builder);

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
    const std::map<int, std::shared_ptr<StatusProperties>>& getStatusTable()
        const override;

private:
    /**
     * The signatureLo
     */
    std::vector<uint8_t> mSignatureLo;

    /**
     * The postponed data
     */
    std::vector<uint8_t> mPostponedData;

    /**
     *
     */
    static const  std::map<int, std::shared_ptr<StatusProperties>> STATUS_TABLE;

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

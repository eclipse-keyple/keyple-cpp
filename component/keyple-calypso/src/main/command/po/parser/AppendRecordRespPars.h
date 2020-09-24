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

/* Calypso */
#include "AbstractPoResponseParser.h"
#include "AppendRecordCmdBuild.h"
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::core::seproxy::message;

/**
 * Update records response parser. See specs: Calypso / page 96 / 9.4.1.1 -
 * Update record
 */
class KEYPLECALYPSO_API AppendRecordRespPars final
: public AbstractPoResponseParser {
public:
    /**
     * Instantiates a new AppendRecordRespPars.
     *
     * @param response the response from the PO
     * @param builder the reference to the builder that created this parser
     */
    AppendRecordRespPars(std::shared_ptr<ApduResponse> response,
                         AppendRecordCmdBuild* builder);

protected:
    /**
     * Get the internal status table
     *
     * @return Status table
     */
    const std::map<int, std::shared_ptr<StatusProperties>>& getStatusTable()
        const override;

    /**
     *
     */
    std::shared_ptr<AppendRecordRespPars> shared_from_this()
    {
        return std::static_pointer_cast<AppendRecordRespPars>(
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

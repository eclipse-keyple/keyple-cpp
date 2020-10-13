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
#include "UpdateRecordCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::core::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::core::seproxy::message;

/**
 * Update Record response parser. See specs: Calypso / page 96 / 9.4.11 - Update
 * Record
 */
class KEYPLECALYPSO_API UpdateRecordRespPars final
: public AbstractPoResponseParser {
public:
    /**
     * Instantiates a new UpdateRecordRespPars.
     *
     * @param response the response from the PO
     * @param builder the reference to the builder that created this parser
     */
    UpdateRecordRespPars(std::shared_ptr<ApduResponse> response,
                         UpdateRecordCmdBuild* builder);

protected:
    /**
     *
     */
    const std::map<int, std::shared_ptr<StatusProperties>>& getStatusTable()
        const override;

private:
    /**
     *
     */
    static const std::map<int, std::shared_ptr<StatusProperties>> STATUS_TABLE;

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

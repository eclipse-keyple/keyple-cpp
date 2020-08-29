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
#include "AbstractApduResponseParser.h"

/* Calypso */
#include "AbstractSamCommandBuilder.h"

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
     * @param builder the reference of the builder that created the parser
     */
    AbstractSamResponseParser(const std::shared_ptr<ApduResponse> response,
                              AbstractSamCommandBuilder* builder);

    /**
     *
     */
    virtual ~AbstractSamResponseParser() = default;

    /**
     * {@inheritDoc}
     */
    std::shared_ptr<bstractSamCommandBuilder> getBuilder() override;

    /**
     * {@inheritDoc}
     */
    void checkStatus() const override;

protected:
    /**
     *
     */
    static const std::map<int, std::shared_ptr<StatusProperties>> STATUS_TABLE;

    /**
     *
     */
    const std::map<int, std::shared_ptr<StatusProperties>>&
        getStatusTable() const override;

    /**
     * {@inheritDoc}
     */
    KeypleSeCommandException buildCommandException(
        const type_info& exceptionClass,
        const std::string& message, SeCommand commandRef, int statusCode)
        override;

};

}
}
}
}

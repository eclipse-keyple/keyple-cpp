/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include <map>
#include <memory>
#include <typeinfo>

/* Core */
#include "AbstractApduResponseParser.h"

/* Calypso */
#include "AbstractSamCommandBuilder.h"
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

class KEYPLECALYPSO_API AbstractSamResponseParser : public AbstractApduResponseParser {
public:
    /**
     * Constructor to build a parser of the APDU response.
     *
     * @param response response to parse
     * @param builder the reference of the builder that created the parser
     */
    AbstractSamResponseParser(const std::shared_ptr<ApduResponse> response,
                              AbstractSamCommandBuilder<AbstractSamResponseParser>* builder);

    /**
     *
     */
    virtual ~AbstractSamResponseParser() = default;

    /**
     * {@inheritDoc}
     *
     * Return type should be
     *   AbstractSamCommandBuilder<AbstractSamResponseParser>*
     */
    AbstractApduCommandBuilder* getBuilder() const override;

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
    const std::map<int, std::shared_ptr<StatusProperties>>& getStatusTable() const override;

    /**
     * {@inheritDoc}
     */
    const KeypleSeCommandException buildCommandException(
        const std::type_info& exceptionClass,
        const std::string& message,
        const std::shared_ptr<SeCommand> commandRef,
        const int statusCode) const override;
};

}
}
}
}

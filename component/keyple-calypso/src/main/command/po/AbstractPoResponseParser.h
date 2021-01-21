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

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "AbstractPoResponseParser.h"
#include "KeypleCalypsoExport.h"

/* Core */
#include "AbstractApduResponseParser.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

class KEYPLECALYPSO_API AbstractPoResponseParser
: public AbstractApduResponseParser {
public:
    /**
     * The generic abstract constructor to build a parser of the APDU response.
     *
     * @param response response to parse
     * @param builder the reference of the builder that created the parser
     */
    AbstractPoResponseParser(std::shared_ptr<ApduResponse> response,
                             AbstractPoCommandBuilder<AbstractPoResponseParser>* builder);

    /**
     * Copy Constructor
     * 
     * Required by macOS/CLang
     */
    AbstractPoResponseParser(const AbstractPoResponseParser& arp) = default;
      
    /**
     *
     */
    virtual ~AbstractPoResponseParser() = default;

    /**
     *
     */
    virtual AbstractPoCommandBuilder<AbstractPoResponseParser>* getBuilder()const override;

    /**
     *
     */
    void checkStatus() const override;

protected:
    /**
     *
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

/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

/* Common */
#include "exceptionhelper.h"

/* Core */
#include "AbstractApduResponseParser_Import.h"

/* Calypso */
#include "AbstractPoResponseParser.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po;
using namespace keyple::core::seproxy::message;

/**
 * Decrease (0030) response parser. See specs: Calypso / page 83 /
 * 9.4.2 Decrease
 */
class DecreaseRespPars final : public AbstractPoResponseParser {
public:
    /**
     * Instantiates a new DecreaseRespPars.
     *
     * @param response the response from the PO
     */
    DecreaseRespPars(std::shared_ptr<ApduResponse> response);

    /**
     * Returns the new counter value as an int between 0
     *
     * @return the new value
     * @throws IllegalStateException - if the counter value is not available
     *         from the command response.
     */
    int getNewValue();

    /**
     *
     */
    std::string toString();

protected:
    /**
     *
     */
    std::unordered_map<int,
        std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
            getStatusTable() const override;

    /**
     *
     */
    std::shared_ptr<DecreaseRespPars> shared_from_this()
    {
        return std::static_pointer_cast<DecreaseRespPars>(
                   AbstractApduResponseParser::shared_from_this());
    }

private:
    /**
     *
     */
    static std::unordered_map<int,
        std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
            STATUS_TABLE;

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
    static DecreaseRespPars::StaticConstructor staticConstructor;



};

}
}
}
}
}

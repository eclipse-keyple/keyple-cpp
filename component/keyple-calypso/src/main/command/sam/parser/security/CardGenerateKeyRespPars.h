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
#include <memory>

#include "AbstractSamResponseParser.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::core::seproxy::message;

class CardGenerateKeyRespPars : public AbstractSamResponseParser {
public:
    /**
     * Instantiates a new CardGenerateKeyRespPars.
     *
     * @param response from the SAM
     */
    CardGenerateKeyRespPars(std::shared_ptr<ApduResponse> response);

    /**
     *
     */
    virtual ~CardGenerateKeyRespPars()
    {
    }

    /**
     * Gets the 32 bytes of ciphered data.
     *
     * @return the ciphered data byte array or null if the operation failed
     */
    virtual std::vector<uint8_t> getCipheredData() const;

protected:
    /**
     *
     */
    std::unordered_map<int, std::shared_ptr<StatusProperties>>
    getStatusTable() const override;

    /**
     *
     */
    std::shared_ptr<CardGenerateKeyRespPars> shared_from_this()
    {
        return std::static_pointer_cast<CardGenerateKeyRespPars>(
            AbstractSamResponseParser::shared_from_this());
    }

private:
    /**
     *
     */
    static std::unordered_map<int, std::shared_ptr<StatusProperties>>
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
    static CardGenerateKeyRespPars::StaticConstructor staticConstructor;
};

}
}
}
}
}
}

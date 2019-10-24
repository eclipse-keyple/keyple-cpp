/********************************************************************************
* Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
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

#include <unordered_map>
#include <memory>
#include <string>

#include "AbstractPoResponseParser.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po;
using namespace keyple::core::seproxy::message;

class ChangeKeyRespPars : public AbstractPoResponseParser {
private:
    /**
     *
     */
    static std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

    /**
     *
     */
    class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
    public:
        /**
         *
         */
        StaticConstructor();
    };

    /**
     *
     */
    static ChangeKeyRespPars::StaticConstructor staticConstructor;

public:
    /**
     * Instantiates a new ChangeKeyRespPars
     */
    ChangeKeyRespPars(std::shared_ptr<ApduResponse> response);

    /**
     *
     */
    virtual ~ChangeKeyRespPars() {}

protected:
    /**
     *
     */
    std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> getStatusTable() override;

protected:
    /**
     *
     */
    std::shared_ptr<ChangeKeyRespPars> shared_from_this()
    {
        return std::static_pointer_cast<ChangeKeyRespPars>(AbstractPoResponseParser::shared_from_this());
    }
};

}
}
}
}
}
}

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

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using AbstractPoResponseParser = org::eclipse::keyple::calypso::command::po::AbstractPoResponseParser;
using ApduResponse             = org::eclipse::keyple::core::seproxy::message::ApduResponse;

class ChangeKeyRespPars : public AbstractPoResponseParser {
private:
    static std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

private:
    class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
    public:
        StaticConstructor();
    };

private:
    static ChangeKeyRespPars::StaticConstructor staticConstructor;


    /**
        * Instantiates a new ChangeKeyRespPars
        */
public:
    ChangeKeyRespPars(std::shared_ptr<ApduResponse> response);

protected:
    std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> getStatusTable() override;

protected:
    std::shared_ptr<ChangeKeyRespPars> shared_from_this() {
        return std::static_pointer_cast<ChangeKeyRespPars>(AbstractPoResponseParser::shared_from_this());
    }
};

}
}
}
}
}
}
}
}

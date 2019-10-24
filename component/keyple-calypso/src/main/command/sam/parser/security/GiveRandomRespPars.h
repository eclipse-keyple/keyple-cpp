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

#include <memory>

#include "AbstractSamResponseParser.h"

/* Forwards declaration */
namespace keyple { namespace core { namespace seproxy { namespace message { class ApduResponse; } } } }

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {


using namespace keyple::calypso::command::sam;
using namespace keyple::core::seproxy::message;

/**
 * PO Give Random response parser.
 * <p>
 * No output data except status word
 */
class GiveRandomRespPars : public AbstractSamResponseParser {
public:
    /**
     * Instantiates a new GiveRandomRespPars.
     *
     * @param response the response
     */
    GiveRandomRespPars(std::shared_ptr<ApduResponse> response);

protected:
    /**
     *
     */
    std::shared_ptr<GiveRandomRespPars> shared_from_this()
    {
        return std::static_pointer_cast<GiveRandomRespPars>(AbstractSamResponseParser::shared_from_this());
    }
};

}
}
}
}
}
}

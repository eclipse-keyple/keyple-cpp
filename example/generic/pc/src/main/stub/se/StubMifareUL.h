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

#include <string>
#include <vector>
#include <memory>

/* Example */
#include "StubSecureElement.h"

namespace keyple {
namespace example {
namespace generic {
namespace pc {
namespace stub {
namespace se {

using namespace keyple::plugin::stub;

/**
 * Simple contact stub SE (no command)
 */
class StubMifareUL : public StubSecureElement {
public:
    /**
     *
     */
    static const std::string seProtocol;

    /**
     *
     */
    const std::vector<uint8_t> atr{0x3B, 0x8F, 0x80, 0x01, 0x80, 0x4F, 0x0C,
                                   0xA0, 0x00, 0x00, 0x03, 0x06, 0x03, 0x00,
                                   0x03, 0x00, 0x00, 0x00, 0x00, 0x68};

    /**
     *
     */
    StubMifareUL();

    /**
     *
     */
    virtual ~StubMifareUL()
    {
    }

    /**
     *
     */
    const std::vector<uint8_t>& getATR() override;

    /**
     *
     */
    std::string getSeProcotol() override;

protected:
    /**
     *
     */
    std::shared_ptr<StubMifareUL> shared_from_this()
    {
        return std::static_pointer_cast<StubMifareUL>(
            StubSecureElement::shared_from_this());
    }
};

}
}
}
}
}
}

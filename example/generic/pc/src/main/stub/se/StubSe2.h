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

/* Plugin  - Stub */
#include "StubSecureElement.h"

namespace keyple {
namespace example {
namespace generic {
namespace pc {
namespace stub {
namespace se {

using namespace keyple::plugin::stub;

/**
 * Simple contactless stub SE (no command)
 */
class StubSe2 : public StubSecureElement {
public:
    /**
     *
     */
    static const std::string seProtocol;

    /**
     *
     */
    const std::vector<uint8_t> atr = {0x3B, 0x8E, 0x80, 0x01, 0x80, 0x31, 0x80,
                                      0x66, 0x40, 0x90, 0x89, 0x12, 0x08, 0x02,
                                      0x83, 0x01, 0x90, 0x00, 0x0B};

    /**
     *
     */
    StubSe2();

    /**
     *
     */
    virtual ~StubSe2()
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
    std::shared_ptr<StubSe2> shared_from_this()
    {
        return std::static_pointer_cast<StubSe2>(
            StubSecureElement::shared_from_this());
    }
};

}
}
}
}
}
}

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

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

/* Plugin */
#include "StubSecureElement.h"

namespace keyple {
namespace example {
namespace calypso {
namespace pc {
namespace stub {
namespace se {

using namespace keyple::plugin::stub;
using namespace keyple::common;

/**
 * This class is an example of a Stub SAM
 */
class StubSamCalypsoClassic : public StubSecureElement {
public:
    /**
     *
     */
    static const std::string seProtocol;

    /**
     * serial number : 12345678
     */
    const std::vector<uint8_t> atr{
        0x3F, 0x96, 0x00, 0x80, 0x5A, 0x00, 0x80, 0xC1, 0x20, 0x00, 0x00, 0x12,
        0x34, 0x56, 0x78, 0x82, 0x90, 0x00};

    /**
     *
     */
    StubSamCalypsoClassic();

    /**
     *
     */
    virtual ~StubSamCalypsoClassic() {}

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
    std::shared_ptr<StubSamCalypsoClassic> shared_from_this()
    {
        return std::static_pointer_cast<StubSamCalypsoClassic>(
                   StubSecureElement::shared_from_this());
    }

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(StubSamCalypsoClassic));
};

}
}
}
}
}
}

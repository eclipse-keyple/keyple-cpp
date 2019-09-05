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
#include <vector>
#include <memory>

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

/* Plugin */
#include "StubSecureElement.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace example {
namespace calypso {
namespace pc {
namespace stub {
namespace se {

using namespace org::eclipse::keyple::plugin::stub;
using namespace org::eclipse::keyple::common;

/**
 * This class is an example of a Stub SAM
 */
class StubSamCalypsoClassic : public StubSecureElement {
private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(StubSamCalypsoClassic));

public:
    /**
     *
     */
    static const std::string seProtocol;

    /**
     *
     */
    const std::string ATR_HEX = "3B3F9600805A0080C120000012345678829000"; // serial number : 12345678

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
    std::vector<char> getATR() override;

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
        return std::static_pointer_cast<StubSamCalypsoClassic>(StubSecureElement::shared_from_this());
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

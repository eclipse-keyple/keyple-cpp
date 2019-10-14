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

/* Example */
#include "StubSecureElement.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace example {
namespace generic {
namespace pc {
namespace stub {
namespace se {


using StubSecureElement = org::eclipse::keyple::plugin::stub::StubSecureElement;

/**
 * Simple contact stub SE (no command)
 */
class StubMifareDesfire : public StubSecureElement {
public:
    /**
     *
     */
    static const std::string seProtocol;

    /**
     *
     */
    const std::string ATR_HEX = "3B8180018080";

    /**
     *
     */
    StubMifareDesfire();

    /**
     *
     */
    virtual ~StubMifareDesfire() {}

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
    std::shared_ptr<StubMifareDesfire> shared_from_this()
    {
        return std::static_pointer_cast<StubMifareDesfire>(StubSecureElement::shared_from_this());
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

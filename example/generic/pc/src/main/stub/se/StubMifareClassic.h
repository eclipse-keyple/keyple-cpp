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
class StubMifareClassic : public StubSecureElement {
public:
    /**
     *
     */
    static const std::string seProtocol;

    /**
     *
     */
    const std::string ATR_HEX = "3B8F8001804F0CA000000306030001000000006A";

    /**
     *
     */
    StubMifareClassic();

    /**
     *
     */
    virtual ~StubMifareClassic() {}

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
    std::shared_ptr<StubMifareClassic> shared_from_this()
    {
        return std::static_pointer_cast<StubMifareClassic>(StubSecureElement::shared_from_this());
    }
};

}
}
}
}
}
}

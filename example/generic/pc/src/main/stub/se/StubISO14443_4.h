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

#include "StubSecureElement.h"

namespace org {
namespace eclipse {
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
class StubISO14443_4 : public StubSecureElement {
public:
    /**
     *
     */
    static const std::string seProtocol;

    /**
     *
     */
    const std::string ATR_HEX = "3B8880010000000000718100F9";

    /**
     *
     */
    StubISO14443_4();

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
    std::shared_ptr<StubISO14443_4> shared_from_this() {
        return std::static_pointer_cast<StubISO14443_4>(org.eclipse.keyple.plugin.stub.StubSecureElement::shared_from_this());
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

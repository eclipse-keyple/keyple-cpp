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

namespace keyple {
namespace example {
namespace calypso {
namespace pc {
namespace stub {
namespace se {


using namespace keyple::plugin::stub;

/**
 * This class is an example of a Stub SAM
 */
class StubSamHoplink : public StubSecureElement {
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
    StubSamHoplink();

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
    std::shared_ptr<StubSamHoplink> shared_from_this()
    {
        return std::static_pointer_cast<StubSamHoplink>(org.eclipse.keyple.plugin.stub.StubSecureElement::shared_from_this());
    }
};

}
}
}
}
}
}

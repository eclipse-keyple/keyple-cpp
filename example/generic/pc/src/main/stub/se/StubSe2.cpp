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

#include "StubSe2.h"
#include "ByteArrayUtil.h"

namespace keyple {
namespace example {
namespace generic {
namespace pc {
namespace stub {
namespace se {

using namespace keyple::core::util;
using namespace keyple::plugin::stub;

const std::string StubSe2::seProtocol = "PROTOCOL_ISO14443_4";

StubSe2::StubSe2()
{
}

std::vector<char> StubSe2::getATR()
{
    return ByteArrayUtil::fromHex(ATR_HEX);
}


std::string StubSe2::getSeProcotol()
{
    return seProtocol;
}

}
}
}
}
}
}


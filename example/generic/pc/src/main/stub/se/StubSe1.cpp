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

#include "StubSe1.h"
#include "ByteArrayUtil.h"

namespace keyple {
namespace example {
namespace generic {
namespace pc {
namespace stub {
namespace se {

using namespace keyple::core::util;
using namespace keyple::plugin::stub;

const std::string StubSe1::seProtocol = "PROTOCOL_ISO7816_3";

StubSe1::StubSe1() 
{
}


std::vector<char> StubSe1::getATR()
{
    return ByteArrayUtil::fromHex(ATR_HEX);
}

std::string StubSe1::getSeProcotol()
{
    return seProtocol;
}

}
}
}
}
}
}


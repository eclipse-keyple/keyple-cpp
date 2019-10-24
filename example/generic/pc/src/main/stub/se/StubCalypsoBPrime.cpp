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

#include "StubCalypsoBPrime.h"
#include "ByteArrayUtil.h"

namespace keyple {
namespace example {
namespace generic {
namespace pc {
namespace stub {
namespace se {

using namespacekeyple::plugin::stub;
using namespace keyple::util;

const std::string StubCalypsoBPrime::seProtocol = "PROTOCOL_B_PRIME";

StubCalypsoBPrime::StubCalypsoBPrime()
{
    /* Get data */
    addHexCommand("FFCA 000000", "CA7195009000");
}

std::vector<char> StubCalypsoBPrime::getATR()
{
    return ByteArrayUtils::fromHex(ATR_HEX);
}

std::string StubCalypsoBPrime::getSeProcotol()
{
    return seProtocol;
}

}
}
}
}
}
}

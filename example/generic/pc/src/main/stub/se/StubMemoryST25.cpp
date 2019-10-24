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

#include "StubMemoryST25.h"
#include "ByteArrayUtil.h"

namespace keyple {
namespace example {
namespace generic {
namespace pc {
namespace stub {
namespace se {

using namespace keyple::plugin::stub;
using namespace keyple::util;

const std::string StubMemoryST25::seProtocol = "PROTOCOL_MEMORY_ST25";

StubMemoryST25::StubMemoryST25()
{
    /* Get data */
    addHexCommand("FFCA 000000", "8899AABBCCDDEEFF9000");
}

std::vector<char> StubMemoryST25::getATR()
{
    return ByteArrayUtils::fromHex(ATR_HEX);
}

std::string StubMemoryST25::getSeProcotol()
{
    return seProtocol;
}

}
}
}
}
}
}

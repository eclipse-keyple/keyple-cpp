/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "StubMemoryST25.h"

/* Core */
#include "ByteArrayUtil.h"

namespace keyple {
namespace example {
namespace commono {
namespace calypso {
namespace stub {

using namespace keyple::plugin::stub;
using namespace keyple::core::util;

const std::string StubMemoryST25::seProtocol = "PROTOCOL_MEMORY_ST25";

StubMemoryST25::StubMemoryST25()
{
    /* Get data */
    addHexCommand("FFCA 000000", "8899AABBCCDDEEFF9000");
}

const std::vector<uint8_t>& StubMemoryST25::getATR()
{
    return ByteArrayUtil::fromHex(ATR_HEX);
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

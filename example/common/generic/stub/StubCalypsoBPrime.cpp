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

#include "StubCalypsoBPrime.h"

/* Core */
#include "ByteArrayUtil.h"

namespace keyple {
namespace example {
namespace commono {
namespace calypso {
namespace stub {

using namespace keyple::plugin::stub;
using namespace keyple::core::util;

const std::string StubCalypsoBPrime::seProtocol = "PROTOCOL_B_PRIME";

StubCalypsoBPrime::StubCalypsoBPrime()
{
    /* Get data */
    addHexCommand("FFCA 000000", "CA7195009000");
}

const std::vector<uint8_t>& StubCalypsoBPrime::getATR()
{
    return ByteArrayUtil::fromHex(ATR_HEX);
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

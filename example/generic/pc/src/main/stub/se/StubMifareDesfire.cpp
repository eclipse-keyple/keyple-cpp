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

/* Example */
#include "StubMifareDesfire.h"

/* Core */
#include "ByteArrayUtil.h"

namespace keyple {
namespace example {
namespace generic {
namespace pc {
namespace stub {
namespace se {

using namespace keyple::plugin::stub;
using namespace keyple::core::util;

const std::string StubMifareDesfire::seProtocol = "PROTOCOL_MIFARE_DESFIRE";

StubMifareDesfire::StubMifareDesfire()
{
    /* Get data */
    addHexCommand("FFCA 000000", "223344556677889000");
}

std::vector<char> StubMifareDesfire::getATR()
{
    return ByteArrayUtil::fromHex(ATR_HEX);
}

std::string StubMifareDesfire::getSeProcotol()
{
    return seProtocol;
}

}
}
}
}
}
}

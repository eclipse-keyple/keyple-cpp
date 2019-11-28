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
#include "StubMifareClassic.h"

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

const std::string StubMifareClassic::seProtocol = "PROTOCOL_MIFARE_CLASSIC";

StubMifareClassic::StubMifareClassic()
{
    /* Get data */
    addHexCommand("FFCA 000000", "112233449000");
}

const std::vector<uint8_t>& StubMifareClassic::getATR()
{
    return atr;
}

std::string StubMifareClassic::getSeProcotol()
{
    return seProtocol;
}

}
}
}
}
}
}

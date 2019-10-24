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

#include "StubISO14443_4.h"
#include "ByteArrayUtil.h"

namespace keyple {
namespace example {
namespace generic {
namespace pc {
namespace stub {
namespace se {

using namespace keyple::plugin::stub;
using namespace keyple::util;

const std::string StubISO14443_4::seProtocol = "PROTOCOL_ISO14443_4";

StubISO14443_4::StubISO14443_4()
{
    /* Select fake Application */
    addHexCommand("00A4 0400 05AABBCCDDEE00", "6A82");

    /* Select Navigo Application */
    addHexCommand("00A4 0400 0AA000000404012509010100", "6F24 840AA0000004040125090101A516BF0C13C708 1122334455667788 53070A3C2312141001 9000");

    /* Select Hoplink Application */
    addHexCommand("00A4 0400 0AA000000291A00000019100", "6F25840BA000000291A00000019102A516BF0C13C708 1122334455667788 53070A3C230C141001 9000");
    addHexCommand("00A4 0400 0BA000000291A0000001910200", "6F25840BA000000291A00000019102A516BF0C13C708 1122334455667788 53070A3C230C141001 9000");

    /* Read Record */
    addHexCommand("00B2 01A4 20", "0000000000000000000000000000000000000000000000000000000000000000 9000");
    addHexCommand("00B2 01D4 30", "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 9000");
    addHexCommand("00B2 01A4 01", "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 9000");

    /* Open Secure Session V3.1 */
    addHexCommand("008A 0BD104 11223344 00", "55667788 00300E30000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 9000");
    addHexCommand("008A 0BD104 11223344", "55667788 00300E30000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 9000");

    /* Close Secure Session */
    addHexCommand("008E 0000 04 88776655 00", "44332211 9000");
    addHexCommand("008E 8000 04 88776655 00", "44332211 9000");
    addHexCommand("008E 0000 04 88776655", "44332211 9000");
    addHexCommand("008E 8000 04 88776655", "44332211 9000");
}

std::vector<char> StubISO14443_4::getATR()
{
    return ByteArrayUtils::fromHex(ATR_HEX);
}

std::string StubISO14443_4::getSeProcotol()
{
    return seProtocol;
}

}
}
}
}
}
}

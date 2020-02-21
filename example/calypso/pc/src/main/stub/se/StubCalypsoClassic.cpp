/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include "StubCalypsoClassic.h"
#include "ByteArrayUtil.h"

namespace keyple {
namespace example {
namespace calypso {
namespace pc {
namespace stub {
namespace se {

using namespace keyple::plugin::stub;
using namespace keyple::core::util;

const std::string StubCalypsoClassic::seProtocol = "PROTOCOL_ISO14443_4";

StubCalypsoClassic::StubCalypsoClassic()
{
    logger->debug("StubCalypsoClassic - adding c-apdu/r-apdu combinations to "
                  "hexCommands map\n");

    /* Select Application */
    addHexCommand("00A4 0400 05 AABBCCDDEE 00", "6A82");
    /* Select Application */
    addHexCommand("00A4 0400 0A A0000004040125090101 00",
                  "6F24840AA0000004040125090101A516BF0C13C708 "
                  "0000000011223344 53070A3C23121410019000");
    /* Read Records */
    addHexCommand("00B2014400",
                  "00112233445566778899AABBCCDDEEFF00112233445566778899AABBCC"
                  "9000");
    /* Read Records - EnvironmentAndHolder (SFI=07)) */
    addHexCommand("00B2013C00",
                  "24B92848080000131A5000120000000000000000000000000000000000"
                  "9000");
    /* Read Records - EventLog (SFI=08, recnbr=1)) */
    addHexCommand("00B2014400",
                  "00112233445566778899AABBCCDDEEFF00112233445566778899AABBCC"
                  "9000");
    /* Open Secure Session V3.1 */
    addHexCommand("008A030104C1C2C3C400",
                  "0308306C00307E1D24B928480800000606F00012000000000000000000"
                  "00000000000000009000");
    /* Open Secure Session V3.1 */
    addHexCommand("008A0B3904C1C2C3C400",
                  "0308306C00307E1D24B928480800000606F00012000000000000000000"
                  "00000000000000009000");
    /* Read Records */
    addHexCommand("00B2014400",
                  "00112233445566778899AABBCCDDEEFF00112233445566778899AABBCC"
                  "9000");
    /* Read Records */
    addHexCommand("00B201F400",
                  "0000000000000000000000000000000000000000000000000000000000"
                  "9000");
    /* Read Records */
    addHexCommand("00B2014C00",
                  "00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
                  "9000");
    /* Read Records */
    addHexCommand("00B2014D00",
                  "011D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
                  "FFFF021D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
                  "FFFFFFFF031D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
                  "FFFFFFFFFFFF041D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
                  "FFFFFFFFFFFFFFFF9000");
    /* Append Record */
    addHexCommand("00E200401D00112233445566778899AABBCCDDEEFF0011223344556677"
                  "8899AABBCC",
                  "9000");
    /* Close Secure Session */
    /* no ratification asked */
    addHexCommand("008E0000040506070800", "010203049000");
    /* ratification asked */
    addHexCommand("008E8000040506070800", "010203049000");
    /* Ratification */
    addHexCommand("00B2000000", "6B00");

    logger->debug("hexCommands size: %d\n", hexCommands.size());
}

const std::vector<uint8_t>& StubCalypsoClassic::getATR()
{
    return atr;
}

std::string StubCalypsoClassic::getSeProcotol()
{
    return seProtocol;
}

}
}
}
}
}
}

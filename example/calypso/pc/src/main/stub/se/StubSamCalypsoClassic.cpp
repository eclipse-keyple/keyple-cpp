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

#include "StubSamCalypsoClassic.h"
#include "ByteArrayUtil.h"

namespace keyple {
namespace example {
namespace calypso {
namespace pc {
namespace stub {
namespace se {

using namespace keyple::plugin::stub;
using namespace keyple::core::util;

const std::string StubSamCalypsoClassic::seProtocol = "PROTOCOL_ISO7816_3";

StubSamCalypsoClassic::StubSamCalypsoClassic()
{
    /* Select Diversifier */
    addHexCommand("8014 0000 08 0000000011223344", "9000");
    /* Get Challenge */
    addHexCommand("8084000004", "C1C2C3C49000");
    /* Digest Init */
    addHexCommand("808A00FF27307E0308306C00307E1D24B928480800000606F000120000"
                  "000000000000000000000000000000",
                  "9000");
    /* Digest Update */
    addHexCommand("808C00000500B2014400", "9000");
    /* Digest Update */
    addHexCommand("808C00001F00112233445566778899AABBCCDDEEFF0011223344556677"
                  "8899AABBCC9000",
                  "9000");
    /* Digest Update */
    addHexCommand("808C00000500B201F400", "9000");
    /* Digest Update */
    addHexCommand("808C00001F000000000000000000000000000000000000000000000000"
                  "00000000009000",
                  "9000");
    /* Digest Update */
    addHexCommand("808C00000500B2014C00", "9000");
    /* Digest Update */
    addHexCommand("808C00001F00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
                  "FFFFFFFFFF9000",
                  "9000");
    /* Digest Update */
    addHexCommand("808C00002200E200401D00112233445566778899AABBCCDDEEFF001122"
                  "33445566778899AABBCC",
                  "9000");
    /* Digest Update */
    addHexCommand("808C0000029000", "9000");
    /* Digest Update */
    addHexCommand("808C00000500B2014D00", "9000");
    /* Digest Update */
    addHexCommand("808C00007E011D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
                  "FFFFFFFFFFFFFF021D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
                  "FFFFFFFFFFFFFFFFFF031D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
                  "FFFFFFFFFFFFFFFFFFFFFF041D00000FFFFFFFFFFFFFFFFFFFFFFFFFFF"
                  "FFFFFFFFFFFFFFFFFFFFFFFFFF9000",
                  "9000");
    /* Digest Close */
    addHexCommand("808E000004", "050607089000");
    /* Digest Authenticate */
    addHexCommand("808200000401020304", "9000");
}

const std::vector<uint8_t>& StubSamCalypsoClassic::getATR()
{
    return atr;
}

std::string StubSamCalypsoClassic::getSeProcotol()
{
    return seProtocol;
}

}
}
}
}
}
}

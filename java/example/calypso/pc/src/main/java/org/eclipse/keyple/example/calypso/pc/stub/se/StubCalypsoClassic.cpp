#include "StubCalypsoClassic.h"
#include "../../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace calypso {
                    namespace pc {
                        namespace stub {
                            namespace se {
                                using StubSecureElement = org::eclipse::keyple::plugin::stub::StubSecureElement;
                                using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
const std::string StubCalypsoClassic::seProtocol = "PROTOCOL_ISO14443_4";

                                StubCalypsoClassic::StubCalypsoClassic() {
                                    /* Select Application */
                                    addHexCommand("00A4 0400 05 AABBCCDDEE 00", "6A82");
                                    /* Select Application */
                                    addHexCommand("00A4 0400 0A A0000004040125090101 00", "6F24840AA0000004040125090101A516BF0C13C708 0000000011223344 53070A3C23121410019000");
                                    /* Read Records */
                                    addHexCommand("00B2014400", "00112233445566778899AABBCCDDEEFF00112233445566778899AABBCC9000");
                                    /* Read Records - EnvironmentAndHolder (SFI=07)) */
                                    addHexCommand("00B2013C00", "24B92848080000131A50001200000000000000000000000000000000009000");
                                    /* Read Records - EventLog (SFI=08, recnbr=1)) */
                                    addHexCommand("00B2014400", "00112233445566778899AABBCCDDEEFF00112233445566778899AABBCC9000");
                                    /* Open Secure Session V3.1 */
                                    addHexCommand("008A030104C1C2C3C400", "0308306C00307E1D24B928480800000606F0001200000000000000000000000000000000009000");
                                    /* Open Secure Session V3.1 */
                                    addHexCommand("008A0B3904C1C2C3C400", "0308306C00307E1D24B928480800000606F0001200000000000000000000000000000000009000");
                                    /* Read Records */
                                    addHexCommand("00B2014400", "00112233445566778899AABBCCDDEEFF00112233445566778899AABBCC9000");
                                    /* Read Records */
                                    addHexCommand("00B201F400", "00000000000000000000000000000000000000000000000000000000009000");
                                    /* Read Records */
                                    addHexCommand("00B2014C00", "00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9000");
                                    /* Read Records */
                                    addHexCommand("00B2014D00", "011D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF021D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF031D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF041D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9000");
                                    /* Append Record */
                                    addHexCommand("00E200401D00112233445566778899AABBCCDDEEFF00112233445566778899AABBCC", "9000");
                                    /* Close Secure Session */
                                    /* no ratification asked */
                                    addHexCommand("008E0000040506070800", "010203049000");
                                    /* ratification asked */
                                    addHexCommand("008E8000040506070800", "010203049000");
                                    /* Ratification */
                                    addHexCommand("00B2000000", "6B00");
                                }

                                std::vector<char> StubCalypsoClassic::getATR() {
                                    return ByteArrayUtils::fromHex(ATR_HEX);
                                }

                                std::string StubCalypsoClassic::getSeProcotol() {
                                    return seProtocol;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

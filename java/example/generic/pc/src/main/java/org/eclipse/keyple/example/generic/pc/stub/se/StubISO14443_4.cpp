#include "StubISO14443_4.h"
#include "../../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace pc {
                        namespace stub {
                            namespace se {
                                using StubSecureElement = org::eclipse::keyple::plugin::stub::StubSecureElement;
                                using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
const std::string StubISO14443_4::seProtocol = "PROTOCOL_ISO14443_4";

                                StubISO14443_4::StubISO14443_4() {
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

                                std::vector<char> StubISO14443_4::getATR() {
                                    return ByteArrayUtils::fromHex(ATR_HEX);
                                }

                                std::string StubISO14443_4::getSeProcotol() {
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

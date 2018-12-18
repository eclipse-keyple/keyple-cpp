#include "StubSamCalypsoClassic.h"
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
const std::string StubSamCalypsoClassic::seProtocol = "PROTOCOL_ISO7816_3";

                                StubSamCalypsoClassic::StubSamCalypsoClassic() {
                                    /* Select Diversifier */
                                    addHexCommand("8014 0000 08 0000000011223344", "9000");
                                    /* Get Challenge */
                                    addHexCommand("0084000004", "C1C2C3C49000");
                                    /* Digest Init */
                                    addHexCommand("808A00FF27307E0308306C00307E1D24B928480800000606F000120000000000000000000000000000000000", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00000500B2014400", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00001F00112233445566778899AABBCCDDEEFF00112233445566778899AABBCC9000", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00000500B201F400", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00001F00000000000000000000000000000000000000000000000000000000009000", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00000500B2014C00", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00001F00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9000", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00002200E200401D00112233445566778899AABBCCDDEEFF00112233445566778899AABBCC", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C0000029000", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00000500B2014D00", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00007E011D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF021D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF031D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF041D00000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9000", "9000");
                                    /* Digest Close */
                                    addHexCommand("808E000004", "050607089000");
                                    /* Digest Authenticate */
                                    addHexCommand("808200000401020304", "9000");
                                }

                                std::vector<char> StubSamCalypsoClassic::getATR() {
                                    return ByteArrayUtils::fromHex(ATR_HEX);
                                }

                                std::string StubSamCalypsoClassic::getSeProcotol() {
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

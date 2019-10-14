#include "StubSamHoplink.h"
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
const std::string StubSamHoplink::seProtocol = "PROTOCOL_ISO7816_3";

                                StubSamHoplink::StubSamHoplink() {
                                    /* Select Diversifier */
                                    addHexCommand("8014 0000 08 0000000011223344", "9000");
                                    /* Get Challenge */
                                    addHexCommand("0084000004", "C1C2C3C49000");
                                    /* Digest Init */
                                    addHexCommand("808A00FF3A300E03082ED700300E300102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F30", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00000500B201A420", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00002200000000000000000000000000000000000000000000000000000000000000009000", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00000500B201D430", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C0000320102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F309000", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00000500B201A420", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00002200000000000000000000000000000000000000000000000000000000000000009000", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C00000500B201D430", "9000");
                                    /* Digest Update */
                                    addHexCommand("808C0000320102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F309000", "9000");
                                    /* Digest Close */
                                    addHexCommand("808E000004", "050607089000");
                                    /* Digest Authenticate */
                                    addHexCommand("808200000401020304", "9000");
                                }

                                std::vector<char> StubSamHoplink::getATR() {
                                    return ByteArrayUtils::fromHex(ATR_HEX);
                                }

                                std::string StubSamHoplink::getSeProcotol() {
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

#include "StubMemoryST25.h"
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
const std::string StubMemoryST25::seProtocol = "PROTOCOL_MEMORY_ST25";

                                StubMemoryST25::StubMemoryST25() {
                                    /* Get data */
                                    addHexCommand("FFCA 000000", "8899AABBCCDDEEFF9000");
                                }

                                std::vector<char> StubMemoryST25::getATR() {
                                    return ByteArrayUtils::fromHex(ATR_HEX);
                                }

                                std::string StubMemoryST25::getSeProcotol() {
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

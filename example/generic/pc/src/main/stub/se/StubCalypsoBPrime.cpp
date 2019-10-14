#include "StubCalypsoBPrime.h"
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
const std::string StubCalypsoBPrime::seProtocol = "PROTOCOL_B_PRIME";

                                StubCalypsoBPrime::StubCalypsoBPrime() {
                                    /* Get data */
                                    addHexCommand("FFCA 000000", "CA7195009000");
                                }

                                std::vector<char> StubCalypsoBPrime::getATR() {
                                    return ByteArrayUtils::fromHex(ATR_HEX);
                                }

                                std::string StubCalypsoBPrime::getSeProcotol() {
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

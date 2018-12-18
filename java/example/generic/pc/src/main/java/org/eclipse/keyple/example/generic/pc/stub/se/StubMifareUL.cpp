#include "StubMifareUL.h"
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
const std::string StubMifareUL::seProtocol = "PROTOCOL_MIFARE_UL";

                                StubMifareUL::StubMifareUL() {
                                    /* Get data */
                                    addHexCommand("FFCA 000000", "223344556677889000");
                                }

                                std::vector<char> StubMifareUL::getATR() {
                                    return ByteArrayUtils::fromHex(ATR_HEX);
                                }

                                std::string StubMifareUL::getSeProcotol() {
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

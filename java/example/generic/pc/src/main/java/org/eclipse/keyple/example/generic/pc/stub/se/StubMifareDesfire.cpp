/* Example */
#include "StubMifareDesfire.h"

/* Core */
#include "ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic {
                    namespace pc {
                        namespace stub {
                            namespace se {

                                using StubSecureElement = org::eclipse::keyple::plugin::stub::StubSecureElement;
                                using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                                const std::string StubMifareDesfire::seProtocol = "PROTOCOL_MIFARE_DESFIRE";

                                StubMifareDesfire::StubMifareDesfire() {
                                    /* Get data */
                                    addHexCommand("FFCA 000000", "223344556677889000");
                                }

                                std::vector<char> StubMifareDesfire::getATR() {
                                    return ByteArrayUtils::fromHex(ATR_HEX);
                                }

                                std::string StubMifareDesfire::getSeProcotol() {
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

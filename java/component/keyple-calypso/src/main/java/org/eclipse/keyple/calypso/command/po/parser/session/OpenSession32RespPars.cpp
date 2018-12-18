#include "OpenSession32RespPars.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {
                            namespace session {
                                using PoRevision = org::eclipse::keyple::calypso::command::po::PoRevision;
                                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;

                                OpenSession32RespPars::OpenSession32RespPars(std::shared_ptr<ApduResponse> response) : AbstractOpenSessionRespPars(response, PoRevision::REV3_2) {
                                }

                                std::shared_ptr<SecureSession> OpenSession32RespPars::toSecureSession(std::vector<char> &apduResponseData) {
                                    return createSecureSession(apduResponseData);
                                }

                                std::shared_ptr<SecureSession> OpenSession32RespPars::createSecureSession(std::vector<char> &apduResponse) {

                                    char flag = apduResponse[8];
                                    // ratification: if the bit 0 of flag is set then the previous session has been ratified
                                    bool previousSessionRatified = (flag & (1 << 0)) == static_cast<char>(0x00);
                                    // secure session: if the bit 1 of flag is set then the secure session is authorized
                                    bool manageSecureSessionAuthorized = (flag & (1 << 1)) == static_cast<char>(0x02);

                                    char kif = apduResponse[9];
                                    char kvc = apduResponse[10];
                                    int dataLength = apduResponse[11];
                                    std::vector<char> data = Arrays::copyOfRange(apduResponse, 12, 12 + dataLength);

                                    return std::make_shared<SecureSession>(Arrays::copyOfRange(apduResponse, 0, 3), Arrays::copyOfRange(apduResponse, 3, 8), previousSessionRatified, manageSecureSessionAuthorized, kif, kvc, data, apduResponse);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

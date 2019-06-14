#include "OpenSession31RespPars.h"
#include "ApduResponse.h"
#include "AbstractOpenSessionRespPars.h"

#include "Arrays.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {
                            namespace security {
                                using PoRevision = org::eclipse::keyple::calypso::command::po::PoRevision;
                                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
                                using AbstractOpenSessionRespPars = org::eclipse::keyple::calypso::command::po::parser::security::AbstractOpenSessionRespPars;

                                OpenSession31RespPars::OpenSession31RespPars(std::shared_ptr<ApduResponse> response) : AbstractOpenSessionRespPars(response, PoRevision::REV3_1) {
                                }

                                std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> OpenSession31RespPars::toSecureSession(std::vector<char> &apduResponseData) {
                                    std::shared_ptr<SecureSession> secureSession;
                                    bool previousSessionRatified = (apduResponseData[4] == static_cast<char>(0x00));
                                    bool manageSecureSessionAuthorized = false;

                                    char kif = apduResponseData[5];
                                    char kvc = apduResponseData[6];
                                    int dataLength = apduResponseData[7];
                                    std::vector<char> data = Arrays::copyOfRange(apduResponseData, 8, 8 + dataLength);

                                    return std::make_shared<SecureSession>(Arrays::copyOfRange(apduResponseData, 0, 3), Arrays::copyOfRange(apduResponseData, 3, 4), previousSessionRatified, manageSecureSessionAuthorized, kif, kvc, data, apduResponseData);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

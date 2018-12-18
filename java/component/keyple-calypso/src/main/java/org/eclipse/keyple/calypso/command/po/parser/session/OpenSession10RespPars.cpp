#include "OpenSession10RespPars.h"
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

                                OpenSession10RespPars::OpenSession10RespPars(std::shared_ptr<ApduResponse> response) : AbstractOpenSessionRespPars(response, PoRevision::REV1_0) {
                                }

                                std::shared_ptr<SecureSession> OpenSession10RespPars::toSecureSession(std::vector<char> &apduResponseData) {
                                    return createSecureSession(apduResponseData);
                                }

                                std::shared_ptr<SecureSession> OpenSession10RespPars::createSecureSession(std::vector<char> &apduResponseData) {
                                    bool previousSessionRatified = true;

                                    /**
                                     * In rev 1.0 mode, the response to the Open Secure Session command is as follows:
                                     * <p>
                                     * <code>CC CC CC CC [RR RR] [NN..NN]</code>
                                     * <p>
                                     * Where:
                                     * <ul>
                                     * <li><code>CC CC CC CC</code> = PO challenge</li>
                                     * <li><code>RR RR</code> = ratification bytes (may be absent)</li>
                                     * <li><code>NN..NN</code> = record data (29 bytes)</li>
                                     * </ul>
                                     * Legal length values are:
                                     * <ul>
                                     * <li>4: ratified, 4-byte challenge, no data</li>
                                     * <li>33: ratified, 4-byte challenge, 29 bytes of data</li>
                                     * <li>6: not ratified (2 ratification bytes), 4-byte challenge, no data</li>
                                     * <li>35 not ratified (2 ratification bytes), 4-byte challenge, 29 bytes of data</li>
                                     * </ul>
                                     */
                                    switch (apduResponseData.size()) {
                                        case 4:
                                        case 33:
                                            previousSessionRatified = true;
                                            break;
                                        case 6:
                                        case 35:
                                            previousSessionRatified = false;
                                            break;
                                        default:
                                            throw std::make_shared<IllegalStateException>("Bad response length to Open Secure Session: " + std::to_string(apduResponseData.size()));
                                    }

                                    /* KVC doesn't exist and is set to null for this type of PO */
                                    return std::make_shared<SecureSession>(Arrays::copyOfRange(apduResponseData, 1, 4), Arrays::copyOfRange(apduResponseData, 4, 5), previousSessionRatified, false, nullptr, nullptr, apduResponseData);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

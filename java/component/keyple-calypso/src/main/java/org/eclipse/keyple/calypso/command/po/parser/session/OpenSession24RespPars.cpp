#include "OpenSession24RespPars.h"
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
                            namespace session {
                                using PoRevision = org::eclipse::keyple::calypso::command::po::PoRevision;
                                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
                                using AbstractOpenSessionRespPars = org::eclipse::keyple::calypso::command::po::parser::session::AbstractOpenSessionRespPars;

                                OpenSession24RespPars::OpenSession24RespPars(std::shared_ptr<ApduResponse> response) : AbstractOpenSessionRespPars(response, PoRevision::REV2_4) {
                                }

                                std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> OpenSession24RespPars::toSecureSession(std::vector<char> &apduResponseData) {
                                    return createSecureSession(apduResponseData);
                                }

                                std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> OpenSession24RespPars::createSecureSession(std::vector<char> &apduResponseData) {
                                    bool previousSessionRatified;

                                    /**
                                     * In rev 2.4 mode, the response to the Open Secure Session command is as follows:
                                     * <p>
                                     * <code>KK CC CC CC CC [RR RR] [NN..NN]</code>
                                     * <p>
                                     * Where:
                                     * <ul>
                                     * <li><code>KK</code> = KVC byte CC</li>
                                     * <li><code>CC CC CC CC</code> = PO challenge</li>
                                     * <li><code>RR RR</code> = ratification bytes (may be absent)</li>
                                     * <li><code>NN..NN</code> = record data (29 bytes)</li>
                                     * </ul>
                                     * Legal length values are:
                                     * <ul>
                                     * <li>5: ratified, 1-byte KCV, 4-byte challenge, no data</li>
                                     * <li>34: ratified, 1-byte KCV, 4-byte challenge, 29 bytes of data</li>
                                     * <li>7: not ratified (2 ratification bytes), 1-byte KCV, 4-byte challenge, no data</li>
                                     * <li>35 not ratified (2 ratification bytes), 1-byte KCV, 4-byte challenge, 29 bytes of
                                     * data</li>
                                     * </ul>
                                     */

                                    std::vector<char> data;

                                    switch (apduResponseData.size()) {
                                        case 5:
                                            previousSessionRatified = true;
                                            break;
                                        case 34:
                                            previousSessionRatified = true;
                                            data = Arrays::copyOfRange(apduResponseData, 5, 34);
                                            break;
                                        case 7:
                                            previousSessionRatified = false;
                                            break;
                                        case 36:
                                            previousSessionRatified = false;
                                            data = Arrays::copyOfRange(apduResponseData, 7, 36);
                                            break;
                                        default:
                                            throw std::make_shared<IllegalStateException>("Bad response length to Open Secure Session: " + std::to_string(apduResponseData.size()));
                                    }

                                    char kvc = apduResponseData[0];

                                    return std::make_shared<SecureSession>(Arrays::copyOfRange(apduResponseData, 1, 4), Arrays::copyOfRange(apduResponseData, 4, 5), previousSessionRatified, false, kvc, data, apduResponseData);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

#include "KeypleCalypsoSecureSessionException.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    namespace exception {
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                        using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;

                        KeypleCalypsoSecureSessionException::KeypleCalypsoSecureSessionException(const std::string &message, Type type, std::vector<std::shared_ptr<ApduRequest>> &requests, std::vector<std::shared_ptr<ApduResponse>> &responses) : org::eclipse::keyple::seproxy::exception::KeypleReaderException(message), type(type), requests(requests), responses(responses) {
                        }

                        KeypleCalypsoSecureSessionException::KeypleCalypsoSecureSessionException(const std::string &message, std::shared_ptr<ApduRequest> req, std::shared_ptr<ApduResponse> resp) : KeypleCalypsoSecureSessionException(message, nullptr, Collections::singletonList(req), Collections::singletonList(resp)) {
                        }

                        KeypleCalypsoSecureSessionException::KeypleCalypsoSecureSessionException(const std::string &message, std::shared_ptr<ApduResponse> resp) : KeypleCalypsoSecureSessionException(message, nullptr, resp) {
                        }

                        KeypleCalypsoSecureSessionException::Type KeypleCalypsoSecureSessionException::getType() {
                            return type;
                        }

                        std::vector<std::shared_ptr<ApduRequest>> KeypleCalypsoSecureSessionException::getRequests() {
                            return requests;
                        }

                        std::vector<std::shared_ptr<ApduResponse>> KeypleCalypsoSecureSessionException::getResponses() {
                            return responses;
                        }
                    }
                }
            }
        }
    }
}

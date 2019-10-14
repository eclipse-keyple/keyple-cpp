#include "KeypleDto.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        namespace model {

                            KeypleDto::KeypleDto(const std::string &action, const std::string &body, Boolean isRequest) : action(action), body(body), isRequest(isRequest) {
                            }

                            KeypleDto::KeypleDto(const std::string &action, const std::string &body, Boolean isRequest, const std::string &sessionId) : action(action), body(body), isRequest(isRequest) {
                                this->sessionId = sessionId;
                            }

                            KeypleDto::KeypleDto(const std::string &action, const std::string &body, Boolean isRequest, const std::string &sessionId, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &clientNodeId) : action(action), body(body), isRequest(isRequest) {
                                this->sessionId = sessionId;
                                this->nativeReaderName = nativeReaderName;
                                this->virtualReaderName = virtualReaderName;
                                this->clientNodeId = clientNodeId;
                            }

                            Boolean KeypleDto::isRequest() {
                                return isRequest_Renamed;
                            }

                            std::string KeypleDto::getAction() {
                                return action;
                            }

                            std::string KeypleDto::getBody() {
                                return body;
                            }

                            std::string KeypleDto::getSessionId() {
                                return sessionId;
                            }

                            std::string KeypleDto::getNodeId() {
                                return clientNodeId;
                            }

                            std::string KeypleDto::getNativeReaderName() {
                                return nativeReaderName;
                            }

                            std::string KeypleDto::getVirtualReaderName() {
                                return virtualReaderName;
                            }

                            std::string KeypleDto::toString() {
                                return std::string::format("KeypleDto : %s - isRequest : %s - native : %s - virtual : %s - clientNodeId : %s - sessionId : %s - body : %s", this->getAction(), this->isRequest(), this->getNativeReaderName(), this->getVirtualReaderName(), this->getNodeId(), this->getSessionId(), this->getBody());
                            }
                        }
                    }
                }
            }
        }
    }
}

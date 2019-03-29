#include "VirtualReaderSessionImpl.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> VirtualReaderSessionImpl::logger = org::slf4j::LoggerFactory::getLogger(VirtualReaderSessionImpl::typeid);

                        VirtualReaderSessionImpl::VirtualReaderSessionImpl(const std::string &sessionId, const std::string &slaveNodeId) : sessionId(sessionId), slaveNodeId(slaveNodeId) {
                            if (sessionId == "") {
                                throw std::invalid_argument("SessionId must not be null");
                            }
                            if (slaveNodeId == "") {
                                throw std::invalid_argument("SlaveNodeId must not be null");
                            }
                        }

                        std::string VirtualReaderSessionImpl::getSessionId() {
                            return sessionId;
                        }

                        std::string VirtualReaderSessionImpl::getSlaveNodeId() {
                            return slaveNodeId;
                        }

                        std::string VirtualReaderSessionImpl::toString() {
                            return "sessionId:" + sessionId + " - slaveNodeId:" + slaveNodeId;
                        }
                    }
                }
            }
        }
    }
}

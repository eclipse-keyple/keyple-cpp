#include "VirtualReaderSessionImpl.h"
#include "../transport/KeypleDto.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                        using Observable = org::eclipse::keyple::util::Observable;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> VirtualReaderSessionImpl::logger = org::slf4j::LoggerFactory::getLogger(VirtualReaderSessionImpl::typeid);

                        VirtualReaderSessionImpl::VirtualReaderSessionImpl(const std::string &sessionId) : sessionId(sessionId) {
                        }

                        std::string VirtualReaderSessionImpl::getSessionId() {
                            return sessionId;
                        }
                    }
                }
            }
        }
    }
}

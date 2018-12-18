#include "VirtualReaderSessionFactory.h"
#include "VirtualReaderSession.h"
#include "VirtualReaderSessionImpl.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {

                        std::shared_ptr<VirtualReaderSession> VirtualReaderSessionFactory::createSession(const std::string &nativeReaderName, const std::string &nodeId) {
                            return std::make_shared<VirtualReaderSessionImpl>(generateSessionId(nativeReaderName, nodeId));
                        }

                        std::string VirtualReaderSessionFactory::generateSessionId(const std::string &nativeReaderName, const std::string &nodeId) {
                            return nativeReaderName + nodeId + std::string::valueOf(System::currentTimeMillis());
                        }
                    }
                }
            }
        }
    }
}

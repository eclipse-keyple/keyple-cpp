#include "KeypleRemoteException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace exception {
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;

                        KeypleRemoteException::KeypleRemoteException(const std::string &message, std::runtime_error cause) : org::eclipse::keyple::seproxy::exception::KeypleBaseException(message, cause) {
                        }

                        KeypleRemoteException::KeypleRemoteException(const std::string &message) : org::eclipse::keyple::seproxy::exception::KeypleBaseException(message) {
                        }
                    }
                }
            }
        }
    }
}

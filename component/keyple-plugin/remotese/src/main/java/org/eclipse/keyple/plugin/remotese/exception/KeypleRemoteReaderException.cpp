#include "KeypleRemoteReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace exception {
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;

                        KeypleRemoteReaderException::KeypleRemoteReaderException(const std::string &message, std::runtime_error cause) : org::eclipse::keyple::seproxy::exception::KeypleReaderException(message, cause) {
                        }

                        KeypleRemoteReaderException::KeypleRemoteReaderException(const std::string &message) : org::eclipse::keyple::seproxy::exception::KeypleReaderException(message) {
                        }
                    }
                }
            }
        }
    }
}

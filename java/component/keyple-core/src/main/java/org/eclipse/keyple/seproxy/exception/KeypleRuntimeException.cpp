#include "KeypleRuntimeException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace exception {

                    KeypleRuntimeException::KeypleRuntimeException(const std::string &message) : RuntimeException(message) {
                    }

                    KeypleRuntimeException::KeypleRuntimeException(const std::string &message, std::runtime_error cause) : RuntimeException(message, cause) {
                    }
                }
            }
        }
    }
}

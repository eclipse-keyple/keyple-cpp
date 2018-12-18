#include "KeypleCalypsoSecureSessionUnauthorizedKvcException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    namespace exception {
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;

                        KeypleCalypsoSecureSessionUnauthorizedKvcException::KeypleCalypsoSecureSessionUnauthorizedKvcException(const std::string &message) : org::eclipse::keyple::seproxy::exception::KeypleReaderException(message) {
                        }
                    }
                }
            }
        }
    }
}

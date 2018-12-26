#include "KeypleChannelStateException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace exception {
                    KeypleChannelStateException::KeypleChannelStateException(const std::string &message) : KeypleReaderException(message) {
                    }

                    KeypleChannelStateException::KeypleChannelStateException(const std::string &message, std::runtime_error cause) : KeypleReaderException(message, cause) {
                    }
                }
            }
        }
    }
}

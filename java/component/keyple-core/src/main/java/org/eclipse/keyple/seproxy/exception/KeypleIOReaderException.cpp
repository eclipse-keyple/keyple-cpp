#include "KeypleIOReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace exception {
                    using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;

                    KeypleIOReaderException::KeypleIOReaderException(const std::string &message) : KeypleReaderException(message) {
                    }

                    KeypleIOReaderException::KeypleIOReaderException(const std::string &message, std::runtime_error cause) : KeypleReaderException(message, cause) {
                    }
                }
            }
        }
    }
}

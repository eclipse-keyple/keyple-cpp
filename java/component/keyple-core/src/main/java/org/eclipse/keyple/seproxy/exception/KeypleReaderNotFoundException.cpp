#include "KeypleReaderNotFoundException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace exception {
                    using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;

                    KeypleReaderNotFoundException::KeypleReaderNotFoundException(const std::string &readerName) : KeypleReaderException("Reader with name " + readerName + " was not found") {
                    }
                }
            }
        }
    }
}

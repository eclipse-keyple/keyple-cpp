#include "KeyplePluginException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace exception {

                    KeyplePluginException::KeyplePluginException(const std::string &message) : KeypleBaseException(message) {
                    }

                    KeyplePluginException::KeyplePluginException(const std::string &message, std::runtime_error cause) : KeypleBaseException(message, cause) {
                    }
                }
            }
        }
    }
}

#include "KeyplePluginNotFoundException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace exception {

                    KeyplePluginNotFoundException::KeyplePluginNotFoundException(const std::string &pluginName) : KeyplePluginException("Plugin with name " + pluginName + " was not found") {
                    }
                }
            }
        }
    }
}

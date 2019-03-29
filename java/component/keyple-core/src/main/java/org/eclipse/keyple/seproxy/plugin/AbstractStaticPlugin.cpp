#include "AbstractStaticPlugin.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {

                    AbstractStaticPlugin::AbstractStaticPlugin(const std::string &name) : AbstractObservablePlugin(name) {
                    }

                    void AbstractStaticPlugin::startObservation() {
                    }

                    void AbstractStaticPlugin::stopObservation() {
                    }

                    void AbstractStaticPlugin::addObserver(std::shared_ptr<Observer> observer) {
                        throw std::make_shared<IllegalAccessError>("Abstract Static Plugin does not support Observers, do not use this function");
                    }

                    void AbstractStaticPlugin::removeObserver(std::shared_ptr<Observer> observer) {
                        throw std::make_shared<IllegalAccessError>("Abstract Static Plugin does not support Observers, do not use this function");
                    }
                }
            }
        }
    }
}

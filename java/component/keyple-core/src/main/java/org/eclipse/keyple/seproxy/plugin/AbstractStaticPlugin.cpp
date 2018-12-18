#include "AbstractStaticPlugin.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {

                    AbstractStaticPlugin::AbstractStaticPlugin(const std::string &name) : AbstractObservablePlugin(name) {
                    }

                    void AbstractStaticPlugin::addObserver(std::shared_ptr<Observer> observer) {
                        AbstractObservablePlugin::addObserver(observer);
                    }

                    void AbstractStaticPlugin::removeObserver(std::shared_ptr<Observer> observer) {
                        AbstractObservablePlugin::removeObserver(observer);
                    }
                }
            }
        }
    }
}

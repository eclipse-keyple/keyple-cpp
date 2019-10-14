#include "AbstractStaticPlugin.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

AbstractStaticPlugin::AbstractStaticPlugin(const std::string &name) : AbstractObservablePlugin(name) {
}

void AbstractStaticPlugin::startObservation() {
}

void AbstractStaticPlugin::stopObservation() {
}

void AbstractStaticPlugin::addObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer) {
    (void)observer;
    throw std::runtime_error("Abstract Static Plugin does not support Observers, do not use this function");
}

void AbstractStaticPlugin::removeObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer) {
    (void)observer;
    throw std::runtime_error("Abstract Static Plugin does not support Observers, do not use this function");
}

}
}
}
}
}
}

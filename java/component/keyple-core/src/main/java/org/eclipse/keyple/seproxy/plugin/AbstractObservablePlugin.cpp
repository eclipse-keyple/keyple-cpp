#include "AbstractObservablePlugin.h"
#include "../event/PluginEvent.h"
#include "AbstractObservableReader.h"
#include "../exception/KeypleReaderException.h"
#include "../event/ObservablePlugin.h"
#include "../exception/KeypleReaderNotFoundException.h"
#include "../message/ProxyReader.h"

#include "LoggerFactory.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace plugin {

using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
using ObservablePlugin = org::eclipse::keyple::seproxy::event::ObservablePlugin;
using PluginEvent = org::eclipse::keyple::seproxy::event::PluginEvent;
using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;

const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(AbstractObservablePlugin));

AbstractObservablePlugin::AbstractObservablePlugin(const std::string &name)
: AbstractLoggedObservable<org::eclipse::keyple::seproxy::event::PluginEvent>(name)
{
    logger->debug("[AbstractObservablePlugin::AbstractObservablePlugin] name: %s\n", name);

/*
    logger->debug("[AbstractObservablePlugin::AbstractObservablePlugin] readers: %p\n", readers);
    if (readers == nullptr) {
        try {
            logger->debug("[AbstractObservablePlugin::AbstractObservablePlugin] initializing native readers\n");
            readers = initNativeReaders();
            logger->debug("[AbstractObservablePlugin::AbstractObservablePlugin] readers: %p\n", readers);
        }
        catch (KeypleReaderException &e) {
            logger->debug("[AbstractObservablePlugin::AbstractObservablePlugin] KeypleReaderException\n");
            e.printStackTrace();
        }
    }
*/
}

void AbstractObservablePlugin::initReaders()
{
    logger->debug("[AbstractObservablePlugin::AbstractObservablePlugin] readers: %p\n", readers);
    if (readers == nullptr) {
        try {
            logger->debug("[AbstractObservablePlugin::AbstractObservablePlugin] initializing native readers\n");
            readers = initNativeReaders();
            logger->debug("[AbstractObservablePlugin::AbstractObservablePlugin] readers: %p\n", readers);
        }
        catch (KeypleReaderException &e) {
            logger->debug("[AbstractObservablePlugin::AbstractObservablePlugin] KeypleReaderException\n");
            e.printStackTrace();
        }
    }
}

std::shared_ptr<std::set<std::shared_ptr<SeReader>>> AbstractObservablePlugin::getReaders() throw(KeypleReaderException) {
    if (readers == nullptr) {
        throw std::make_shared<KeypleReaderException>("List of readers has not been initialized");
    }
    return readers;
}

void AbstractObservablePlugin::startObservation() {
};

void AbstractObservablePlugin::stopObservation() {
};

void AbstractObservablePlugin::addObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    logger->debug("[AbstractObservablePlugin::addObserver] observer: %p\n", observer);

    std::shared_ptr<org::eclipse::keyple::util::Observer<PluginEvent>> _observer = std::dynamic_pointer_cast<org::eclipse::keyple::util::Observer<PluginEvent>>(observer);
    logger->debug("[AbstractObservablePlugin::addObserver] casted observer: %p\n", _observer);

    AbstractLoggedObservable<PluginEvent>::addObserver(_observer);
    if (AbstractLoggedObservable<PluginEvent>::countObservers() == 1) {
        logger->debug("[AbstractObservablePlugin::addObserver] start the plugin monitoring\n");
        startObservation();
    }
}

void AbstractObservablePlugin::removeObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    AbstractLoggedObservable<PluginEvent>::removeObserver(std::dynamic_pointer_cast<org::eclipse::keyple::util::Observer<PluginEvent>>(observer));
    if (AbstractLoggedObservable<PluginEvent>::countObservers() == 0) {
        logger->debug("[AbstractObservablePlugin::removeObserver] remove the plugin monitor");
        stopObservation();
    }
}

int AbstractObservablePlugin::compareTo(std::shared_ptr<ReaderPlugin> plugin)
{
    return this->AbstractLoggedObservable::getName().compare(plugin->getName());
}

std::shared_ptr<SeReader> AbstractObservablePlugin::getReader(const std::string &name)
{
    for (auto reader : *readers) {
        if (reader->getName() == name) {
            return std::shared_ptr<ProxyReader>(std::dynamic_pointer_cast<ProxyReader>(reader));
        }
    }
    throw std::make_shared<KeypleReaderNotFoundException>(name);
}

}
}
}
}
}

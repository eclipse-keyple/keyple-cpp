#include "AbstractObservablePlugin.h"
#include "PluginEvent.h"
#include "AbstractObservableReader.h"
#include "KeypleReaderException.h"
#include "ObservablePlugin.h"
#include "KeypleReaderNotFoundException.h"
#include "ProxyReader.h"

#include "LoggerFactory.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using ReaderPlugin                  = org::eclipse::keyple::core::seproxy::ReaderPlugin;
using ObservablePlugin              = org::eclipse::keyple::core::seproxy::event::ObservablePlugin;
using PluginEvent                   = org::eclipse::keyple::core::seproxy::event::PluginEvent;
using KeypleReaderException         = org::eclipse::keyple::core::seproxy::exception::KeypleReaderException;
using KeypleReaderNotFoundException = org::eclipse::keyple::core::seproxy::exception::KeypleReaderNotFoundException;
using ProxyReader                   = org::eclipse::keyple::core::seproxy::message::ProxyReader;

AbstractObservablePlugin::AbstractObservablePlugin(const std::string &name)
: AbstractLoggedObservable<PluginEvent>(name)
{
    logger->debug("constructor (name: %s)\n", name);
}

void AbstractObservablePlugin::initReaders()
{
    logger->debug("readers: %p\n", readers);
    if (readers == nullptr) {
        try {
            logger->debug("initializing native readers\n");
            readers = initNativeReaders();
            logger->debug("readers: %p\n", readers);
        } catch (KeypleReaderException &e) {
            logger->debug("KeypleReaderException\n");
            e.printStackTrace();
        }
    }
}

std::shared_ptr<std::set<std::shared_ptr<SeReader>>> AbstractObservablePlugin::getReaders()
{
    if (readers == nullptr) {
        throw std::make_shared<KeypleReaderException>("List of readers has not been initialized");
    }

    return readers;
}

std::shared_ptr<std::set<std::string>> AbstractObservablePlugin::getReaderNames()
{
    std::shared_ptr<std::set<std::string>> readerNames = std::make_shared<std::set<std::string>>();
    for (auto reader : *readers) {
        readerNames->insert(reader->getName());
    }
    return readerNames;
}

void AbstractObservablePlugin::addObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    AbstractLoggedObservable<PluginEvent>::addObserver(observer);
    if (AbstractLoggedObservable<PluginEvent>::countObservers() == 1) {
        logger->debug("Start the plugin monitoring\n");
        startObservation();
    }
}

void AbstractObservablePlugin::removeObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    AbstractLoggedObservable<PluginEvent>::removeObserver(observer);
    if (AbstractLoggedObservable<PluginEvent>::countObservers() == 0) {
        logger->debug("Stop the plugin monitoring\n");
        stopObservation();
    }
}

int AbstractObservablePlugin::compareTo(std::shared_ptr<ReaderPlugin> plugin)
{
    return this->AbstractLoggedObservable::getName().compare(plugin->getName());
}

/*
    * Alex: consider note in header comment (covariant return type).
    */
std::shared_ptr<SeReader> AbstractObservablePlugin::getReader(const std::string &name)
{
    logger->debug("getReader - looking for reader: %s in list of %d readers", name, readers->size());

    for (auto reader : *readers)
    {
        logger->debug("getReader - reader: %s", reader->getName());

        if (reader->getName() == name)
        {
            return std::shared_ptr<SeReader>(
                std::dynamic_pointer_cast<SeReader>(reader));
        }
    }
    throw std::make_shared<KeypleReaderNotFoundException>(name);
}

} // namespace plugin
}     // namespace seproxy
}         // namespace keyple
}             // namespace eclipse
} // namespace org
}

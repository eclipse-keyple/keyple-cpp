/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include "AbstractObservablePlugin.h"
#include "PluginEvent.h"
#include "AbstractObservableReader.h"
#include "KeypleReaderException.h"
#include "ObservablePlugin.h"
#include "KeypleReaderNotFoundException.h"
#include "ProxyReader.h"

#include "LoggerFactory.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;

AbstractObservablePlugin::AbstractObservablePlugin(const std::string &name)
: AbstractLoggedObservable<PluginEvent>(name)
{
    logger->debug("constructor (name: %s)\n", name);
}

void AbstractObservablePlugin::initReaders()
{
    if (readers == nullptr) {
        try {
            logger->debug("initializing native readers\n");
            readers = initNativeReaders();
        } catch (KeypleReaderException& e) {
            logger->error("initReaders - caught KeypleReaderException " \
                          "(message: %s, cause: %s)\n", e.getMessage(),
                          e.getCause().what());
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
    logger->debug("getReader - looking for reader: %s in list of %d readers\n", name, readers->size());

    for (auto reader : *readers)
    {
        logger->debug("getReader - reader: %s\n", reader->getName());

        if (reader->getName() == name)
        {
            return std::shared_ptr<SeReader>(
                std::dynamic_pointer_cast<SeReader>(reader));
        }
    }
    throw std::make_shared<KeypleReaderNotFoundException>(name);
}

}
}
}
}

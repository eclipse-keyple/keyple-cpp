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

#include "AbstractPlugin.h"

/* Core */
#include "PluginEvent.h"
#include "KeypleReaderException.h"
#include "KeypleRuntimeException.h"
#include "ObservablePlugin.h"
#include "KeypleReaderNotFoundException.h"
#include "ProxyReader.h"

/* Common */
#include "LoggerFactory.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;

AbstractPlugin::AbstractPlugin(const std::string& name) : name(name)
{
    try {
        readers = initNativeReaders();
    } catch (KeypleReaderException& e) {
        (void)e;
        throw KeypleRuntimeException("Could not instanciate readers in plugin"
                                     " constructor");
    }
}

std::set<std::shared_ptr<SeReader>> AbstractPlugin::initNativeReaders()
{
    return std::set<std::shared_ptr<SeReader>>();
}

std::set<std::shared_ptr<SeReader>>& AbstractPlugin::getReaders()
{
    if (!readers.size()) {
        throw KeypleReaderException("List of readers has not been initialized");
    }

    return readers;
}

const std::set<std::string> AbstractPlugin::getReaderNames()
{
    std::set<std::string> readerNames;

    for (auto reader : readers)
        readerNames.insert(reader->getName());

    return readerNames;
}

void AbstractPlugin::addObserver(
    std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    logger->trace("[] addObserver => Adding <fixme> as an observer of '%s'\n",
                  this->name.c_str());

    Observable<PluginEvent>::addObserver(observer);
}

void AbstractPlugin::removeObserver(
    std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    logger->trace("[%s] removeObserver => Deleting a plugin observer\n",
                  this->name);

    Observable<PluginEvent>::removeObserver(observer);
}

int AbstractPlugin::compareTo(std::shared_ptr<ReaderPlugin> plugin)
{
    logger->debug("compareTo - comparing %s to %s\n", this->name,
                  plugin->getName());

    return this->name.compare(plugin->getName());
}

/*
 * Alex: consider note in header comment (covariant return type).
 */
std::shared_ptr<SeReader> AbstractPlugin::getReader(const std::string& name)
{
    logger->debug("getReader - looking for reader: %s in list of %d readers\n",
                  name.c_str(), readers.size());

    for (auto reader : readers) {
        if (reader->getName() == name) {
            return std::shared_ptr<SeReader>(
                std::dynamic_pointer_cast<SeReader>(reader));
        }
    }

    throw KeypleReaderNotFoundException(name);
}

const std::string& AbstractPlugin::getName() const
{
    return name;
}

void AbstractPlugin::notifyObservers(std::shared_ptr<PluginEvent> event)
{
    logger->trace("[%s] AbstractPlugin => Notifying a plugin event to %d "
                  "observers. EVENTNAME = %s\n",
                  this->name.c_str(), this->countObservers(),
                  event->getEventType().getName().c_str());

    setChanged();

    Observable<PluginEvent>::notifyObservers(event);
}

void AbstractPlugin::setParameters(
    const std::map<std::string, std::string>& parameters)
{
    for (auto const& en : parameters) {
        setParameter(en.first, en.second);
    }
}

}
}
}
}

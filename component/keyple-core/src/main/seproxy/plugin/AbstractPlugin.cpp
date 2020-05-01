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

AbstractPlugin::AbstractPlugin(const std::string& name)
: AbstractSeProxyComponent(name)
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
    return readers;
}

const std::set<std::string> AbstractPlugin::getReaderNames()
{
    std::set<std::string> readerNames;

    for (auto reader : readers)
        readerNames.insert(reader->getName());

    return readerNames;
}

int AbstractPlugin::compareTo(std::shared_ptr<ReaderPlugin> plugin)
{
    return getName().compare(plugin->getName());
}

/*
 * Alex: consider note in header comment (covariant return type).
 */
std::shared_ptr<SeReader> AbstractPlugin::getReader(const std::string& name)
{
    for (auto reader : readers) {
        if (reader->getName() == name) {
            return std::shared_ptr<SeReader>(
                std::dynamic_pointer_cast<SeReader>(reader));
        }
    }

    throw KeypleReaderNotFoundException(name);
}

}
}
}
}

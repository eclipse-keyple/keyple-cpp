/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

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

AbstractPlugin::AbstractPlugin(const std::string& name) : AbstractSeProxyComponent(name)
{
    /*
     * /!\ C++ vs. Java: C++ cannot call a pure virtual function from a
     *                   constructor. That call must be made in the plugins
     *                   implementation directly
     *
     * mReaders = initNativeReaders();
     */
}

ConcurrentMap<const std::string, std::shared_ptr<SeReader>>& AbstractPlugin::getReaders()
{
    return mReaders;
}

const std::set<std::string> AbstractPlugin::getReaderNames() const
{
    std::set<std::string> readerNames;

    for (const auto& reader : mReaders)
        readerNames.insert(reader.first);

    return readerNames;
}

const std::shared_ptr<SeReader> AbstractPlugin::getReader(const std::string& name)
{
    ConcurrentMap<const std::string, std::shared_ptr<SeReader>> ::const_iterator it;

    if ((it = mReaders.find(name)) != mReaders.end())
        return it->second;
    else
        throw KeypleReaderNotFoundException(name);
}

}
}
}
}

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

#include "IOSPluginImpl.h"

/* Core */
#include "KeypleReaderIOException.h"
#include "KeypleReaderNotFoundException.h"

/* iOS plugin */
#include "IOSReader.h"
#include "IOSReaderImpl.h"
#include "IOSTerminalException.h"

namespace keyple {
namespace plugin {
namespace ios {

using namespace keyple::core::seproxy::exception;

std::shared_ptr<IOSPluginImpl> IOSPluginImpl::mInstance;

IOSPluginImpl::IOSPluginImpl() : AbstractThreadedObservablePlugin(PLUGIN_NAME)
{
    initNativeReaders();
}

std::shared_ptr<IOSPluginImpl> IOSPluginImpl::getInstance()
{
    if (!mInstance) {
        /*
         * Intermediate raw pointer required because constructor is private,
         * std::make_shared would trigger compilation errors
         */
        IOSPluginImpl* tmp = new IOSPluginImpl();
        mInstance = std::shared_ptr<IOSPluginImpl>(tmp);
    }

    return mInstance;
}

const std::map<const std::string, const std::string>& IOSPluginImpl::getParameters() const
{
    return mParameters;
}

void IOSPluginImpl::setParameter(const std::string& key, const std::string& value)
{
    (void)key;
    (void)value;
}

const std::set<std::string>& IOSPluginImpl::fetchNativeReadersNames()
{
    nativeReadersNames.clear();

    try {
        const std::vector<IOSTerminal>& terminals = getTerminals();

        logger->debug("fetchNativeReadersNames - filling native readers name "
                      "list with card terminal items\n");

        if (terminals.empty()) {
            logger->trace("fetchNativeReadersNames - no reader available\n");
        } else {
            for (auto& term : terminals) {
                logger->debug("fetchNativeReadersNames - reader: %\n",
                              term.getName());
                nativeReadersNames.insert(term.getName());
            }
        }
    } catch (IOSTerminalException& e) {
        logger->trace("fetchNativeReadersNames - terminal list is not "
                      "accessible, name: %, exception: %, cause: %\n",
                      getName(), e.getMessage(), e.getCause().what());
        throw KeypleReaderIOException("Could not access terminals list", e);
    }

    return nativeReadersNames;
}

ConcurrentMap<const std::string, std::shared_ptr<SeReader>>& IOSPluginImpl::initNativeReaders()
{
    /* C++ vs. Java: clear class member instead of creating new map */
    mNativeReaders.clear();

    /*
     * activate a special processing "SCARD_E_NO_NO_SERVICE" (on Windows
     * platforms the removal of the last PC/SC reader stops the "Windows Smart
     * Card service")
     */
#ifdef _WIN32
    scardNoServiceHackNeeded = true;
    logger->info("System detected : %\n", scardNoServiceHackNeeded);
#else
    scardNoServiceHackNeeded = false;
#endif

    /*
     * Parse the current readers list to create the ProxyReader(s) associated
     * with new reader(s).
     */
    logger->debug("initNativeReaders - getting card terminals\n");
    std::vector<IOSTerminal>& terminals = getTerminals();
    logger->trace("[%] initNativeReaders => CardTerminal in list: %\n", getName(), terminals);

    if (terminals.empty()) {
        logger->trace("No reader available\n");
        return mNativeReaders;
    }

    try {
        for (auto& term : terminals) {
            std::shared_ptr<IOSReaderImpl> IOSReader =
                std::make_shared<IOSReaderImpl>(getName(), term);
            mNativeReaders.insert({IOSReader->getName(), IOSReader});
        }
    } catch (IOSTerminalException& e) {
        logger->trace("[%] terminal list not accessible, msg: %, cause: %",
                      getName(),
                      e.getMessage(),
                      e.getCause().what());
        /*
         * Throw new KeypleReaderIOException("Could not access terminals list",
         * e); do not propagate exception at the constructor will propagate it
         * as a keyple::core::seproxy::exception::KeypleRuntimeException/
         */
    }

    return mNativeReaders;
}

std::shared_ptr<SeReader> IOSPluginImpl::fetchNativeReader(const std::string& name)
{
    /* Return the current reader if it is already listed */
    ConcurrentMap<const std::string, std::shared_ptr<SeReader>> ::const_iterator it;
    if ((it = mNativeReaders.find(name)) != mNativeReaders.end())
        return it->second;

    /*
     * Parse the current PC/SC readers list to create the ProxyReader(s)
     * associated with new reader(s)
     */
    std::shared_ptr<SeReader> seReader = nullptr;
    std::vector<IOSTerminal>& terminals = getTerminals();
    //std::vector<std::string> terminalList;

    try {
        for (auto& term : terminals) {
            if (!term.getName().compare(name)) {
                logger->trace("[%s] fetchNativeReader => CardTerminal in new IOSReader: %\n",
                              getName(),
                              terminals);
                seReader = std::make_shared<IOSReaderImpl>(getName(), term);
            }
        }
    } catch (const IOSTerminalException& e) {
        logger->trace("[%] caught IOSTerminalException (msg: %, cause: %)\n",
                      getName(),
                      e.getMessage(),
                      e.getCause().what());
        throw KeypleReaderIOException("Could not access terminals list", e);
    }

    if (seReader == nullptr)
        throw KeypleReaderNotFoundException("Reader " + name + " not found!");

    return seReader;
}

std::vector<IOSTerminal>& IOSPluginImpl::getTerminals()
{
    mTerminals.clear();

    try {
        const std::vector<std::string>& list = IOSTerminal::listTerminals();

        std::for_each(list.begin(),
                      list.end(),
                      [&](const std::string& name){mTerminals.push_back(IOSTerminal(name));});

    } catch (const IOSTerminalException& e) {
        (void)e;
        logger->error("unexpected exception - %", e);
    }

    return mTerminals;
}

}
}
}

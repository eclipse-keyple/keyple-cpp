/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include "PcscPluginImpl.h"

/* Core */
#include "KeypleReaderIOException.h"
#include "KeypleReaderNotFoundException.h"

/* PC/SC plugin */
#include "PcscReader.h"
#include "PcscReaderImpl.h"
#include "PcscTerminalException.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::core::seproxy::exception;

std::shared_ptr<PcscPluginImpl> PcscPluginImpl::mInstance;

PcscPluginImpl::PcscPluginImpl() : AbstractThreadedObservablePlugin(PLUGIN_NAME)
{
    mReaders = initNativeReaders();
}

std::shared_ptr<PcscPluginImpl> PcscPluginImpl::getInstance()
{
    if (!mInstance) {
        /*
         * Intermediate raw pointer required because constructor is private,
         * std::make_shared would trigger compilation errors
         */
        PcscPluginImpl* tmp = new PcscPluginImpl();
        mInstance = std::shared_ptr<PcscPluginImpl>(tmp);
    }

    return mInstance;
}

const std::map<const std::string, const std::string>&
    PcscPluginImpl::getParameters() const
{
    return mParameters;
}

void PcscPluginImpl::setParameter(const std::string& key,
                                  const std::string& value)
{
    (void)key;
    (void)value;
}

const std::set<std::string>& PcscPluginImpl::fetchNativeReadersNames()
{
    nativeReadersNames.clear();

    try {
        const std::vector<PcscTerminal>& terminals = getTerminals();

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
    } catch (PcscTerminalException& e) {
        logger->trace("fetchNativeReadersNames - terminal list is not "
                      "accessible, name: %, exception: %, cause: %\n",
                      getName(), e.getMessage(), e.getCause().what());
        throw KeypleReaderIOException("Could not access terminals list", e);
    }

    return nativeReadersNames;
}

std::map<const std::string, std::shared_ptr<SeReader>>
    PcscPluginImpl::initNativeReaders()
{
    std::map<const std::string, std::shared_ptr<SeReader>> nativeReaders;

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
    std::vector<PcscTerminal>& terminals = getTerminals();
    logger->trace("[%] initNativeReaders => CardTerminal in list: %\n",
                  getName(), terminals);

    if (terminals.empty()) {
        logger->trace("No reader available\n");
        return nativeReaders;
    }

    try {
        for (auto& term : terminals) {
            std::shared_ptr<PcscReaderImpl> pcscReader =
                std::make_shared<PcscReaderImpl>(getName(), term);
            nativeReaders.insert({pcscReader->getName(), pcscReader});
        }
    } catch (PcscTerminalException& e) {
        logger->trace("[%] terminal list not accessible, msg: %, cause: %",
                      getName(), e.getMessage(), e.getCause().what());
        /*
         * Throw new KeypleReaderIOException("Could not access terminals list",
         * e); do not propagate exception at the constructor will propagate it
         * as a keyple::core::seproxy::exception::KeypleRuntimeException/
         */
    }

    return nativeReaders;
}

std::shared_ptr<SeReader>
PcscPluginImpl::fetchNativeReader(const std::string& name)
{
    /* Return the current reader if it is already listed */
    if (mReaders.find(name) != mReaders.end())
        return mReaders[name];

    /*
     * Parse the current PC/SC readers list to create the ProxyReader(s)
     * associated with new reader(s)
     */
    std::shared_ptr<SeReader> seReader = nullptr;
    std::vector<PcscTerminal>& terminals = getTerminals();
    std::vector<std::string> terminalList;

    try {
        for (auto& term : terminals) {
            if (!term.getName().compare(name)) {
                logger->trace("[%s] fetchNativeReader => CardTerminal in new " \
                              "PcscReader: %\n", getName(), terminals);
                seReader = std::make_shared<PcscReaderImpl>(getName(), term);
            }
        }
    } catch (const PcscTerminalException& e) {
        logger->trace("[%] caught PcscTerminalException (msg: %, cause: %)\n",
                      getName(), e.getMessage(), e.getCause().what());
        throw KeypleReaderIOException("Could not access terminals list", e);
    }

    if (seReader == nullptr) {
        throw KeypleReaderNotFoundException("Reader " + name + " not found!");
    }

    return seReader;
}

std::vector<PcscTerminal>& PcscPluginImpl::getTerminals()
{
    mTerminals.clear();

    try {
        const std::vector<std::string>& list = PcscTerminal::listTerminals();

        for (auto name : list)
            mTerminals.push_back(PcscTerminal(name));

    } catch (PcscTerminalException& e) {
        (void)e;
        logger->error("unexpected exception - %", e);
    }

    return mTerminals;
}

}
}
}

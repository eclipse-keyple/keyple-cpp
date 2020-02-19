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

#include "PcscPluginImpl.h"

/* Core */
#include "KeypleBaseException.h"
#include "KeypleReaderException.h"
#include "KeypleRuntimeException.h"

/* PC/SC plugin */
#include "PcscReader.h"
#include "PcscReaderImpl_Import.h"
#include "PcscTerminalException.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::core::seproxy::exception;

PcscPluginImpl::PcscPluginImpl() : AbstractThreadedObservablePlugin(PLUGIN_NAME)
{
}

PcscPluginImpl& PcscPluginImpl::getInstance()
{
    if (uniqueInstance.readers.empty())
        throw KeypleRuntimeException("Reader list is not accessible");

    return uniqueInstance;
}

const std::map<const std::string, const std::string>
    PcscPluginImpl::getParameters()
{
     return std::map<const std::string, const std::string>();
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

        logger->debug("fetchNativeReadersNames - filling native readers name " \
                      "list with card terminal items\n");

        if (terminals.empty()) {
            logger->trace("fetchNativeReadersNames - no reader available\n");
        } else {
            for (auto &term : terminals) {
                logger->debug("fetchNativeReadersNames - reader: %s\n",
                              term.getName().c_str());
                nativeReadersNames.insert(term.getName());
            }
        }
    } catch (PcscTerminalException &e) {
        logger->trace("fetchNativeReadersNames - terminal list is not " \
                      "accessible, name: %s, exception: %s, cause: %s\n",
                      this->getName().c_str(), e.getMessage().c_str(),
                      e.getCause().what());
        throw KeypleReaderException("Could not access terminals list", e);
    }

    return nativeReadersNames;
}

std::set<std::shared_ptr<SeReader>> PcscPluginImpl::initNativeReaders()
{
    logger->debug("initNativeReaders - creating new list\n");
    std::set<std::shared_ptr<SeReader>> nativeReaders;

    /*
     * activate a special processing "SCARD_E_NO_NO_SERVICE" (on Windows
     * platforms the removal of the last PC/SC reader stops the "Windows Smart
     * Card service")
     */
#ifdef _WIN32
    scardNoServiceHackNeeded = true;
    logger.info("System detected : %d", scardNoServiceHackNeeded);
#else
    scardNoServiceHackNeeded = false;
#endif

    /*
     * Parse the current readers list to create the ProxyReader(s) associated
     * with new reader(s).
     */
    logger->debug("initNativeReaders - getting card terminals\n");
    std::vector<PcscTerminal>& terminals = getTerminals();
    logger->trace("[%s] initNativeReaders => CardTerminal in list: %s\n",
                  this->getName(), "terminals <fixme>");

    if (terminals.empty()) {
        logger->trace("No reader available\n");
        return nativeReaders;
    }

    try {
        for (auto &term : terminals) {
            logger->debug("initNativeReaders - inserting terminal into list\n");
            nativeReaders.insert(
                std::dynamic_pointer_cast<SeReader>(
                    std::make_shared<PcscReaderImpl>(this->getName(), term)));
        }
    } catch (PcscTerminalException &e) {
        logger->trace("[%s] terminal list not accessible, msg: %s, cause: %s",
                      this->getName().c_str(), e.getMessage().c_str(),
                      e.getCause().what());
        /*
         * Throw new KeypleReaderException("Could not access terminals list",
         * e); do not propagate exception at the constructor will propagate it
         * as a keyple::core::seproxy::exception::KeypleRuntimeException/
         */
    }

    return nativeReaders;
}

std::shared_ptr<SeReader> PcscPluginImpl::fetchNativeReader(
    const std::string &name)
{
    /* Return the current reader if it is already listed */
    for (auto reader : readers) {
        if (reader->getName() == name) {
            return reader;
        }
    }

    /*
     * Parse the current PC/SC readers list to create the ProxyReader(s)
     * associated with new reader(s)
     */
    std::shared_ptr<AbstractReader> reader = nullptr;
    std::vector<PcscTerminal>& terminals = getTerminals();
    std::vector<std::string> terminalList;

    try {
        for (auto& term : terminals) {
            if (!term.getName().compare(name)) {
                reader = std::make_shared<PcscReaderImpl>(this->getName(), term);
            }
        }
    } catch (PcscTerminalException& e) {
        logger->trace("[%s] caught PcscTerminalException (msg: %s, cause: " \
                      "%s)\n", this->getName().c_str(), e.getMessage().c_str(),
                      e.getCause().what());
        throw KeypleReaderException("Could not access terminals list", e);
    }

    if (reader == nullptr) {
        throw KeypleReaderException("Reader " + name + " not found!");
    }

    return reader;
}

std::vector<PcscTerminal>& PcscPluginImpl::getTerminals()
{
   terminals.clear();

   try {
       const std::vector<std::string>& list = PcscTerminal::listTerminals();

       for (auto name : list)
           terminals.push_back(PcscTerminal(name));

   } catch (PcscTerminalException& e) {
       logger->error("getTerminalsv - error listing terminals\n");
   }

   return terminals;
}

void PcscPluginImpl::addObserver(std::shared_ptr<PluginObserver> observer)
{
    AbstractThreadedObservablePlugin::addObserver(observer);
}

void PcscPluginImpl::removeObserver(std::shared_ptr<PluginObserver> observer)
{
    AbstractThreadedObservablePlugin::removeObserver(observer);
}

}
}
}

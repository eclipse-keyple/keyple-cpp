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

/* Core */
#include "KeypleBaseException.h"
#include "KeypleReaderException.h"
#include "AbstractObservableReader.h"

/* PC/SC plugin */
#include "PcscReader.h"
#include "PcscPlugin.h"
#include "PcscTerminalException.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::exception;

PcscPlugin::PcscPlugin() : AbstractThreadedObservablePlugin("PcscPlugin")
{
    logger->debug("constructor\n");
}

PcscPlugin PcscPlugin::getInstance()
{
    static PcscPlugin uniqueInstance;

    return uniqueInstance;
}

std::unordered_map<std::string, std::string> PcscPlugin::getParameters() {
     return std::unordered_map<std::string, std::string>();
}

void PcscPlugin::setParameter(const std::string &key, const std::string &value)
{
    (void)key;
    (void)value;
}

std::shared_ptr<PcscPlugin> PcscPlugin::setLogging(bool logging)
{
    this->logging = logging;
                        return shared_from_this();
}

std::shared_ptr<std::set<std::string>> PcscPlugin::fetchNativeReadersNames()
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
                              static_cast<std::string>(term.getName()));
                nativeReadersNames.insert(term.getName());
            }
        }
    } catch (PcscTerminalException &e) {
        logger->trace("fetchNativeReadersNames - terminal list is not " \
                      "accessible, name: %s, exception: %s, cause: %s\n",
                      this->getName(), e.getMessage(), e.getCause().what());
        throw KeypleReaderException("Could not access terminals list", e);
    }

    return std::make_shared<std::set<std::string>>(nativeReadersNames);
}

std::shared_ptr<std::set<std::shared_ptr<SeReader>>>
PcscPlugin::initNativeReaders()
{
    logger->debug("initNativeReaders - creating new list\n");
    std::shared_ptr<std::set<std::shared_ptr<SeReader>>> nativeReaders =
        std::shared_ptr<std::set<std::shared_ptr<SeReader>>>(
            new std::set<std::shared_ptr<SeReader>>());

    /*
     * Parse the current readers list to create the ProxyReader(s) associated
     * with new reader(s).
     */
    logger->debug("initNativeReaders - getting card terminals\n");
    std::vector<PcscTerminal>& terminals = getTerminals();

    if (terminals.empty()) {
        logger->trace("initNativeReaders - no reader available\n");
        return nativeReaders;
    }

    try {
        for (auto &term : terminals) {
            logger->debug("initNativeReaders - inserting terminal into list\n");
            nativeReaders->insert(
                std::make_shared<PcscReader>(this->getName(), term));
        }
    } catch (PcscTerminalException &e) {
        logger->trace("[%s] terminal list not accessible, msg: %s, cause: %s",
                      this->getName(), e.getMessage(), e.getCause().what());
        throw KeypleReaderException("Could not access terminals list", e);
    }

    return nativeReaders;
}

std::shared_ptr<AbstractObservableReader>
PcscPlugin::fetchNativeReader(const std::string &name)
{
    // return the current reader if it is already listed
    for (auto reader : *readers) {
        if (reader->getName() == name) {
            return std::dynamic_pointer_cast<AbstractObservableReader>(reader);
        }
    }
    /*
     * parse the current PC/SC readers list to create the ProxyReader(s)
     * associated with new reader(s)
     */
    std::shared_ptr<AbstractObservableReader> reader = nullptr;
    std::vector<PcscTerminal>& terminals = getTerminals();
    std::vector<std::string> terminalList;

    try {
        for (auto& term : terminals) {
            if (!term.getName().compare(name)) {
                reader = std::make_shared<PcscReader>(this->getName(), term);
            }
        }
    } catch (PcscTerminalException& e) {
        logger->trace("[%s] caught PcscTerminalException (msg: %s, cause: " \
                      "%s)\n", this->getName(), e.getMessage(),
                      e.getCause().what());
        throw KeypleReaderException("Could not access terminals list", e);
    }

    if (reader == nullptr) {
        throw KeypleReaderException("Reader " + name + " not found!");
    }
    return reader;
}

std::vector<PcscTerminal>& PcscPlugin::getTerminals()
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

}
}
}

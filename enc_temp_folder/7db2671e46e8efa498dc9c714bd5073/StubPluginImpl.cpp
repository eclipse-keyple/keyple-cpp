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

#include "StubPluginImpl.h"

/* Stub Plugin */
#include "StubReaderImpl.h"

namespace keyple {
namespace plugin {
namespace stub {

StubPluginImpl::StubPluginImpl(const std::string& pluginName)
: AbstractThreadedObservablePlugin(pluginName)
{
    /*
     * Monitoring is not handled by a lower layer (as in PC/SC), reduce the
     * threading period to 10 ms to speed up responsiveness.
     */
    threadWaitTimeout = 10;
}

const std::map<const std::string, const std::string>
StubPluginImpl::getParameters()
{
    return parameters;
}

void StubPluginImpl::setParameter(const std::string& key,
                                  const std::string& value)
{
    parameters.insert(
        std::pair<const std::string, const std::string>(key, value));
}

void StubPluginImpl::plugStubReader(const std::string& readerName,
                                    bool synchronous)
{
    plugStubReader(readerName, TransmissionMode::CONTACTLESS, synchronous);
}

void StubPluginImpl::plugStubReader(const std::string& readerName,
                                    TransmissionMode transmissionMode,
                                    bool synchronous)
{
    logger->info("Plugging a new reader with readerName %s\n",
                 readerName.c_str());

    /* Add the native reader to the native readers list */
    bool exist =
        connectedStubNames.find(readerName) != connectedStubNames.end();

    if (!exist && synchronous) {
        /* add the reader as a new reader to the readers list */
        logger->debug("Inserting reader %s into 'readers' list\n",
                 readerName.c_str());
        readers.insert(std::make_shared<StubReaderImpl>(
            this->getName(), readerName, transmissionMode));
    }

    connectedStubNames.insert(readerName);

    if (exist) {
        logger->error("Reader with readerName %s was already plugged\n",
                      readerName.c_str());
    }
}

void StubPluginImpl::plugStubReaders(const std::set<std::string>& readerNames,
                                     bool synchronous)
{
    logger->debug("Plugging %d readers\n", readerNames.size());

    /*
     * Plug stub readers that were not plugged already
     * Duplicate readerNames
     */
    std::set<std::string> newNames = readerNames;

    /* Remove already connected stubNames */
    for (auto& connected : connectedStubNames)
        newNames.erase(connected);

    logger->debug("New readers to be created #%d\n", newNames.size());

    /*
     * Add new readerNames to the connectedStubNames
     */
    if (newNames.size() > 0) {
        if (synchronous) {
            std::list<std::shared_ptr<StubReaderImpl>> newReaders;
            for (auto& name : newNames) {
                newReaders.push_back(
                    std::make_shared<StubReaderImpl>(this->getName(), name));
            }

            std::copy(newReaders.begin(), newReaders.end(),
                      std::inserter(readers, readers.end()));
        }

        std::copy(readerNames.begin(), readerNames.end(),
                  std::inserter(connectedStubNames, connectedStubNames.end()));
    } else {
        logger->error("All %s readers were already plugged\n",
                      readerNames.size());
    }
}

void StubPluginImpl::unplugStubReader(const std::string& readerName,
                                      bool synchronous)
{
    if (connectedStubNames.find(readerName) == connectedStubNames.end()) {
        logger->warn("unplugStubReader() No reader found with name %s\n",
                     readerName.c_str());
    } else {
        /* Remove the reader from the readers list */
        if (synchronous) {
            logger->debug("synchronous unplug\n");
            connectedStubNames.erase(readerName);
            std::shared_ptr<SeReader> readerToErase = getReader(readerName);
            readers.erase(readerToErase);
        } else {
            logger->debug("asynchronous unplg\n");
            connectedStubNames.erase(readerName);
        }

        /* Remove the native reader from the native readers list */
        logger->info("Unplugged reader with name %s, connectedStubNames size" \
                     ": %d\n", readerName.c_str(), connectedStubNames.size());
    }
}

void StubPluginImpl::unplugStubReaders(const std::set<std::string>& readerNames,
                                       bool synchronous)
{
    logger->info("Unplug %d stub readers\n", readerNames.size());
    logger->debug("Unplug stub readers.. %s", "readerNames <fixme>");

    std::list<std::shared_ptr<StubReaderImpl>> readersToDelete;

    for (const std::string name : readerNames) {
        try {
            std::shared_ptr<SeReader> seReader = getReader(name);
            std::shared_ptr<StubReaderImpl> stubReaderImpl =
                std::dynamic_pointer_cast<StubReaderImpl>(seReader);
            readersToDelete.push_back(stubReaderImpl);
        } catch (KeypleReaderNotFoundException& e) {
            (void)e;
            logger->warn("unplugStubReaders() No reader found with name %s\n",
                         name.c_str());
        }
    }

    for (const auto& readerName : readerNames)
        connectedStubNames.erase(readerName);

    if (synchronous) {
        for (const auto& readerToDelete : readersToDelete)
            readers.erase(readerToDelete);
    }
}

const std::set<std::string>& StubPluginImpl::fetchNativeReadersNames()
{
    if (connectedStubNames.empty()) {
        logger->trace("No reader available\n");
    }

    return connectedStubNames;
}

std::set<std::shared_ptr<SeReader>> StubPluginImpl::initNativeReaders()
{
    /* Init Stub Readers response object */
    std::set<std::shared_ptr<SeReader>> newNativeReaders;

    return newNativeReaders;
}

std::shared_ptr<SeReader>
StubPluginImpl::fetchNativeReader(const std::string& readerName)
{
    for (const auto& reader : readers) {
        if (!reader->getName().compare(readerName)) {
            return reader;
        }
    }

    std::shared_ptr<SeReader> reader = nullptr;

    if (connectedStubNames.find(readerName) != connectedStubNames.end()) {
        reader = std::make_shared<StubReaderImpl>(this->getName(), readerName);
    }

    return reader;
}

void StubPluginImpl::addObserver(std::shared_ptr<PluginObserver> observer)
{
    AbstractThreadedObservablePlugin::addObserver(observer);
}

void StubPluginImpl::removeObserver(std::shared_ptr<PluginObserver> observer)
{
    AbstractThreadedObservablePlugin::removeObserver(observer);
}

}
}
}

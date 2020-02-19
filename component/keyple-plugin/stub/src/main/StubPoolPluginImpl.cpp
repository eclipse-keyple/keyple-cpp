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

#include "StubPoolPluginImpl.h"

/* Common */
#include "stringhelper.h"

/* Stub plugin */
#include "StubPoolPluginFactory.h"
#include "StubReaderImpl.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::common;
using namespace keyple::core::seproxy;

StubPoolPluginImpl::StubPoolPluginImpl(const std::string& pluginName)
: stubPlugin(
    dynamic_cast<StubPluginImpl&>(
        std::make_shared<StubPoolPluginFactory>(pluginName)
            ->getPluginInstance()))
{
    /* Create an embedded stubplugin to manage reader */
}

const std::string& StubPoolPluginImpl::getName() const
{
    return stubPlugin.getName();
}

std::set<std::string> StubPoolPluginImpl::getReaderGroupReferences()
{
    std::set<std::string> v;
    std::map<const std::string, std::shared_ptr<StubReaderImpl>>::iterator it;

    for(it = readerPool.begin(); it != readerPool.end(); ++it)
        v.insert(it->first);

    return v;
}

std::shared_ptr<SeReader> StubPoolPluginImpl::plugStubPoolReader(
    const std::string& groupReference, const std::string& readerName,
    std::shared_ptr<StubSecureElement> se)
{
    try {
        /* Create new reader */
        stubPlugin.plugStubReader(readerName, true);

        /* Get new reader */
        std::shared_ptr<StubReaderImpl> newReader =
            std::dynamic_pointer_cast<StubReaderImpl>(
                stubPlugin.getReader(readerName));

        newReader->insertSe(se);

        /* Map reader to groupReference */
        readerPool.insert(
            std::pair<const std::string, std::shared_ptr<StubReaderImpl>>(
                groupReference, newReader));

        return newReader;
    } catch (KeypleReaderNotFoundException& e) {
        throw IllegalStateException("Impossible to allocateReader, stubplugin" \
                                    " failed to create a reader");
    }
}

void StubPoolPluginImpl::unplugStubPoolReader(const std::string& groupReference)
{
    try {
        /* Get reader */
        std::shared_ptr<SeReader> stubReader =
            readerPool.find(groupReference)->second;

        /* Remove reader from pool */
        readerPool.erase(groupReference);

        /* Remove reader from plugin */
        stubPlugin.unplugStubReader(stubReader->getName(), true);

    } catch (KeypleReaderException& e) {
        throw IllegalStateException(
                  StringHelper::formatSimple(
                      "Impossible to release reader, reader with " \
                      "groupReference was not found in stubplugin : %s",
                      groupReference));
    }
}

std::shared_ptr<SeReader> StubPoolPluginImpl::allocateReader(
    const std::string& groupReference)
{
    /* Find the reader in the readerPool */
    std::shared_ptr<StubReaderImpl> seReader =
        readerPool.find(groupReference)->second;

    /* Check if the reader is available */
    if (seReader == nullptr ||
        allocatedReader.find(seReader->getName()) != allocatedReader.end()) {
        return nullptr;
    } else {
        allocatedReader.insert(
            std::pair<const std::string, const std::string>(
                seReader->getName(), groupReference));
        return seReader;
    }
}

void StubPoolPluginImpl::releaseReader(std::shared_ptr<SeReader> seReader)
{
    if (seReader == nullptr) {
        throw IllegalArgumentException("Could not release seReader, seReader " \
                                       "is null");
    }

    std::shared_ptr<StubReaderImpl> stubReader =
        std::dynamic_pointer_cast<StubReaderImpl>(seReader);

    if (!stubReader) {
        throw IllegalArgumentException("Can not release seReader, SeReader " \
                                       "should be of type StubReader");
    }

    /**
     * Remove and Re-insert SE to reset logical channel
     */
    if (stubReader->checkSePresence()) {
        std::shared_ptr<StubSecureElement> se = stubReader->getSe();
        stubReader->removeSe();
        stubReader->insertSe(se);
    }

    allocatedReader.erase(seReader->getName());

}

const std::map<const std::string, const std::string>&
    StubPoolPluginImpl::listAllocatedReaders()
{
    return allocatedReader;
}

const std::set<std::string> StubPoolPluginImpl::getReaderNames()
{
    return stubPlugin.getReaderNames();
}

std::set<std::shared_ptr<SeReader>>& StubPoolPluginImpl::getReaders()
{
    return stubPlugin.getReaders();
}

std::shared_ptr<SeReader> StubPoolPluginImpl::getReader(const std::string& name)
{
    return stubPlugin.getReader(name);
}

int StubPoolPluginImpl::compareTo(std::shared_ptr<ReaderPlugin> plugin)
{
    return stubPlugin.compareTo(plugin);
}

const std::map<const std::string, const std::string>
    StubPoolPluginImpl::getParameters()
{
    return stubPlugin.getParameters();
}

void StubPoolPluginImpl::setParameter(const std::string& key,
                                      const std::string& value)
{
    stubPlugin.setParameter(key, value);
}

void StubPoolPluginImpl::setParameters(
    const std::map<std::string, std::string>& parameters)
{
    stubPlugin.setParameters(parameters);
}

}
}
}

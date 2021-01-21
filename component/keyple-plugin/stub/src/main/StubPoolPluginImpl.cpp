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

#include "StubPoolPluginImpl.h"

/* Common */
#include "stringhelper.h"

/* Core */
#include "IllegalArgumentException.h"
#include "IllegalStateException.h"
#include "KeypleAllocationNoReaderException.h"
#include "KeypleAllocationReaderException.h"

/* Stub plugin */
#include "StubPluginFactory.h"
#include "StubReaderImpl.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy;

StubPoolPluginImpl::StubPoolPluginImpl(const std::string& pluginName)
{
    /* Create an embedded stubplugin to manage reader */
    auto factory = std::make_shared<StubPluginFactory>(pluginName);
    std::shared_ptr<ReaderPlugin> plugin = factory->getPlugin();
    auto stubPlugin = std::dynamic_pointer_cast<StubPluginImpl>(plugin);
    mStubPlugin = stubPlugin;
}

const std::string& StubPoolPluginImpl::getName() const
{
    return mStubPlugin->getName();
}

std::set<std::string> StubPoolPluginImpl::getReaderGroupReferences()
{
    std::set<std::string> v;
    std::map<const std::string, std::shared_ptr<StubReaderImpl>>::iterator it;

    for (it = mReaderPool.begin(); it != mReaderPool.end(); ++it)
        v.insert(it->first);

    return v;
}

std::shared_ptr<SeReader> StubPoolPluginImpl::plugStubPoolReader(
    const std::string& groupReference, const std::string& readerName,
    std::shared_ptr<StubSecureElement> se)
{
    try {
        /* Create new reader */
        mStubPlugin->plugStubReader(readerName, true);

        /* Get new reader */
        std::shared_ptr<StubReaderImpl> newReader =
            std::dynamic_pointer_cast<StubReaderImpl>(
                mStubPlugin->getReader(readerName));

        newReader->insertSe(se);

        /* Map reader to groupReference */
        mReaderPool.insert(
            std::pair<const std::string, std::shared_ptr<StubReaderImpl>>(
                groupReference, newReader));

        return newReader;
    } catch (KeypleReaderNotFoundException& e) {
        (void)e;
        throw IllegalStateException("Impossible to allocateReader, stubplugin"
                                    " failed to create a reader");
    }
}

void StubPoolPluginImpl::unplugStubPoolReader(const std::string& groupReference)
{
    try {
        /* Get reader */
        std::shared_ptr<SeReader> stubReader =
            mReaderPool.find(groupReference)->second;

        /* Remove reader from pool */
        mReaderPool.erase(groupReference);

        /* Remove reader from plugin */
        mStubPlugin->unplugStubReader(stubReader->getName(), true);

    } catch (KeypleReaderException& e) {
        (void)e;
        throw IllegalStateException(
                  "Impossible to release reader, reader with " \
                  "groupReference was not found in stubplugin : " +
                  groupReference);
    }
}

std::shared_ptr<SeReader> StubPoolPluginImpl::allocateReader(
    const std::string& groupReference)
{
    /* Find the reader in the readerPool */
    std::shared_ptr<StubReaderImpl> seReader =
        mReaderPool.find(groupReference)->second;

    /* Check if the reader is found */
    if (seReader == nullptr)
        throw KeypleAllocationReaderException(
                 "Impossible to allocate a reader for groupReference : " +
                 groupReference +
                 ". Has the reader being plugged to this referenceGroup?");

    /* Check if reader is available */
    if (mAllocatedReader.find(seReader->getName()) != mAllocatedReader.end())
        throw KeypleAllocationNoReaderException(
                  "Impossible to allocate a reader for groupReference : " +
                  groupReference +
                  ". No reader Available");

    /* Allocate reader */
    mAllocatedReader.insert({seReader->getName(), groupReference});
    return seReader;
}

void StubPoolPluginImpl::releaseReader(std::shared_ptr<SeReader> seReader)
{
    if (seReader == nullptr) {
        throw IllegalArgumentException("Could not release seReader, seReader "
                                       "is null");
    }

    std::shared_ptr<StubReaderImpl> stubReader =
        std::dynamic_pointer_cast<StubReaderImpl>(seReader);

    if (!stubReader) {
        throw IllegalArgumentException("Can not release seReader, SeReader "
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

    mAllocatedReader.erase(seReader->getName());
}

const std::map<const std::string, const std::string>&
    StubPoolPluginImpl::listAllocatedReaders()
{
    return mAllocatedReader;
}

const std::set<std::string> StubPoolPluginImpl::getReaderNames() const
{
    return mStubPlugin->getReaderNames();
}

ConcurrentMap<const std::string, std::shared_ptr<SeReader>>&
    StubPoolPluginImpl::getReaders()
{
    return mStubPlugin->getReaders();
}

const std::shared_ptr<SeReader> StubPoolPluginImpl::getReader(
    const std::string& name)
{
    return mStubPlugin->getReader(name);
}

const std::map<const std::string, const std::string>&
    StubPoolPluginImpl::getParameters() const
{
    return mStubPlugin->getParameters();
}

void StubPoolPluginImpl::setParameter(const std::string& key,
                                      const std::string& value)
{
    mStubPlugin->setParameter(key, value);
}

void StubPoolPluginImpl::setParameters(
    const std::map<const std::string, const std::string>& parameters)
{
    mStubPlugin->setParameters(parameters);
}

}
}
}

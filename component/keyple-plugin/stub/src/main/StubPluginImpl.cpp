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

#include "StubPluginImpl.h"

/* Common */
#include "KeypleStd.h"

/* Core */
#include "AbstractObservableState.h"

/* Stub Plugin */
#include "StubReaderImpl.h"

namespace keyple {
namespace plugin {
namespace stub {

StubPluginImpl::StubPluginImpl(const std::string& pluginName)
: AbstractThreadedObservablePlugin(pluginName)
{
    /*
     * Monitoring is not handled by a lower layer (as in PC/SC), reduce the threading period to 10
     * ms to speed up responsiveness.
     */
    mThreadWaitTimeout = 10;

    initNativeReaders();
}

const std::map<const std::string, const std::string>& StubPluginImpl::getParameters() const
{
    return mParameters;
}

void StubPluginImpl::setParameter(const std::string& key, const std::string& value)
{
    mParameters.insert({key, value});
}

void StubPluginImpl::plugStubReader(const std::string& readerName, bool synchronous)
{
    plugStubReader(readerName, TransmissionMode::CONTACTLESS, synchronous);
}

void StubPluginImpl::plugStubReader(const std::string& readerName,
                                    TransmissionMode transmissionMode,
                                    bool synchronous)
{
    mLogger->info("Plugging a new reader with readerName %\n", readerName);

    /* Add the native reader to the native readers list */
    bool exist =  mConnectedStubNames.find(readerName) != mConnectedStubNames.end();

    if (!exist && synchronous) {
        /* add the reader as a new reader to the readers list */
        auto reader = std::make_shared<StubReaderImpl>(getName(), readerName, transmissionMode);
        mNativeReaders.insert({readerName, reader});
    }

    mConnectedStubNames.insert(readerName);

    if (exist) {
        mLogger->error("Reader with readerName % was already plugged\n", readerName);
    }
}

void StubPluginImpl::plugStubReaders(const std::set<std::string>& readerNames,
                                     bool synchronous)
{
    mLogger->debug("Plugging % readers\n", readerNames.size());

    /* Plug stub readers that were not plugged already duplicate readerNames */
    std::set<std::string> newNames = readerNames;

    /* Remove already connected stubNames */
    for (auto& connected : mConnectedStubNames)
        newNames.erase(connected);

    mLogger->debug("New readers to be created #%\n", newNames.size());

    /* Add new readerNames to the connectedStubNames */
    if (newNames.size() > 0) {
        if (synchronous) {
            ConcurrentMap<const std::string, std::shared_ptr<StubReaderImpl>> newReaders;

            for (auto& name : newNames)
                newReaders.insert({name, std::make_shared<StubReaderImpl>(this->getName(), name)});

            std::copy(newReaders.begin(),
                      newReaders.end(),
                      std::inserter(mNativeReaders, mNativeReaders.end()));
        }

        std::copy(readerNames.begin(),
                  readerNames.end(),
                  std::inserter(mConnectedStubNames, mConnectedStubNames.end()));
    } else {
        mLogger->error("All % readers were already plugged\n", readerNames.size());
    }
}

void StubPluginImpl::unplugStubReader(const std::string& readerName,
                                      bool synchronous)
{
    if (mConnectedStubNames.find(readerName) == mConnectedStubNames.end()) {
        mLogger->warn("unplugStubReader() No reader found with name %\n", readerName);
    } else {
        /* Remove the reader from the readers list */
        if (synchronous) {
            mConnectedStubNames.erase(readerName);
            mNativeReaders.erase(readerName);
        } else {
            mConnectedStubNames.erase(readerName);
        }

        /* Remove the native reader from the native readers list */
        mLogger->info("Unplugged reader with name %, connectedStubNames size : %\n",
                      readerName,
                      mConnectedStubNames.size());
    }
}

void StubPluginImpl::unplugStubReaders(const std::set<std::string>& readerNames,
                                       const bool synchronous)
{
    mLogger->info("Unplug % stub readers\n", readerNames.size());
    mLogger->debug("Unplug stub readers.. %\n", readerNames);

    std::list<std::shared_ptr<StubReaderImpl>> readersToDelete;

    for (const std::string name : readerNames) {
        try {
            readersToDelete.push_back(std::dynamic_pointer_cast<StubReaderImpl>(getReader(name)));
        } catch (KeypleReaderNotFoundException& e) {
            (void)e;
            mLogger->warn("unplugStubReaders() No reader found with name %\n", name);
        }
    }

    for (const auto& readerName : readerNames)
        mConnectedStubNames.erase(readerName);

    if (synchronous) {
        for (const auto& reader : readersToDelete)
            mNativeReaders.erase(reader->getName());
    }
}

const std::set<std::string>& StubPluginImpl::fetchNativeReadersNames()
{
    mLogger->debug("fetching native reader names\n");

    if (mConnectedStubNames.size() == 0) {
        mLogger->trace("No reader available\n");
    }

    return mConnectedStubNames;
}

ConcurrentMap<const std::string, std::shared_ptr<SeReader>>& StubPluginImpl::initNativeReaders()
{
    /* Init Stub Readers response object */
    mNativeReaders.clear();

    return mNativeReaders;
}

std::shared_ptr<SeReader> StubPluginImpl::fetchNativeReader(
    const std::string& readerName)
{
    std::shared_ptr<SeReader> reader = nullptr;

    ConcurrentMap<const std::string, std::shared_ptr<SeReader>>
        ::const_iterator it = mNativeReaders.find(readerName);

    if (it != mNativeReaders.end())
        reader = it->second;

    if (reader == nullptr &&
        mConnectedStubNames.find(readerName) != mConnectedStubNames.end())
        reader = std::make_shared<StubReaderImpl>(getName(), readerName);

    return reader;
}

}
}
}

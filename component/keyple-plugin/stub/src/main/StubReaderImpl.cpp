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

#include "StubReaderImpl.h"

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleReaderIOException.h"
#include "KeypleReaderException.h"
#include "ObservableReaderStateService.h"
#include "SeProtocol.h"
#include "SmartInsertionMonitoringJob.h"
#include "SmartRemovalMonitoringJob.h"
#include "WaitForStartDetect.h"
#include "WaitForSeInsertion.h"
#include "WaitForSeProcessing.h"
#include "WaitForSeRemoval.h"

/* Plugin - Stub */
#include "StubSecureElement.h"

/* Common */
#include "Thread.h"
#include "IllegalArgumentException.h"
#include "InterruptedException.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::protocol;


StubReaderImpl::StubReaderImpl(const std::string& pluginName,
                               const std::string& readerName,
                               TransmissionMode transmissionMode)
: AbstractObservableLocalReader(pluginName, readerName),
  mTransmissionMode(transmissionMode)
{
    mLoopWaitSe = false;
    mLoopWaitSeRemoval = false;

    /* Create a executor service with one thread whose name is customized */
    mExecutorService = std::make_shared<MonitoringPool>();

    mStateService = initStateService();
}


StubReaderImpl::StubReaderImpl(const std::string& pluginName, const std::string& readerName)
: StubReaderImpl(pluginName, readerName, TransmissionMode::NONE) {}

StubReaderImpl::~StubReaderImpl()
{
    mShuttingDown = true;
    if (mStateService) {
        for (auto& state : mStateService->getStates()) {
            if (state.second && state.second->mMonitoringJob) {
                state.second->mMonitoringJob->stop();
                while (state.second->mMonitoringJob->isRunning());
            }
        }
    }
}

const std::vector<uint8_t>& StubReaderImpl::getATR()
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (mSe == nullptr)
        throw KeypleReaderIOException("No SE available.");

    return mSe->getATR();
}

bool StubReaderImpl::isPhysicalChannelOpen()
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mSe != nullptr && mSe->isPhysicalChannelOpen();
}

void StubReaderImpl::openPhysicalChannel()
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (mSe == nullptr)
        throw KeypleReaderIOException("No SE available.");

    mSe->openPhysicalChannel();
}

void StubReaderImpl::closePhysicalChannel()
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (mSe == nullptr)
        throw KeypleReaderIOException("No SE available.");
    mSe->closePhysicalChannel();
}

std::vector<uint8_t> StubReaderImpl::transmitApdu(const std::vector<uint8_t>& apduIn)
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (mSe == nullptr)
        throw KeypleReaderIOException("No SE available.");

    return mSe->processApdu(apduIn);
}

bool StubReaderImpl::protocolFlagMatches(const std::shared_ptr<SeProtocol> protocolFlag)
{
    bool result;

    /* Test protocolFlag to check if ATR based protocol filtering is required */
    //if (protocolFlag != null) {
        if (!isPhysicalChannelOpen())
            openPhysicalChannel();

        /*
         * C++ vs. Java: moved that verification after isPhysicalChannelOpen() and
         * openPhysicalChannel() as they both lock the class mutex for mSe manipulation.
         */
        std::lock_guard<std::mutex> lock(mMutex);
     
        if (mSe == nullptr)
            throw KeypleReaderIOException("No SE available.");

        /* The request will be executed only if the protocol match the requestElement */
        const std::string& selectionMask = getProtocolsMap().find(protocolFlag)->second;

        if (selectionMask.empty())
            throw KeypleReaderIOException("Target selector mask not found!");

        Pattern* p = Pattern::compile(selectionMask);
        const std::string& protocol = mSe->getSeProcotol();
        if (!p->matcher(protocol)->matches()) {
            mLogger->trace("[%] protocolFlagMatches => unmatching SE. PROTOCOLFLAG = %\n",
                           getName(),
                           protocolFlag);
            result = false;
        } else {
            mLogger->trace("[%] protocolFlagMatches => matching SE. PROTOCOLFLAG = %\n",
                           getName(),
                           protocolFlag);
            result = true;
        } //else {
        //    // no protocol defined returns true
        //    result = true;
    //}

    return result;
}

bool StubReaderImpl::checkSePresence()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mLogger->trace("SE: %\n", mSe);
    return mSe != nullptr;
}

void StubReaderImpl::setParameter(const std::string& name, const std::string& value)
{
    mParameters.insert({name, value});
}

const std::map<const std::string, const std::string>& StubReaderImpl::getParameters() const
{
    return mParameters;
}

const TransmissionMode& StubReaderImpl::getTransmissionMode() const
{
    return mTransmissionMode;
}

void StubReaderImpl::insertSe(std::shared_ptr<StubSecureElement> se)
{
    mLogger->debug("Insert SE %\n", se);

    /* Clean channels status */
    mLogger->trace("Verifying if physical channel is open\n");
    if (isPhysicalChannelOpen()) {
        try {
            mLogger->trace("Closing physical channel\n");
            closePhysicalChannel();
        } catch (KeypleReaderException& e) {
            mLogger->error("Error while closing channel reader. %\n", e.getMessage());
        }
    }

    std::lock_guard<std::mutex> lock(mMutex);
    mLogger->trace("Setting SE to %\n", se);
    if (se != nullptr)
        mSe = se;
}

void StubReaderImpl::removeSe()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mLogger->debug("Remove SE %\n", mSe);
    mSe = nullptr;
}

std::shared_ptr<StubSecureElement> StubReaderImpl::getSe()
{
    return mSe;
}

bool StubReaderImpl::waitForCardPresent()
{
    /* C++ vs. Java: do not log in this function */

    if (mShuttingDown)
        return false;

    mLoopWaitSe = true;

    while (mLoopWaitSe) {
        if (checkSePresence())
            return true;

        try {
            Thread::sleep(10);
        } catch (const InterruptedException& e) {
            (void)e;
        }
    }

    return false;
}

void StubReaderImpl::stopWaitForCard()
{
    mLogger->trace("[%] stopping waitForCard - setting mLoopWaitSe to false\n", getName());
    mLoopWaitSe = false;
}

bool StubReaderImpl::waitForCardAbsentNative()
{
    /* C++ vs. Java: do not log in this function */

    if (mShuttingDown)
        return false;

    mLoopWaitSeRemoval = true;

    while (mLoopWaitSeRemoval) {
        if (!checkSePresence())
            return true;

        try {
            Thread::sleep(10);
        } catch (const InterruptedException& e) {
            (void)e;
        }
    }

    return false;
}

void StubReaderImpl::stopWaitForCardRemoval()
{
    mLoopWaitSeRemoval = false;
}

std::shared_ptr<ObservableReaderStateService> StubReaderImpl::initStateService()
{
    if (mExecutorService == nullptr)
        throw IllegalArgumentException("Executor service has not been initialized");

    std::map<MonitoringState, std::shared_ptr<AbstractObservableState>> states;

    states.insert(
        std::pair<MonitoringState, std::shared_ptr<AbstractObservableState>>(
            MonitoringState::WAIT_FOR_START_DETECTION,
            std::make_shared<WaitForStartDetect>(this)));

    states.insert(
        std::pair<MonitoringState, std::shared_ptr<AbstractObservableState>>(
            MonitoringState::WAIT_FOR_SE_INSERTION,
            std::make_shared<WaitForSeInsertion>(
                this, std::make_shared<SmartInsertionMonitoringJob>(this),
                mExecutorService)));

    states.insert(
        std::pair<MonitoringState, std::shared_ptr<AbstractObservableState>>(
            MonitoringState::WAIT_FOR_SE_PROCESSING,
            std::make_shared<WaitForSeProcessing>(
                this, std::make_shared<SmartRemovalMonitoringJob>(this),
                mExecutorService)));

    states.insert(
        std::pair<MonitoringState, std::shared_ptr<AbstractObservableState>>(
            MonitoringState::WAIT_FOR_SE_REMOVAL,
            std::make_shared<WaitForSeRemoval>(
                this, std::make_shared<SmartRemovalMonitoringJob>(this),
                mExecutorService)));

    return std::make_shared<ObservableReaderStateService>(
        this, states, MonitoringState::WAIT_FOR_SE_INSERTION);
}

}
}
}

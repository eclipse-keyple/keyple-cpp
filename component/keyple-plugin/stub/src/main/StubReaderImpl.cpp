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

StubReaderImpl::StubReaderImpl(const std::string& pluginName, const std::string& readerName)
: AbstractObservableLocalReader(pluginName, readerName)
{
    /* Create a executor service with one thread whose name is customized */
    mExecutorService = std::make_shared<MonitoringPool>();

    mStateService = initStateService();
}

StubReaderImpl::StubReaderImpl(const std::string& pluginName,
                               const std::string& name,
                               TransmissionMode transmissionMode)
: StubReaderImpl(pluginName, name)
{
    mTransmissionMode = transmissionMode;
}

StubReaderImpl::~StubReaderImpl()
{
    /* Force threads to stop */
    mShuttingDown = true;

    /*
     * I would rather avoid that, base classes virtual destructors will end up
     * calling stopWaitForCard() and stopWaitForCardRemoval() at a time where
     * the class state is unfortunatly unknown (seems like some class members
     * are already gone. Let's stop them first here, double call is no problem.
     */
    stopWaitForCard();
    while(mLoopWaitSeOngoing)
        Thread::sleep(1);

    stopWaitForCardRemoval();
    while(mLoopWaitSeRemovalOngoing)
        Thread::sleep(1);
}

const std::vector<uint8_t>& StubReaderImpl::getATR()
{
    return mSe->getATR();
}

bool StubReaderImpl::isPhysicalChannelOpen()
{
    return mSe != nullptr && mSe->isPhysicalChannelOpen();
}

void StubReaderImpl::openPhysicalChannel()
{
    if (mSe != nullptr)
        mSe->openPhysicalChannel();
}

void StubReaderImpl::closePhysicalChannel()
{
    if (mSe != nullptr)
        mSe->closePhysicalChannel();
}

std::vector<uint8_t> StubReaderImpl::transmitApdu(const std::vector<uint8_t>& apduIn)
{
    if (mSe == nullptr)
        throw KeypleReaderIOException("No SE available.");

    return mSe->processApdu(apduIn);
}

bool StubReaderImpl::protocolFlagMatches(const std::shared_ptr<SeProtocol> protocolFlag)
{
    bool result;

    if (mSe == nullptr)
        throw KeypleReaderIOException("No SE available.");

    /* Test protocolFlag to check if ATR based protocol filtering is required */
    //if (protocolFlag != null) {
    if (!isPhysicalChannelOpen())
        openPhysicalChannel();

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
    }
    //} else {
    //    // no protocol defined returns true
    //    result = true;
    //}
    return result;
}

bool StubReaderImpl::checkSePresence()
{
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
    if (isPhysicalChannelOpen()) {
        try {
            closePhysicalChannel();
        } catch (KeypleReaderException& e) {
            mLogger->error("Error while closing channel reader. %\n", e.getMessage());
        }
    }

    if (se != nullptr)
        mSe = se;
}

void StubReaderImpl::removeSe()
{
    mLogger->debug("Remove SE %\n", mSe);

    mSe = nullptr;
}

std::shared_ptr<StubSecureElement> StubReaderImpl::getSe()
{
    return mSe;
}

bool StubReaderImpl::waitForCardPresent()
{
    mLoopWaitSe = true;
    mLoopWaitSeOngoing = true;

    mLogger->trace("[%] waitForCardPresent\n", getName());

    while (mLoopWaitSe) {
        if (checkSePresence()) {
            mLogger->trace("[%] card present\n", getName());
            mLoopWaitSeOngoing = false;
            return true;
        }

        try {
            Thread::sleep(10);
        } catch (const InterruptedException& e) {
            mLogger->debug("Sleep was interrupted - %\n", e);
        }
    }

    mLogger->trace("[%] waitForCardPresent - complete\n", getName());
    mLoopWaitSeOngoing = false;
    return false;
    // logger.trace("[{}] no card was inserted", this.getName());
    // return false;
}

void StubReaderImpl::stopWaitForCard()
{
    /* Do not log anything here! */
    mLoopWaitSe = false;
}

bool StubReaderImpl::waitForCardAbsentNative()
{
    mLoopWaitSeRemoval = true;
    mLoopWaitSeRemovalOngoing = true;

    mLogger->trace("[%] waitForCardAbsentNative\n", getName());

    while (mLoopWaitSeRemoval) {
        if (!checkSePresence()) {
            mLogger->trace("[%] card removed\n", getName());
            mLoopWaitSeRemovalOngoing = false;
            return true;
        }

        try {
            Thread::sleep(10);
        } catch (InterruptedException& e) {
            mLogger->debug("Sleep was interrupted - %\n", e);
        }
    }

    mLogger->trace("[%] waitForCardAbsentNative - complete\n", getName());
    mLoopWaitSeRemovalOngoing = false;
    return false;
    // logger.trace("[{}] no card was removed", this.getName());
    // return false;
}

void StubReaderImpl::stopWaitForCardRemoval()
{
    /* Do not log anything here! */
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

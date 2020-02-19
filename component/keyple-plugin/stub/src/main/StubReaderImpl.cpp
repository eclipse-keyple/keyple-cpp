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
#include "ByteArrayUtil.h"
#include "KeypleBaseException.h"
#include "KeypleChannelControlException.h"
#include "KeypleIOReaderException.h"
#include "KeypleReaderException.h"
#include "ObservableReaderStateService.h"
#include "SeProtocol_Import.h"
#include "SmartInsertionMonitoringJob.h"
#include "SmartRemovalMonitoringJob.h"
#include "WaitForStartDetect.h"
#include "WaitForSeInsertion.h"
#include "WaitForSeProcessing.h"
#include "WaitForSeRemoval.h"

/* Plugin - Stub */
#include "StubReaderImpl.h"
#include "StubSecureElement.h"

/* Common */
#include "Thread.h"
#include "InterruptedException.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::plugin::local;
using namespace keyple::core::seproxy::plugin::local::monitoring;
using namespace keyple::core::seproxy::plugin::local::state;
using namespace keyple::core::seproxy::protocol;

StubReaderImpl::StubReaderImpl(
  const std::string& pluginName, const std::string& readerName)
: AbstractObservableLocalReader(pluginName, readerName)
{
    /* Create a executor service with one thread whose name is customized */
    this->executorService = std::make_shared<MonitoringPool>();

    this->stateService = initStateService();
}

StubReaderImpl::StubReaderImpl(
  const std::string& pluginName, const std::string& name,
  TransmissionMode transmissionMode)
: StubReaderImpl(pluginName, name)
{
    this->transmissionMode = transmissionMode;
}

const std::vector<uint8_t>& StubReaderImpl::getATR()
{
    return se->getATR();
}

bool StubReaderImpl::isPhysicalChannelOpen()
{
    return se != nullptr && se->isPhysicalChannelOpen();
}

void StubReaderImpl::openPhysicalChannel()
{
    if (se != nullptr) {
        se->openPhysicalChannel();
    }
}

void StubReaderImpl::closePhysicalChannel()
{
    if (se != nullptr) {
        se->closePhysicalChannel();
    }
}

std::vector<uint8_t> StubReaderImpl::transmitApdu(std::vector<uint8_t>& apduIn)
{
    if (se == nullptr) {
        throw KeypleIOReaderException("No SE available.");
    }

    return se->processApdu(apduIn);
}

bool StubReaderImpl::protocolFlagMatches(const SeProtocol& protocolFlag)
{
    bool result;

    if (se == nullptr) {
        throw KeypleReaderException("No SE available.");
    }

    /* Test protocolFlag to check if ATR based protocol filtering is required */
    //if (protocolFlag != null) {
        if (!isPhysicalChannelOpen()) {
            openPhysicalChannel();
        }

        /*
         * The requestSet will be executed only if the protocol match the
         * requestElement
         */
        const std::string& selectionMask =
            protocolsMap.find(protocolFlag)->second;

        if (selectionMask.empty()) {
            throw KeypleReaderException("Target selector mask not found!");
        }

        Pattern* p = Pattern::compile(selectionMask);
        const std::string& protocol = se->getSeProcotol();
        if (!p->matcher(protocol)->matches()) {
            logger->trace("[%s] protocolFlagMatches => unmatching SE. " \
                          "PROTOCOLFLAG = %s\n", this->getName(),
                          protocolFlag.toString());
            result = false;
        } else {
            logger->trace("[%s] protocolFlagMatches => matching SE. " \
                          "PROTOCOLFLAG = %s\n", this->getName().c_str(),
                          protocolFlag.toString().c_str());
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
    return se != nullptr;
}

void StubReaderImpl::setParameter(const std::string& name,
                                  const std::string& value)
{
    if (!name.compare(StubReader::ALLOWED_PARAMETER_1) ||
        !name.compare(StubReader::ALLOWED_PARAMETER_2)) {
        parameters.insert(
            std::pair<const std::string, const std::string>(name, value));
    } else if (!name.compare(StubReader::CONTACTS_PARAMETER)) {
        transmissionMode = TransmissionMode::CONTACTS;
    } else if (!name.compare(StubReader::CONTACTLESS_PARAMETER)) {
        transmissionMode = TransmissionMode::CONTACTLESS;
    } else {
        throw KeypleReaderException("parameter name not supported : " + name);
    }
}

const std::map<const std::string, const std::string>
    StubReaderImpl::getParameters()
{
    return parameters;
}

TransmissionMode StubReaderImpl::getTransmissionMode()
{
    return transmissionMode;
}

void StubReaderImpl::insertSe(std::shared_ptr<StubSecureElement> _se)
{
    logger->debug("Insert SE %s\n", "_se<fixme>");

    /* Clean channels status */
    if (isPhysicalChannelOpen()) {
        try {
            closePhysicalChannel();
        } catch (KeypleReaderException& e) {
            logger->error("Error while closing channel reader. %s\n",
                          e.getMessage());
        }
    }
    if (_se != nullptr) {
        se = _se;
    }
}

void StubReaderImpl::removeSe()
{
    logger->debug("Remove SE %s\n", se != nullptr ? "se<fixme>" : "none");

    se = nullptr;
}

std::shared_ptr<StubSecureElement> StubReaderImpl::getSe()
{
    return se;
}

bool StubReaderImpl::waitForCardPresent()
{
    loopWaitSe = true;

    while (loopWaitSe) {
        if (checkSePresence()) {
            return true;
        }

        try {
            Thread::sleep(10);
        } catch (InterruptedException& e) {
            logger->debug("Sleep was interrupted\n");
        }
    }
    return false;
    // logger.trace("[{}] no card was inserted", this.getName());
    // return false;
}

void StubReaderImpl::stopWaitForCard()
{
    loopWaitSe = false;
}

bool StubReaderImpl::waitForCardAbsentNative()
{
    loopWaitSeRemoval = true;

    while (loopWaitSeRemoval) {
        if (!checkSePresence()) {
            logger->trace("[%s] card removed\n", this->getName());
            return true;
        }

        try {
            Thread::sleep(10);
        } catch (InterruptedException& e) {
            logger->debug("Sleep was interrupted\n");
        }
    }
    return false;
    // logger.trace("[{}] no card was removed", this.getName());
    // return false;
}

void StubReaderImpl::stopWaitForCardRemoval()
{
    loopWaitSeRemoval = false;
}

std::shared_ptr<ObservableReaderStateService> StubReaderImpl::initStateService()
{
    if (executorService == nullptr) {
        throw IllegalArgumentException("Executor service has not been " \
                                       "initialized");
    }

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
                executorService)));

    states.insert(
        std::pair<MonitoringState, std::shared_ptr<AbstractObservableState>>(
            MonitoringState::WAIT_FOR_SE_PROCESSING,
            std::make_shared<WaitForSeProcessing>(
                this, std::make_shared<SmartRemovalMonitoringJob>(this),
                executorService)));

    states.insert(
        std::pair<MonitoringState, std::shared_ptr<AbstractObservableState>>(
            MonitoringState::WAIT_FOR_SE_REMOVAL,
            std::make_shared<WaitForSeRemoval>(
                this, std::make_shared<SmartRemovalMonitoringJob>(this),
                executorService)));

    return std::make_shared<ObservableReaderStateService>(
               this, states, MonitoringState::WAIT_FOR_SE_INSERTION);
}

void StubReaderImpl::addObserver(std::shared_ptr<ReaderObserver> observer)
{
    AbstractObservableLocalReader::addObserver(observer);
}

void StubReaderImpl::removeObserver(std::shared_ptr<ReaderObserver> observer)
{
    AbstractObservableLocalReader::removeObserver(observer);
}

void StubReaderImpl::startSeDetection(PollingMode pollingMode)
{
    AbstractObservableLocalReader::startSeDetection(pollingMode);
}

void StubReaderImpl::setDefaultSelectionRequest(
    std::shared_ptr<AbstractDefaultSelectionsRequest>
    defaultSelectionsRequest, NotificationMode notificationMode)
{
    AbstractObservableLocalReader::setDefaultSelectionRequest(
        defaultSelectionsRequest, notificationMode);
}

}
}
}

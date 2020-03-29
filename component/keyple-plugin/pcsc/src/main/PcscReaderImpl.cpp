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

#include "PcscReaderImpl.h"

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

/* PC/SC Plugin */
#include "PcscTerminalException.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::plugin::local;
using namespace keyple::core::seproxy::plugin::local::monitoring;
using namespace keyple::core::seproxy::plugin::local::state;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

const std::string PcscReaderImpl::PROTOCOL_T0   = "T=0";
const std::string PcscReaderImpl::PROTOCOL_T1   = "T=1";
const std::string PcscReaderImpl::PROTOCOL_T_CL = "T=CL";
const std::string PcscReaderImpl::PROTOCOL_ANY  = "T=0";

PcscReaderImpl::PcscReaderImpl(const std::string& pluginName,
                               PcscTerminal& terminal)
: AbstractObservableLocalReader(pluginName, terminal.getName()),
  terminal(terminal)
{
    this->executorService = std::make_shared<MonitoringPool>();
    this->stateService    = initStateService();

    logger->debug("[PcscReaderImpl] constructor => using terminal %s\n",
                  terminal.getName().c_str());

    /*
     * Using null values to use the standard method for defining default values
     *
     * /!\ Must use hardcoded strings here as
     *     PcscReader::SETTING_KEY_TRANSMISSION_MODE and the three other strings
     *     are not yet instantiated.
     */
    try {
        setParameter("transmission_mode", "");
        setParameter("protocol", "");
        setParameter("mode", "");
        setParameter("disconnect", "");
    } catch (KeypleBaseException& e) {
        (void)e;
        /* Can not fail with null value */
    }
}

PcscReaderImpl::PcscReaderImpl(const PcscReaderImpl& o)
: Nameable(), Configurable(),
  AbstractObservableLocalReader(o.pluginName, o.terminal.getName()),
  executorService(o.executorService), terminal(o.terminal),
  parameterCardProtocol(o.parameterCardProtocol),
  cardExclusiveMode(o.cardExclusiveMode), cardReset(o.cardReset),
  transmissionMode(o.transmissionMode)
{
    this->stateService = o.stateService;
}

std::shared_ptr<ObservableReaderStateService> PcscReaderImpl::initStateService()
{
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
        this, states, MonitoringState::WAIT_FOR_START_DETECTION);
}

void PcscReaderImpl::closePhysicalChannel()
{
    try {
        logger->debug("[%s] closePhysicalChannel\n", this->getName().c_str());
        terminal.closeAndDisconnect(cardReset);

    } catch (PcscTerminalException& e) {
        throw KeypleChannelControlException("Error closing physical channel",
                                            e);
    }

    this->channelOpen = false;
}

bool PcscReaderImpl::checkSePresence()
{
    try {
        logger->debug("checkSePresence - calling isCardPresent\n");
        return terminal.isCardPresent(true);

    } catch (PcscTerminalException& e) {
        logger->debug("[%s] checkSePresence - caught PcscTerminalException "
                      " (msg: %s, cause: %ds\n",
                      terminal.getName().c_str(), e.getMessage().c_str(),
                      e.getCause().what());
    }

    return false;
}

bool PcscReaderImpl::waitForCardPresent()
{
    logger->debug("[%s] waitForCardPresent => loop with latency of %d ms\n",
                  this->getName().c_str(), insertLatency);

    /* Activate loop */
    loopWaitSe = true;

    try {
        while (loopWaitSe) {
            logger->trace("[%s] waitForCardPresent => looping\n",
                          this->getName().c_str());
            if (terminal.waitForCardPresent(insertLatency)) {
                /* Card inserted */
                return true;
            } else {
                logger->debug("FIXME. Do we need to handle a thread"
                              "interruption here?\n");
                /*                if (Thread.interrupted()) {
                    logger->debug("[%s] waitForCardPresent => task has been " \
                                  cancelled\n", this->getName());
*/                    /* Task has been cancelled */
                //return false;
                //                  }
            }
        }

        /* If loop was stopped */
        return false;

    } catch (PcscTerminalException& e) {
        throw KeypleIOReaderException(StringHelper::formatSimple(
            "[%s] Exception occurred in waitForCardPresent. "
            "Message: %s",
            this->getName(), e.getMessage()));
    }
}

void PcscReaderImpl::stopWaitForCard()
{
    loopWaitSe = false;
}

bool PcscReaderImpl::waitForCardAbsentNative()
{
    logger->debug("[%s] waitForCardAbsentNative => loop with latency of "
                  "%d ms\n", this->getName().c_str(), removalLatency);

    loopWaitSeRemoval = true;

    try {
        while (loopWaitSeRemoval) {
            logger->trace("[%s] waitForCardAbsentNative => looping\n",
                          this->getName().c_str());
            if (terminal.waitForCardAbsent(removalLatency)) {
                /* Card removed */
                return true;
            } else {
                logger->debug("FIXME. Do we need to handle a thread"
                              "interruption here?\n");
                /*                    if (Thread.interrupted()) {
                    logger->debug("[%s] waitForCardAbsentNative => task " \
                                    "has been cancelled\n", this->getName());
*/                        /* Task has been cancelled */
                //return false;
                //                    }
            }
        }

        return false;

    } catch (PcscTerminalException& e) {
        throw KeypleIOReaderException(StringHelper::formatSimple(
            "[%s] Exception occurred in waitForCardAbsentNa)tive. "
            "Message: %s",
            this->getName(), e.getMessage()));
    }
}

void PcscReaderImpl::stopWaitForCardRemoval()
{
    loopWaitSeRemoval = false;
}

std::vector<uint8_t> PcscReaderImpl::transmitApdu(
    const std::vector<uint8_t>& apduIn)
{
    logger->debug("transmitApdu\n");

    std::vector<uint8_t> response;

    try {
        response = terminal.transmitApdu(apduIn);
    } catch (PcscTerminalException& e) {
        logger->error("transmitApdu - caught PcscTerminalException (msg: %s, "
                      "cause: %s)\n",
                      e.getMessage().c_str(), e.getCause().what());
        throw KeypleIOReaderException("transmitApdu failed", e);
    } catch (std::invalid_argument& e) {
        /* Card could have been removed prematurely */
        logger->error("transmitApdu - caught std::invalid_argument (cause : "
                      "%s)\n",
                      e.what());
        throw KeypleIOReaderException("transmitApdu failed", e);
    }

    return response;
}

bool PcscReaderImpl::protocolFlagMatches(const SeProtocol& protocolFlag)
{
    bool result;

    /* Test protocolFlag to check if ATR based protocol filtering is required */
    //if (protocolFlag != nullptr) {
    if (!isPhysicalChannelOpen()) {
        logger->debug("protocolFlagMatches - physical channel not open, "
                      "opening it\n");
        openPhysicalChannel();
    }

    logger->debug("protocolFlagMatches - going through protocols map\n");
    std::map<SeProtocol, std::string>::iterator it = protocolsMap.begin();
    while (it != protocolsMap.end()) {
        SeProtocol p = it->first;
        logger->debug("available protocol: %s - %s\n", p.getName().c_str(),
                      it->second.c_str());
        it++;
    }

    /*
         * The requestSet will be executed only if the protocol match the
         * requestElement.
         */
    std::string selectionMask = protocolsMap[protocolFlag];
    logger->debug("protocolFlagMatches - selectionMask: %s\n",
                  selectionMask.c_str());
    if (selectionMask == "") {
        throw KeypleReaderException(
            "Target selector mask not found!"); // nullptr));
    }

    Pattern* p      = Pattern::compile(selectionMask);
    std::string atr = ByteArrayUtil::toHex(this->terminal.getATR());
    if (!p->matcher(atr)->matches()) {
        logger->debug("[%s] protocolFlagMatches => unmatching SE. "
                      "PROTOCOLFLAG = %s\n",
                      this->getName().c_str(), protocolFlag.toString().c_str());
        result = false;
    } else {
        logger->debug("[%s] protocolFlagMatches => matching SE. "
                      "PROTOCOLFLAG = %s\n",
                      this->getName().c_str(), protocolFlag.toString().c_str());
        result = true;
    }
    //} else {
    //    // no protocol defined returns true
    //    result = true;
    //}

    return result;
}

void PcscReaderImpl::setParameter(const std::string& name,
                                  const std::string& value)
{    logger->debug("[%s] setParameter => PCSC: Set a parameter. NAME = %s,"
                  " VALUE = %s\n",
                  this->getName().c_str(), name.c_str(), value.c_str());

    if (name == "") {
        throw std::invalid_argument("Parameter shouldn't be null\n");
    }

    /*
     * /!\ Must use hardcoded strings here as
     *     PcscReader::SETTING_KEY_TRANSMISSION_MODE and the three other strings
     *     are not yet instantiated.
     */
    if (name == "transmission_mode") {
        if (value == "")
            transmissionMode = static_cast<TransmissionMode>(0);
        else if (value == "contacts")
            transmissionMode = TransmissionMode::CONTACTS;
        else if (value == "contactless")
            transmissionMode = TransmissionMode::CONTACTLESS;
        else
            throw std::invalid_argument("Bad tranmission mode " + name + " : " +
                                        value);

    } else if (name == "protocol") {
        if (value == "" || value == "Tx")
            parameterCardProtocol = "*";
        else if (value == "T0")
            parameterCardProtocol = "T=0";
        else if (value == "T1")
            parameterCardProtocol = "T=1";
        else if (value == "TCL")
            parameterCardProtocol = "T=CL";
        else
            throw std::invalid_argument("Bad protocol " + name + " : " + value);

    } else if (name == "mode") {
        if (value == "" || value == "shared") {
            if (cardExclusiveMode) {
                try {
                    terminal.endExclusive();
                } catch (PcscTerminalException& e) {
                    throw KeypleReaderException("Couldn't disable exclusive "
                                                "mode",
                                                e);
                }
            }
            cardExclusiveMode = false;
        } else if (value == "exclusive") {
            cardExclusiveMode = true;
        } else {
            throw std::invalid_argument("Parameter value not supported " +
                                        name + " : " + value);
        }
    } else if (name == "disconnect") {
        if (value == "" || value == "reset") {
            cardReset = true;
        } else if (value == "unpower") {
            cardReset = false;
        } else if (value == "eject" ||
                   value == "leave") {
            throw std::invalid_argument("This disconnection parameter is not "
                                        "supported by this plugin" +
                                        name + " : " + value);
        } else {
            throw std::invalid_argument("Parameters not supported : " + name +
                                        " : " + value);
        }
    } else {
        throw std::invalid_argument("This parameter is unknown! " + name +
                                    " : " + value);
    }
}

const std::map<const std::string, const std::string>
PcscReaderImpl::getParameters()
{
    std::map<const std::string, const std::string> parameters;

    /* Returning the protocol */
    std::string protocol = parameterCardProtocol;
    if (protocol == "*") {
        protocol = SETTING_PROTOCOL_TX;
    } else if (protocol == "T=0") {
        protocol = SETTING_PROTOCOL_T0;
    } else if (protocol == "T=1") {
        protocol = SETTING_PROTOCOL_T1;
    } else {
        throw IllegalStateException("Illegal protocol: " + protocol);
    }

    parameters.emplace(SETTING_KEY_PROTOCOL, protocol);

    /* The mode ? */
    if (!cardExclusiveMode) {
        parameters.emplace(SETTING_KEY_MODE, SETTING_MODE_SHARED);
    }

    return parameters;
}

const std::vector<uint8_t>& PcscReaderImpl::getATR()
{
    return terminal.getATR();
}

bool PcscReaderImpl::isPhysicalChannelOpen()
{
    return this->channelOpen;
}

void PcscReaderImpl::openPhysicalChannel()
{
    /*
     *  Init of the physical SE channel: if not yet established, opening of a
     * new physical channel
     */
    try {
        this->terminal.openAndConnect(parameterCardProtocol);
        if (cardExclusiveMode) {
            this->terminal.beginExclusive();
            logger->debug("[%s] Opening of a physical SE channel in "
                          "exclusive mode\n",
                          this->getName().c_str());
        } else {
            logger->debug("[%s] Opening of a physical SE channel in "
                          "shared mode\n",
                          this->getName().c_str());
        }
    } catch (PcscTerminalException& e) {
        throw KeypleChannelControlException(
            "Error while opening Physical Channel", e);
    }

    this->channelOpen = true;
}

TransmissionMode PcscReaderImpl::getTransmissionMode()
{
    if (transmissionMode != static_cast<TransmissionMode>(0)) {
        return transmissionMode;
    } else {
        if (!parameterCardProtocol.compare(PROTOCOL_T1) ||
            !parameterCardProtocol.compare(PROTOCOL_T_CL)) {
            return TransmissionMode::CONTACTLESS;
        } else {
            return TransmissionMode::CONTACTS;
        }
    }
}

void PcscReaderImpl::notifyObservers(std::shared_ptr<ReaderEvent> event)
{
    AbstractObservableLocalReader::notifyObservers(event);
}

void PcscReaderImpl::addObserver(std::shared_ptr<ReaderObserver> observer)
{
    AbstractObservableLocalReader::addObserver(observer);
}

void PcscReaderImpl::removeObserver(std::shared_ptr<ReaderObserver> observer)
{
    AbstractObservableLocalReader::removeObserver(observer);
}

void PcscReaderImpl::startSeDetection(PollingMode pollingMode)
{
    AbstractObservableLocalReader::startSeDetection(pollingMode);
}

void PcscReaderImpl::stopSeDetection()
{
    AbstractObservableLocalReader::stopSeDetection();
}

void PcscReaderImpl::setDefaultSelectionRequest(
    std::shared_ptr<AbstractDefaultSelectionsRequest> defaultSelectionsRequest,
    NotificationMode notificationMode)
{
    AbstractObservableLocalReader::setDefaultSelectionRequest(
        defaultSelectionsRequest, notificationMode);
}

void PcscReaderImpl::setDefaultSelectionRequest(
    std::shared_ptr<AbstractDefaultSelectionsRequest> defaultSelectionsRequest,
    NotificationMode notificationMode, PollingMode pollingMode)
{
    AbstractObservableLocalReader::setDefaultSelectionRequest(
        defaultSelectionsRequest, notificationMode, pollingMode);
}

void PcscReaderImpl::clearObservers()
{
    AbstractObservableLocalReader::clearObservers();
}

void PcscReaderImpl::notifySeProcessed()
{
    AbstractObservableLocalReader::notifySeProcessed();
}

}
}
}

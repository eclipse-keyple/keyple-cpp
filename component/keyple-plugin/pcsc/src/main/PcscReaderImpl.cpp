/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

/* Common */
#include "IllegalArgumentException.h"
#include "IllegalStateException.h"

/* Core */
#include "ByteArrayUtil.h"
#include "ObservableReaderStateService.h"
#include "SeProtocol.h"
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

using namespace keyple::common;
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
  mLoopWaitSeRemoval(true), mTerminal(terminal),
  mTransmissionMode(TransmissionMode::NONE)
{
    mExecutorService = std::make_shared<MonitoringPool>();
    mStateService    = initStateService();

    mLogger->debug("[PcscReaderImpl] constructor => using terminal %\n",
                  terminal.getName());

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
    } catch (KeypleException& e) {
        (void)e;
        /* Can not fail with null value */
    }
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
        this, states, MonitoringState::WAIT_FOR_START_DETECTION);
}

void PcscReaderImpl::closePhysicalChannel()
{
    try {
        mLogger->debug("[%] closePhysicalChannel\n", getName());
        mTerminal.closeAndDisconnect(mCardReset);

    } catch (PcscTerminalException& e) {
        throw KeypleReaderIOException("Error while closing physical channel",e);
    }

    mChannelOpen = false;
}

bool PcscReaderImpl::checkSePresence()
{
    try {
        mLogger->debug("checkSePresence - calling isCardPresent\n");
        return mTerminal.isCardPresent(true);

    } catch (PcscTerminalException& e) {
        throw KeypleReaderIOException("Exception occurred in isSePresent", e);
    }

    return false;
}

bool PcscReaderImpl::waitForCardPresent()
{
    mLogger->debug("[%] waitForCardPresent => loop with latency of % ms\n",
                  getName(), INSERT_LATENCY);

    /* Activate loop */
    mLoopWaitSe = true;

    /* Rearm flag for removal process as well */
    mLoopWaitSeRemoval = true;

    try {
        mLogger->trace("[%] waitForCardPresent => looping...\n", getName());
        while (mLoopWaitSe) {
            if (mTerminal.waitForCardPresent(INSERT_LATENCY)) {
                /* Card inserted */
                 mLogger->debug("[%] waitForCardPresent => card inserted\n",
                               getName());
                return true;
            } else {
                /* Check if task has been cancelled ? */
            }
        }

        /* If loop was stopped */
        return false;

    } catch (PcscTerminalException& e) {
        throw KeypleReaderIOException(
                  "[" + getName() + "] Exception occurred in " \
                  "waitForCardPresent. Message: " + e.getMessage());
    }
}

void PcscReaderImpl::stopWaitForCard()
{
    mLoopWaitSe = false;
}

bool PcscReaderImpl::waitForCardAbsentNative()
{
    mLogger->debug("[%] waitForCardAbsentNative => loop with latency of "
                  "% ms\n", getName(), REMOVAL_LATENCY);

    while(!mLoopWaitSeRemoval)
        /* A stopWaitForCardRemoval() has been triggered. Wait for finish. */
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    try {
        while (mLoopWaitSeRemoval) {
            mLogger->trace("[%] waitForCardAbsentNative => looping\n",getName());
            if (mTerminal.waitForCardAbsent(REMOVAL_LATENCY)) {
                /* notify removal only if expected */
                if(mLoopWaitSeRemoval) {
                    /* Card removed */
                    return true;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        mLoopWaitSeRemoval = true;
        return false;

    } catch (PcscTerminalException& e) {
        throw KeypleReaderIOException(
                  "[" + getName() + "] Exception occurred in " \
                  "waitForCardAbsentNative. Message: " + e.getMessage());
    }
}

void PcscReaderImpl::stopWaitForCardRemoval()
{
    mLoopWaitSeRemoval = false;
}

std::vector<uint8_t> PcscReaderImpl::transmitApdu(
    const std::vector<uint8_t>& apduIn)
{
    mLogger->debug("transmitApdu\n");

    std::vector<uint8_t> response;

    try {
        response = mTerminal.transmitApdu(apduIn);
    } catch (PcscTerminalException& e) {
        mLogger->error("transmitApdu - PcscTerminalException %\n", e);
        throw KeypleReaderIOException("transmitApdu failed", e);
    } catch (std::invalid_argument& e) {
        /* Card could have been removed prematurely */
        mLogger->error("transmitApdu - std::invalid_argument %\n", e.what());
        throw KeypleReaderIOException("transmitApdu failed", e);
    }

    return response;
}

bool PcscReaderImpl::protocolFlagMatches(
    const std::shared_ptr<SeProtocol> protocolFlag)
{
    bool result;

    /* Test protocolFlag to check if ATR based protocol filtering is required */
    if (protocolFlag) {
        if (!isPhysicalChannelOpen()) {
            mLogger->debug("protocolFlagMatches - physical channel not open, "
                        "opening it\n");
            openPhysicalChannel();
        }

        /*
         * The request will be executed only if the protocol match the
         * requestElement
         */
        std::string selectionMask = mProtocolsMap[protocolFlag];
        mLogger->debug("protocolFlagMatches - selectionMask: %\n", selectionMask);
        if (selectionMask == "") {
            throw KeypleReaderIOException(
                "Target selector mask not found!"); // nullptr));
        }

        Pattern* p      = Pattern::compile(selectionMask);
        std::string atr = ByteArrayUtil::toHex(mTerminal.getATR());
        if (!p->matcher(atr)->matches()) {
            mLogger->debug("[%] protocolFlagMatches => unmatching SE. "
                        "PROTOCOLFLAG = %\n", getName(), protocolFlag);
            result = false;
        } else {
            mLogger->debug("[%] protocolFlagMatches => matching SE. "
                        "PROTOCOLFLAG = %\n", getName(), protocolFlag);
            result = true;
        }
    } else {
        /* No protocol defined returns true */
        result = true;
    }

    return result;
}

void PcscReaderImpl::setParameter(const std::string& name,
                                  const std::string& value)
{    mLogger->debug("[%] setParameter => PCSC: Set a parameter. NAME = %,"
                  " VALUE = %\n", getName(), name, value);

    if (name == "") {
        throw std::invalid_argument("Parameter shouldn't be null\n");
    }

    /*
     * /!\ Must use hardcoded strings here as
     *     PcscReader::SETTING_KEY_TRANSMISSION_MODE and the three other strings
     *     are not yet instantiated.
     */
    if (name == PcscReader::SETTING_KEY_TRANSMISSION_MODE) {
        if (value == "")
            mTransmissionMode = TransmissionMode::NONE;
        else if (value == "contacts")
            mTransmissionMode = TransmissionMode::CONTACTS;
        else if (value == "contactless")
            mTransmissionMode = TransmissionMode::CONTACTLESS;
        else
            throw IllegalArgumentException(
                      "Bad tranmission mode " + name +  " : " + value);

    } else if (name == PcscReader::SETTING_KEY_PROTOCOL) {

        /* /!\ C++ vs. Java - Clear parameters table */
        mParameters.clear();

        if (value == "" || value == "Tx")
            mParameterCardProtocol = "*";
        else if (value == PcscReader::SETTING_PROTOCOL_T0)
            mParameterCardProtocol = "T=0";
        else if (value == PcscReader::SETTING_PROTOCOL_T1)
            mParameterCardProtocol = "T=1";
        else if (value == PcscReader::SETTING_PROTOCOL_T_CL)
            mParameterCardProtocol = "T=CL";
        else
            throw IllegalArgumentException(
                      "Bad protocol " + name + " : " + value);

        /* /!\ C++ vs. Java */
        mParameters.emplace(SETTING_KEY_PROTOCOL, mParameterCardProtocol);

        /*
         * /!\ C++ vs. Java
         *
         * Actualize 'transmissionMode' according to 'parameterCardProtocol'.
         */
        if (mParameterCardProtocol == PROTOCOL_T1 ||
            mParameterCardProtocol == PROTOCOL_T_CL) {
            mTransmissionMode = TransmissionMode::CONTACTLESS;
        } else {
            mTransmissionMode = TransmissionMode::CONTACTS;
        }

    } else if (name == PcscReader::SETTING_KEY_MODE) {
        if (value == "" || value == PcscReader::SETTING_MODE_SHARED) {
            if (mCardExclusiveMode) {
                try {
                    mTerminal.endExclusive();
                } catch (PcscTerminalException& e) {
                    throw KeypleReaderIOException("Couldn't disable exclusive "
                                                  "mode", e);
                }
            }
            mCardExclusiveMode = false;

            /* /!\ C++ vs. Java */
            mParameters.emplace(SETTING_KEY_MODE, SETTING_MODE_SHARED);

        } else if (value == PcscReader::SETTING_MODE_EXCLUSIVE) {
            mCardExclusiveMode = true;
        } else {
            throw IllegalArgumentException(
                      "Parameter value not supported " + name + " : " + value);
        }


    } else if (name == PcscReader::SETTING_KEY_DISCONNECT) {
        if (value == "" || value == PcscReader::SETTING_DISCONNECT_RESET) {
            mCardReset = true;
        } else if (value == PcscReader::SETTING_DISCONNECT_UNPOWER) {
            mCardReset = false;
        } else if (value == PcscReader::SETTING_DISCONNECT_EJECT ||
                   value == PcscReader::SETTING_DISCONNECT_LEAVE) {
            throw IllegalArgumentException(
                      "This disconnection parameter is not supported by this " \
                      "plugin" + name + " : " + value);
        } else {
            throw IllegalArgumentException(
                      "Parameters not supported : " + name + " : " + value);
        }
    } else {
        throw IllegalArgumentException(
                  "This parameter is unknown! " + name + " : " + value);
    }
}

const std::map<const std::string, const std::string>&
    PcscReaderImpl::getParameters() const
{
    /*
     * /!\ C++ vs. Java: This code has been integrated to the setParameter()
     *                   function. This avoids changing the function prototype
     *                   (either const or ref should be removed). Also it makes
     *                   more sense to update the class in a setter function
     *                   than a getter.
     */
//     /* Returning the protocol */
//     std::string protocol = parameterCardProtocol;
//     if (protocol == "*") {
//         protocol = SETTING_PROTOCOL_TX;
//     } else if (protocol == "T=0") {
//         protocol = SETTING_PROTOCOL_T0;
//     } else if (protocol == "T=1") {
//         protocol = SETTING_PROTOCOL_T1;
//     } else {
//         throw IllegalStateException("Illegal protocol: " + protocol);
//     }

//     parameters.emplace(SETTING_KEY_PROTOCOL, protocol);

//     /* The mode ? */
//     if (!cardExclusiveMode) {
//         parameters.emplace(SETTING_KEY_MODE, SETTING_MODE_SHARED);
//     }

    return mParameters;
}

const std::vector<uint8_t>& PcscReaderImpl::getATR()
{
    return mTerminal.getATR();
}

bool PcscReaderImpl::isPhysicalChannelOpen()
{
    return mChannelOpen;
}

void PcscReaderImpl::openPhysicalChannel()
{
    /*
     *  Init of the physical SE channel: if not yet established, opening of a
     * new physical channel
     */
    try {
        mTerminal.openAndConnect(mParameterCardProtocol);
        if (mCardExclusiveMode) {
            mTerminal.beginExclusive();
            mLogger->debug("[%] Opening of a physical SE channel in "
                          "exclusive mode\n", getName());
        } else {
            mLogger->debug("[%] Opening of a physical SE channel in "
                          "shared mode\n", getName());
        }
    } catch (PcscTerminalException& e) {
        throw KeypleReaderIOException(
            "Error while opening Physical Channel", e);
    }

    mChannelOpen = true;
}

const TransmissionMode& PcscReaderImpl::getTransmissionMode() const
{
    /*
     * /!\ Java diff
     *
     * getTransmissionMode() is Java can either return the actual value of
     * 'transmissionMode' or a value deducted from 'parameterCardProtocol' if
     * not set.
     *
     * This is a bit deceiving and 'transmissionMode' could never be set
     * correctly. Instead, I made sure 'transmissionMode' is set correctly
     * whenever 'parameterCardProtocol' changes (e.g. in setParameter())
     *
     * This allows getTranmissionMode() to be 'const', which seems right for a
     * 'getter'.
     */

    return mTransmissionMode;
}

}
}
}

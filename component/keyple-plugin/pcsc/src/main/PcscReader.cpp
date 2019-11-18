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
#include "KeypleChannelStateException.h"
#include "KeypleIOReaderException.h"
#include "KeypleReaderException.h"
#include "SeProtocol_Import.h"

/* PC/SC Plugin */
#include "PcscReader.h"
#include "PcscTerminalException.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

const std::shared_ptr<Logger> logger =
    LoggerFactory::getLogger(typeid(PcscReader));

const std::string PcscReader::SETTING_KEY_TRANSMISSION_MODE =
    "transmission_mode";
const std::string PcscReader::SETTING_TRANSMISSION_MODE_CONTACTS =
    "contacts";
const std::string PcscReader::SETTING_TRANSMISSION_MODE_CONTACTLESS =
    "contactless";
const std::string PcscReader::SETTING_KEY_PROTOCOL = "protocol";
const std::string PcscReader::SETTING_PROTOCOL_T0 = "T0";
const std::string PcscReader::SETTING_PROTOCOL_T1 = "T1";
const std::string PcscReader::SETTING_PROTOCOL_T_CL = "TCL";
const std::string PcscReader::SETTING_PROTOCOL_TX = "Tx";
const std::string PcscReader::SETTING_KEY_MODE = "mode";
const std::string PcscReader::SETTING_MODE_EXCLUSIVE = "exclusive";
const std::string PcscReader::SETTING_MODE_SHARED = "shared";
const std::string PcscReader::SETTING_KEY_DISCONNECT = "disconnect";
const std::string PcscReader::SETTING_DISCONNECT_RESET = "reset";
const std::string PcscReader::SETTING_DISCONNECT_UNPOWER = "unpower";
const std::string PcscReader::SETTING_DISCONNECT_LEAVE = "leave";
const std::string PcscReader::SETTING_DISCONNECT_EJECT = "eject";
const std::string PcscReader::SETTING_KEY_THREAD_TIMEOUT =
    "thread_wait_timeout";
const std::string PcscReader::SETTING_KEY_LOGGING = "logging";
const std::string PcscReader::PROTOCOL_T0 = "T=0";
const std::string PcscReader::PROTOCOL_T1 = "T=1";
const std::string PcscReader::PROTOCOL_T_CL = "T=CL";
const std::string PcscReader::PROTOCOL_ANY = "T=0";

PcscReader::PcscReader(
  const std::string &pluginName, PcscTerminal& terminal)
: AbstractThreadedLocalReader(pluginName, terminal.getName()),
  terminal(terminal)
{
    logger->debug("PcscReader::PcscReader pluginName: %s, terminal: %s\n",
                  pluginName, terminal.getName());

    this->protocolsMap = std::unordered_map<SeProtocol, std::string>();

    /*
     * Using null values to use the standard method for defining default values
     */
    try {
        setParameter(SETTING_KEY_TRANSMISSION_MODE, "");
        setParameter(SETTING_KEY_PROTOCOL, "");
        setParameter(SETTING_KEY_MODE, "");
        setParameter(SETTING_KEY_DISCONNECT, "");
        setParameter(SETTING_KEY_LOGGING, "");
    } catch (KeypleBaseException& e) {

        logger->trace("[%s] PcscReader - caught KeypleBaseException (msg: %s," \
                      " cause: %s\n", pluginName, e.getMessage(),
                      e.getCause().what());
    }
}

PcscReader::PcscReader(const PcscReader& o)
: Nameable(),
  AbstractThreadedLocalReader(o.pluginName, o.terminal.getName()),
  terminal(o.terminal), parameterCardProtocol(o.parameterCardProtocol),
  cardExclusiveMode(o.cardExclusiveMode), cardReset(o.cardReset),
  transmissionMode(o.transmissionMode)

{
}

void PcscReader::closePhysicalChannel()
{
    try {
        logger->trace("[%s] closePhysicalChannel\n", this->getName());
        terminal.closeAndDisconnect(cardReset);

    } catch (PcscTerminalException &e) {
        throw KeypleChannelStateException("Error closing physical channel", e);
    }

    this->channelOpen = false;
}

bool PcscReader::checkSePresence()
{
    try {
	    logger->debug("checkSePresence - calling isCardPresent\n");
        return terminal.isCardPresent();

    } catch (PcscTerminalException &e) {
        logger->trace("[%s] checkSePresence - caught PcscTerminalException " \
                      " (msg: %s, cause: %ds\n", terminal.getName(),
                      e.getMessage(), e.getCause().what());
    }

    return false;
}

bool PcscReader::waitForCardPresent(long long timeout)
{
    logger->debug("waitForCardPresent - timeout: %d\n", timeout);

    try {
        return terminal.waitForCardPresent(timeout);

    } catch (PcscTerminalException &e) {
        logger->trace("[%s] waitForCardPresent - caught PcscTerminalException " \
                      " (msg: %s, cause: %ds\n", terminal.getName(),
                      e.getMessage(), e.getCause().what());
    }

    return false;
}

bool PcscReader::waitForCardAbsent(long long timeout)
{
    logger->debug("waitForCardAbsent - timeout: %d\n", timeout);

    try {
        if (terminal.waitForCardAbsent(timeout)) {
            return true;
        } else {
            return false;
        }

    } catch (PcscTerminalException &e) {
        logger->trace("[%s] waitForCardAbsent - caught PcscTerminalException " \
                      " (msg: %s, cause: %ds\n", terminal.getName(),
                      e.getMessage(), e.getCause().what());
    }

    return false;
}

std::vector<char> PcscReader::transmitApdu(std::vector<char> &apduIn)
{
    logger->debug("transmitApdu\n");

    std::vector<char> response;

    try {
        response = terminal.transmitApdu(apduIn);
    } catch (PcscTerminalException& e) {
        logger->error("transmitApdu - caught PcscTerminalException (msg: %s, " \
                      "cause: %s)\n", e.getMessage(), e.getCause().what());
        throw KeypleIOReaderException("transmitApdu failed", e);
    } catch (std::invalid_argument& e) {
        /* Card could have been removed prematurely */
        logger->error("transmitApdu - caught std::invalid_argument (cause : " \
                      "%s)\n", e.what());
        throw KeypleIOReaderException("transmitApdu failed", e);
    }

    return response;
}

bool PcscReader::protocolFlagMatches(const SeProtocol& protocolFlag)
{
    bool result;

    /* Test protocolFlag to check if ATR based protocol filtering is required */
    //if (protocolFlag != nullptr) {
        if (!isPhysicalChannelOpen()) {
            logger->debug("protocolFlagMatches - physical channel not open, " \
                          "opening it\n");
            openPhysicalChannel();
        }

        logger->debug("protocolFlagMatches - going through protocols map\n");
        std::unordered_map<SeProtocol, std::string>::iterator it =
            protocolsMap.begin();
        while (it != protocolsMap.end()) {
            SeProtocol p = it->first;
            logger->debug("available protocol: %s - %s\n", p.getName(),
                          it->second);
            it++;
        }

        /*
         * The requestSet will be executed only if the protocol match the
         * requestElement.
         */
        std::string selectionMask = protocolsMap[protocolFlag];
        logger->debug("protocolFlagMatches - selectionMask: %s\n",
                      selectionMask);
        if (selectionMask == "") {
            throw KeypleReaderException("Target selector mask not found!");// nullptr));
        }

        Pattern *p = Pattern::compile(selectionMask);
        std::string atr = ByteArrayUtil::toHex(this->terminal.getATR());
        if (!p->matcher(atr)->matches()) {
            logger->trace("[%s] protocolFlagMatches => unmatching SE. " \
                          "PROTOCOLFLAG = %s\n", this->getName(),
                          protocolFlag.toString());
            result = false;
        } else {
            logger->trace("[%s] protocolFlagMatches => matching SE. " \
                          "PROTOCOLFLAG = %s\n", this->getName(),
                          protocolFlag.toString());
            result = true;
        }
    //} else {
    //    // no protocol defined returns true
    //    result = true;
    //}

    return result;
}

void PcscReader::setParameter(const std::string &name, const std::string &value)
{
    logger->trace("[%s] setParameter => PCSC: Set a parameter. NAME = %s," \
                  " VALUE = %s\n", this->getName(), name, value);

    if (name == "") {
        throw std::invalid_argument("Parameter shouldn't be null\n");
    }

    if (name == SETTING_KEY_TRANSMISSION_MODE) {
        if (value == "")
            transmissionMode = static_cast<TransmissionMode>(0);
        else if (value == SETTING_TRANSMISSION_MODE_CONTACTS)
            transmissionMode = TransmissionMode::CONTACTS;
        else if (value == SETTING_TRANSMISSION_MODE_CONTACTLESS)
            transmissionMode = TransmissionMode::CONTACTLESS;
        else
            throw std::invalid_argument("Bad tranmission mode " + name + " : " +
                                        value);

    } else if (name == SETTING_KEY_PROTOCOL) {
        if (value == "" || value == SETTING_PROTOCOL_TX)
            parameterCardProtocol = "*";
        else if (value == SETTING_PROTOCOL_T0)
            parameterCardProtocol = "T=0";
        else if (value == SETTING_PROTOCOL_T1)
            parameterCardProtocol = "T=1";
        else if (value == SETTING_PROTOCOL_T_CL)
            parameterCardProtocol = "T=CL";
        else
           throw std::invalid_argument("Bad protocol " + name + " : " + value);

    } else if (name == SETTING_KEY_MODE) {
        if (value == "" || value == SETTING_MODE_SHARED) {
            if (cardExclusiveMode) {
                try {
                    terminal.endExclusive();
                } catch (PcscTerminalException &e) {
                    throw KeypleReaderException("Couldn't disable exclusive " \
                                                "mode", e);
                }
            }
            cardExclusiveMode = false;
        } else if (value == SETTING_MODE_EXCLUSIVE) {
            cardExclusiveMode = true;
        } else {
            throw std::invalid_argument("Parameter value not supported " +
                                        name + " : " + value);
        }
    } else if (name == SETTING_KEY_THREAD_TIMEOUT) {
        // TODO use setter
        if (value == "") {
            threadWaitTimeout = SETTING_THREAD_TIMEOUT_DEFAULT;
        } else {
            long long timeout = stoll(value);

            if (timeout <= 0) {
                throw std::invalid_argument("Timeout has to be of at least 1ms "
                                            + name + value);
            }

            threadWaitTimeout = timeout;
        }
    } else if (name == SETTING_KEY_DISCONNECT) {
        if (value == "" || value == SETTING_DISCONNECT_RESET) {
            cardReset = true;
        } else if (value == SETTING_DISCONNECT_UNPOWER) {
            cardReset = false;
        } else if (value == SETTING_DISCONNECT_EJECT ||
                   value == SETTING_DISCONNECT_LEAVE) {
            throw std::invalid_argument("This disconnection parameter is not " \
                                        "supported by this plugin" + name +
                                        " : " + value);
        } else {
            throw std::invalid_argument("Parameters not supported : " + name +
                                        " : " + value);
        }
    } else if (name == SETTING_KEY_LOGGING) {
        /*
         * Convert "true" or "false" to 1 or 0
         * default is null and perfectly acceptable
         */
        //std::istringstream(value) >> logging;
    } else {
        throw std::invalid_argument("This parameter is unknown !" + name +
                                    " : " + value);
    }
}

std::unordered_map<std::string, std::string> PcscReader::getParameters()
{
    std::unordered_map<std::string, std::string> parameters;

    { // Returning the protocol
        std::string protocol = parameterCardProtocol;
        if (protocol == "*") {
            protocol = SETTING_PROTOCOL_TX;
        }
        else if (protocol == "T=0") {
            protocol = SETTING_PROTOCOL_T0;
        }
        else if (protocol == "T=1") {
            protocol = SETTING_PROTOCOL_T1;
        }
        else {
            throw std::make_shared<IllegalStateException>("Illegal protocol: " +
                                                          protocol);
        }
        parameters.emplace(SETTING_KEY_PROTOCOL, protocol);
    }

    { // The mode ?
        if (!cardExclusiveMode) {
            parameters.emplace(SETTING_KEY_MODE, SETTING_MODE_SHARED);
        }
    }

    { // The thread wait timeout
        if (threadWaitTimeout != SETTING_THREAD_TIMEOUT_DEFAULT) {
            parameters.emplace(SETTING_KEY_THREAD_TIMEOUT,
                               std::to_string(threadWaitTimeout));
        }
    }


    return parameters;
}

std::vector<char> PcscReader::getATR()
{
    return terminal.getATR();
}

bool PcscReader::isPhysicalChannelOpen()
{
    return this->channelOpen;
}

void PcscReader::openPhysicalChannel()
{
    /*
     *  Init of the physical SE channel: if not yet established, opening of a
     * new physical channel
     */
    try {
        this->terminal.openAndConnect(parameterCardProtocol);
        if (cardExclusiveMode) {
            this->terminal.beginExclusive();
            logger->trace("[%s] Opening of a physical SE channel in " \
                          "exclusive mode\n", this->getName());
        } else {
            logger->trace("[%s] Opening of a physical SE channel in " \
                              "shared mode\n", this->getName());
        }
    } catch (PcscTerminalException &e) {
        throw KeypleChannelStateException(
                  "Error while opening Physical Channel", e);
    }

    this->channelOpen = true;
}

TransmissionMode PcscReader::getTransmissionMode()
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

bool PcscReader::equals(std::shared_ptr<void> o)
{
    (void)o;
    return false;
}

int PcscReader::hashCode()
{
    return 0;
}

void PcscReader::setParameters(
    std::unordered_map<std::string, std::string> &parameters)
{
    (void)parameters;
}

void PcscReader::notifyObservers(std::shared_ptr<ReaderEvent> event)
{
    (void)event;
}

}
}
}

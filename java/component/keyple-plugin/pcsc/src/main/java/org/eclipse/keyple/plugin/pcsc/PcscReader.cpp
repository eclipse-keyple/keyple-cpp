/* Core */
#include "ByteArrayUtils.h"
#include "KeypleBaseException.h"
#include "KeypleChannelStateException.h"
#include "KeypleIOReaderException.h"
#include "KeypleReaderException.h"
#include "NoStackTraceThrowable.h"
#include "Protocol_Import.h"
#include "PcscReader.h"
#include "SeProtocol.h"

/* Smartcard I/O */
#include "ATR.h"
#include "CardException.h"
#include "CommandAPDU.h"
#include "ResponseAPDU.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace plugin {
namespace pcsc {

using AbstractThreadedLocalReader = org::eclipse::keyple::seproxy::plugin::AbstractThreadedLocalReader;
using ATR                         = org::eclipse::keyple::smartcardio::ATR;
using CardException               = org::eclipse::keyple::smartcardio::CardException;
using CommandAPDU                 = org::eclipse::keyple::smartcardio::CommandAPDU;
using KeypleBaseException         = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
using KeypleIOReaderException     = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
using KeypleReaderException       = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
using NoStackTraceThrowable       = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
using Protocol                    = org::eclipse::keyple::seproxy::protocol::Protocol;
using ResponseAPDU                = org::eclipse::keyple::smartcardio::ResponseAPDU;
using SeProtocol                  = org::eclipse::keyple::seproxy::protocol::SeProtocol;
using TransmissionMode            = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
using ByteArrayUtils              = org::eclipse::keyple::util::ByteArrayUtils;

const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(PcscReader));

const std::string PcscReader::SETTING_KEY_TRANSMISSION_MODE = "transmission_mode";
const std::string PcscReader::SETTING_TRANSMISSION_MODE_CONTACTS = "contacts";
const std::string PcscReader::SETTING_TRANSMISSION_MODE_CONTACTLESS = "contactless";
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
const std::string PcscReader::SETTING_KEY_THREAD_TIMEOUT = "thread_wait_timeout";
const std::string PcscReader::SETTING_KEY_LOGGING = "logging";
const std::string PcscReader::PROTOCOL_T0 = "T=0";
const std::string PcscReader::PROTOCOL_T1 = "T=1";
const std::string PcscReader::PROTOCOL_T_CL = "T=CL";
const std::string PcscReader::PROTOCOL_ANY = "T=0";

PcscReader::PcscReader(const std::string &pluginName, std::shared_ptr<CardTerminal> terminal) : AbstractThreadedLocalReader(pluginName, terminal->getName()), terminal(terminal)
{
    logger->debug("PcscReader::PcscReader pluginName: %s, terminal: %p\n", pluginName, terminal);

    this->card = nullptr;
    this->channel = nullptr;
    this->protocolsMap = std::unordered_map<std::shared_ptr<SeProtocol>, std::string>();

    // Using null values to use the standard method for defining default values
    try {
        setParameter(SETTING_KEY_TRANSMISSION_MODE, "");
        setParameter(SETTING_KEY_PROTOCOL, "");
        setParameter(SETTING_KEY_MODE, "");
        setParameter(SETTING_KEY_DISCONNECT, "");
        setParameter(SETTING_KEY_LOGGING, "");
    } catch (KeypleBaseException &e) {
        logger->trace("[%s] PcscReader => KeypleBaseException\n");
    }
}

void PcscReader::closePhysicalChannel()
{
    try {
        if (card != nullptr) {
            if (logging) {
                logger->trace("[%s] closePhysicalChannel => closing the channel\n", this->getName());
            }
            channel.reset();
            card->disconnect(cardReset);
            card.reset();
        }
        else {
            if (logging) {
                logger->trace("[%s] closePhysicalChannel => card object is null\n", this->getName());
            }
        }
    }
    catch (const CardException &e) {
        throw KeypleChannelStateException("Error while closing physical channel"); //, e);
    }
}

bool PcscReader::checkSePresence()
{
    try {
	logger->debug("checkSePresence - calling isCardPresent\n");
        return terminal->isCardPresent();
    }
    catch (const CardException &e) {
        logger->trace("[%s] Exception occured in isSePresent. Message: %s\n", this->getName(), e.getMessage());
        throw NoStackTraceThrowable();
    }

    return false;
}

bool PcscReader::waitForCardPresent(long long timeout)
{
    logger->debug("waitForCardPresent - timeout: %d\n", timeout);

    try {
        return terminal->waitForCardPresent(timeout);
    }
    catch (const CardException &e) {
        logger->trace("[%s] Exception occured in waitForCardPresent. Message: %s\n", this->getName(), e.getMessage());
        throw NoStackTraceThrowable();
    }

    return false;
}

bool PcscReader::waitForCardAbsent(long long timeout)
{
    try {
        if (terminal->waitForCardAbsent(timeout)) {
            closeLogicalChannel();
            closePhysicalChannel();
            return true;
        } else {
            return false;
        }
    } catch (CardException &e) {
        logger->trace("[%s] Exception occured in waitForCardAbsent. Message: %s\n",
                      this->getName(), e.getMessage());
        throw NoStackTraceThrowable();
    }

    return false;
}

std::vector<char> PcscReader::transmitApdu(std::vector<char> &apduIn)
{
    logger->debug("transmitApdu\n");

    std::shared_ptr<ResponseAPDU> apduResponseData;
    try {
        apduResponseData = channel->transmit(std::shared_ptr<CommandAPDU>(new CommandAPDU(apduIn)));
    }
    catch (CardException &e) {
        throw KeypleIOReaderException(this->getName() + ":" + e.getMessage());
    }

    return apduResponseData->getBytes();
}

bool PcscReader::protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag)
{
    bool result;

    /* Get protocolFlag to check if ATR filtering is required */
    Protocol* protocol = reinterpret_cast<Protocol*>(protocolFlag.get());
    if (*protocol != Protocol::ANY) {
        if (!isPhysicalChannelOpen()) {
            openPhysicalChannel();
        }

        /*
         * The requestSet will be executed only if the protocol match the
         * requestElement.
         */
        std::string selectionMask = protocolsMap[protocolFlag];
        if (selectionMask == "") {
            throw KeypleReaderException("Target selector mask not found!");// nullptr));
        }

        Pattern *p = Pattern::compile(selectionMask);
        std::string atr = ByteArrayUtils::toHex(card->getATR()->getBytes());
        if (!p->matcher(atr)->matches()) {
            logger->trace("[%s] protocolFlagMatches => unmatching SE. PROTOCOLFLAG = %s\n",
                          this->getName(), protocolFlag);
            result = false;
        } else {
            logger->trace("[%s] protocolFlagMatches => matching SE. PROTOCOLFLAG = %s\n",
                          this->getName(), protocolFlag);
            result = true;
        }
    } else {
        // no protocol defined returns true
        result = true;
    }
return result;
}

void PcscReader::setParameter(const std::string &name, const std::string &value)
{
    logger->debug("PcscReader::setParameter name: %s, value: %s\n", name, value);

    if (logging) {
        logger->trace("[%s] setParameter => PCSC: Set a parameter. NAME = %s, VALUE = %s\n", this->getName(), name, value);
    }

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
            throw std::invalid_argument("Bad tranmission mode " + name + " : " + value);

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
            if (cardExclusiveMode && card != nullptr) {
                try {
                    card->endExclusive();
                }
                catch (CardException &e) {
                    throw KeypleReaderException("Couldn't disable exclusive mode"); //, e));
                }
            }
            cardExclusiveMode = false;
        }
        else if (value == SETTING_MODE_EXCLUSIVE) {
            cardExclusiveMode = true;
        }
        else {
            throw std::invalid_argument("Parameter value not supported " + name + " : " + value);
        }
    }
    else if (name == SETTING_KEY_THREAD_TIMEOUT) {
        // TODO use setter
        if (value == "") {
            threadWaitTimeout = SETTING_THREAD_TIMEOUT_DEFAULT;
        }
        else {
            long long timeout = stoll(value);

            if (timeout <= 0) {
                throw std::invalid_argument("Timeout has to be of at least 1ms " + name + value);
            }

            threadWaitTimeout = timeout;
        }
    }
    else if (name == SETTING_KEY_DISCONNECT) {
        if (value == "" || value == SETTING_DISCONNECT_RESET) {
            cardReset = true;
        }
        else if (value == SETTING_DISCONNECT_UNPOWER) {
            cardReset = false;
        }
        else if (value == SETTING_DISCONNECT_EJECT || value == SETTING_DISCONNECT_LEAVE) {
            throw std::invalid_argument("This disconnection parameter is not supported by this plugin" + name + " : " + value);
        }
        else {
            throw std::invalid_argument("Parameters not supported : " + name + " : " + value);
        }
    }
    else if (name == SETTING_KEY_LOGGING) {
        /* Convert "true" or "false" to 1 or 0 */
        std::istringstream(value) >> logging; // default is null and perfectly acceptable
    }
    else {
        throw std::invalid_argument("This parameter is unknown !" + name + " : " + value);
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
            throw std::make_shared<IllegalStateException>("Illegal protocol: " + protocol);
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
    //JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            parameters.emplace(SETTING_KEY_THREAD_TIMEOUT, std::to_string(threadWaitTimeout));
        }
    }


    return parameters;
}

std::vector<char> PcscReader::getATR()
{
    logger->debug("getATR -\n");

    ATR *atr = card->getATR();
    if (!atr) {
        logger->debug("getATR - atr is null\n");
        return std::vector<char>();
    }

    return card->getATR()->getBytes();
}

bool PcscReader::isPhysicalChannelOpen()
{
    return card != nullptr;
}

void PcscReader::openPhysicalChannel()
{
    // init of the physical SE channel: if not yet established, opening of a new physical
    // channel
    try {
        if (card == nullptr) {
            this->card = std::shared_ptr<Card>(this->terminal->connect(parameterCardProtocol));
            if (cardExclusiveMode) {
                card->beginExclusive();
                if (logging) {
                    logger->trace("[%s] Opening of a physical SE channel in exclusive mode\n", this->getName());
                }
            }
            else {
                if (logging) {
                    logger->trace("[%s] Opening of a physical SE channel in shared mode\n", this->getName());
                }
            }
        }
        this->channel = std::shared_ptr<CardChannel>(card->getBasicChannel());
    }
    catch (const CardException &e) {
        throw KeypleChannelStateException("Error while opening Physical Channel\n"); //, e));
    }
}

TransmissionMode PcscReader::getTransmissionMode()
{
    if (transmissionMode != static_cast<TransmissionMode>(0)) {
        return transmissionMode;
    } else {
        if (!parameterCardProtocol.compare(PROTOCOL_T1) || !parameterCardProtocol.compare(PROTOCOL_T_CL)) {
            return TransmissionMode::CONTACTLESS;
        }
        else {
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

void PcscReader::setParameters(std::unordered_map<std::string, std::string> &parameters)
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
}
}

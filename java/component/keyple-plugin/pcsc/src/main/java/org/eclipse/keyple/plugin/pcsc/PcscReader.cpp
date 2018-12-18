#include "PcscReader.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocol.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/Protocol.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace pcsc {
                    using namespace org::eclipse::keyple::seproxy::exception;
                    using AbstractThreadedLocalReader = org::eclipse::keyple::seproxy::plugin::AbstractThreadedLocalReader;
                    using Protocol = org::eclipse::keyple::seproxy::protocol::Protocol;
                    using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> PcscReader::logger = org::slf4j::LoggerFactory::getLogger(PcscReader::typeid);
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

                    PcscReader::PcscReader(const std::string &pluginName, std::shared_ptr<CardTerminal> terminal) : org::eclipse::keyple::seproxy::plugin::AbstractThreadedLocalReader(pluginName, terminal->getName()), terminal(terminal) {
                        this->card.reset();
                        this->channel.reset();
                        this->protocolsMap = std::unordered_map<std::shared_ptr<SeProtocol>, std::string>();

                        // Using null values to use the standard method for defining default values
                        try {
                            setParameter(SETTING_KEY_TRANSMISSION_MODE, "");
                            setParameter(SETTING_KEY_PROTOCOL, "");
                            setParameter(SETTING_KEY_MODE, "");
                            setParameter(SETTING_KEY_DISCONNECT, "");
                            setParameter(SETTING_KEY_LOGGING, "");
                        }
                        catch (const KeypleBaseException &ex) {
                            // can not fail with null value
                        }
                    }

                    void PcscReader::closePhysicalChannel() throw(KeypleChannelStateException) {
                        try {
                            if (card != nullptr) {
                                if (logging) {
                                    logger->trace("[{}] closePhysicalChannel => closing the channel.", this->getName());
                                }
                                channel.reset();
                                card->disconnect(cardReset);
                                card.reset();
                            }
                            else {
                                if (logging) {
                                    logger->trace("[{}] closePhysicalChannel => card object is null.", this->getName());
                                }
                            }
                        }
                        catch (const CardException &e) {
                            throw std::make_shared<KeypleChannelStateException>("Error while closing physical channel", e);
                        }
                    }

                    bool PcscReader::isSePresent() throw(NoStackTraceThrowable) {
                        try {
                            return terminal->isCardPresent();
                        }
                        catch (const CardException &e) {
                            logger->trace("[{}] Exception occured in isSePresent. Message: {}", this->getName(), e->getMessage());
                            throw std::make_shared<NoStackTraceThrowable>();
                        }
                    }

                    bool PcscReader::waitForCardPresent(long long timeout) throw(NoStackTraceThrowable) {
                        try {
                            return terminal->waitForCardPresent(timeout);
                        }
                        catch (const CardException &e) {
                            logger->trace("[{}] Exception occured in waitForCardPresent. Message: {}", this->getName(), e->getMessage());
                            throw std::make_shared<NoStackTraceThrowable>();
                        }
                    }

                    bool PcscReader::waitForCardAbsent(long long timeout) throw(NoStackTraceThrowable) {
                        try {
                            if (terminal->waitForCardAbsent(timeout)) {
                                closeLogicalChannel();
                                closePhysicalChannel();
                                return true;
                            }
                            else {
                                return false;
                            }
                        }
                        catch (const KeypleChannelStateException &e) {
                            logger->trace("[{}] Exception occured in waitForCardAbsent. Message: {}", this->getName(), e->what());
                            throw std::make_shared<NoStackTraceThrowable>();
                        }
                        catch (const CardException &e) {
                            logger->trace("[{}] Exception occured in waitForCardAbsent. Message: {}", this->getName(), e->getMessage());
                            throw std::make_shared<NoStackTraceThrowable>();
                        }
                    }

                    std::vector<char> PcscReader::transmitApdu(std::vector<char> &apduIn) throw(KeypleIOReaderException) {
                        std::shared_ptr<ResponseAPDU> apduResponseData;
                        try {
                            apduResponseData = channel->transmit(std::make_shared<CommandAPDU>(apduIn));
                        }
                        catch (const CardException &e) {
                            throw std::make_shared<KeypleIOReaderException>(this->getName() + ":" + e->getMessage());
                        }
                        return apduResponseData->getBytes();
                    }

                    bool PcscReader::protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) throw(KeypleReaderException) {
                        bool result;
                        // Get protocolFlag to check if ATR filtering is required
                        if (protocolFlag != Protocol::ANY) {
                            if (!isPhysicalChannelOpen()) {
                                openPhysicalChannel();
                            }
                            // the requestSet will be executed only if the protocol match the requestElement
                            std::string selectionMask = protocolsMap[protocolFlag];
                            if (selectionMask == "") {
                                throw std::make_shared<KeypleReaderException>("Target selector mask not found!", nullptr);
                            }
                            std::shared_ptr<Pattern> p = Pattern::compile(selectionMask);
                            std::string atr = ByteArrayUtils::toHex(card->getATR().getBytes());
                            if (!p->matcher(atr).matches()) {
                                if (logging) {
                                    logger->trace("[{}] protocolFlagMatches => unmatching SE. PROTOCOLFLAG = {}", this->getName(), protocolFlag);
                                }
                                result = false;
                            }
                            else {
                                if (logging) {
                                    logger->trace("[{}] protocolFlagMatches => matching SE. PROTOCOLFLAG = {}", this->getName(), protocolFlag);
                                }
                                result = true;
                            }
                        }
                        else {
                            // no protocol defined returns true
                            result = true;
                        }
                        return result;
                    }

                    void PcscReader::setParameter(const std::string &name, const std::string &value) throw(std::invalid_argument, KeypleBaseException) {
                        if (logging) {
                            logger->trace("[{}] setParameter => PCSC: Set a parameter. NAME = {}, VALUE = {}", this->getName(), name, value);
                        }
                        if (name == "") {
                            throw std::invalid_argument("Parameter shouldn't be null");
                        }
                        if (name == SETTING_KEY_TRANSMISSION_MODE) {
                            if (value == "") {
                                transmissionMode = nullptr;
                            }
                            else if (value == SETTING_TRANSMISSION_MODE_CONTACTS) {
                                transmissionMode = TransmissionMode::CONTACTS;
                            }
                            else if (value == SETTING_TRANSMISSION_MODE_CONTACTLESS) {
                                transmissionMode = TransmissionMode::CONTACTLESS;
                            }
                            else {
                                throw std::invalid_argument("Bad tranmission mode " + name + " : " + value);
                            }
                        }
                        else if (name == SETTING_KEY_PROTOCOL) {
                            if (value == "" || value == SETTING_PROTOCOL_TX) {
                                parameterCardProtocol = "*";
                            }
                            else if (value == SETTING_PROTOCOL_T0) {
                                parameterCardProtocol = "T=0";
                            }
                            else if (value == SETTING_PROTOCOL_T1) {
                                parameterCardProtocol = "T=1";
                            }
                            else if (value == SETTING_PROTOCOL_T_CL) {
                                parameterCardProtocol = "T=CL";
                            }
                            else {
                                throw std::invalid_argument("Bad protocol " + name + " : " + value);
                            }
                        }
                        else if (name == SETTING_KEY_MODE) {
                            if (value == "" || value == SETTING_MODE_SHARED) {
                                if (cardExclusiveMode && card != nullptr) {
                                    try {
                                        card->endExclusive();
                                    }
                                    catch (const CardException &e) {
                                        throw std::make_shared<KeypleReaderException>("Couldn't disable exclusive mode", e);
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
                                long long timeout = StringHelper::fromString<long long>(value);

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
                            logging = StringHelper::fromString<bool>(value); // default is null and perfectly acceptable
                        }
                        else {
                            throw std::invalid_argument("This parameter is unknown !" + name + " : " + value);
                        }
                    }

                    std::unordered_map<std::string, std::string> PcscReader::getParameters() {
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
                                parameters.emplace(SETTING_KEY_THREAD_TIMEOUT, Long::toString(threadWaitTimeout));
                            }
                        }


                        return parameters;
                    }

                    std::vector<char> PcscReader::getATR() {
                        return card->getATR().getBytes();
                    }

                    bool PcscReader::isPhysicalChannelOpen() {
                        return card != nullptr;
                    }

                    void PcscReader::openPhysicalChannel() throw(KeypleChannelStateException) {
                        // init of the physical SE channel: if not yet established, opening of a new physical
                        // channel
                        try {
                            if (card == nullptr) {
                                if (isLogicalChannelOpen()) {
                                    throw std::make_shared<KeypleChannelStateException>("Logical channel found open while physical channel is not!");
                                }
                                this->card = this->terminal->connect(parameterCardProtocol);
                                if (cardExclusiveMode) {
                                    card->beginExclusive();
                                    if (logging) {
                                        logger->trace("[{}] Opening of a physical SE channel in exclusive mode.", this->getName());
                                    }
                                }
                                else {
                                    if (logging) {
                                        logger->trace("[{}] Opening of a physical SE channel in shared mode.", this->getName());
                                    }
                                }
                            }
                            this->channel = card->getBasicChannel();
                        }
                        catch (const CardException &e) {
                            throw std::make_shared<KeypleChannelStateException>("Error while opening Physical Channel", e);
                        }
                    }

                    TransmissionMode PcscReader::getTransmissionMode() {
                        if (transmissionMode != nullptr) {
                            return transmissionMode;
                        }
                        else {
                            if (parameterCardProtocol.contentEquals(PROTOCOL_T1) || parameterCardProtocol.contentEquals(PROTOCOL_T_CL)) {
                                return TransmissionMode::CONTACTLESS;
                            }
                            else {
                                return TransmissionMode::CONTACTS;
                            }
                        }
                    }
                }
            }
        }
    }
}

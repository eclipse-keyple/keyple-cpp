#include "StubReader.h"
#include "StubSecureElement.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleChannelStateException.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleIOReaderException.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocol.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/Protocol.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequestSet.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponseSet.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace stub {
                    using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                    using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                    using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
                    using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                    using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;
                    using AbstractThreadedLocalReader = org::eclipse::keyple::seproxy::plugin::AbstractThreadedLocalReader;
                    using Protocol = org::eclipse::keyple::seproxy::protocol::Protocol;
                    using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> StubReader::logger = org::slf4j::LoggerFactory::getLogger(StubReader::typeid);
const std::string StubReader::ALLOWED_PARAMETER_1 = "parameter1";
const std::string StubReader::ALLOWED_PARAMETER_2 = "parameter2";
const std::string StubReader::CONTACTLESS_PARAMETER = "contactless";
const std::string StubReader::CONTACTS_PARAMETER = "contacts";
const std::string StubReader::pluginName = "StubPlugin";

                    StubReader::StubReader(const std::string &name) : org::eclipse::keyple::seproxy::plugin::AbstractThreadedLocalReader(pluginName, name) {
                        readerName = name;
                        sePresent = false;
                        threadWaitTimeout = 5000;
                    }

                    std::vector<char> StubReader::getATR() {
                        return se->getATR();
                    }

                    bool StubReader::isPhysicalChannelOpen() {
                        return se != nullptr && se->isPhysicalChannelOpen();
                    }

                    void StubReader::openPhysicalChannel() throw(KeypleChannelStateException) {
                        if (se != nullptr) {
                            se->openPhysicalChannel();
                        }
                    }

                    void StubReader::closePhysicalChannel() throw(KeypleChannelStateException) {
                        if (se != nullptr) {
                            se->closePhysicalChannel();
                        }
                    }

                    std::vector<char> StubReader::transmitApdu(std::vector<char> &apduIn) throw(KeypleIOReaderException) {
                        return se->processApdu(apduIn);
                    }

                    bool StubReader::protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) throw(KeypleReaderException) {
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
                            std::string protocol = se->getSeProcotol();
                            if (!p->matcher(protocol).matches()) {
                                logger->trace("[{}] protocolFlagMatches => unmatching SE. PROTOCOLFLAG = {}", this->getName(), protocolFlag);
                                result = false;
                            }
                            else {
                                logger->trace("[{}] protocolFlagMatches => matching SE. PROTOCOLFLAG = {}", this->getName(), protocolFlag);
                                result = true;
                            }
                        }
                        else {
                            // no protocol defined returns true
                            result = true;
                        }
                        return result;
                    }

                    bool StubReader::isSePresent() {
                        return se != nullptr;
                    }

                    void StubReader::setParameter(const std::string &name, const std::string &value) throw(KeypleReaderException) {
                        if (name == ALLOWED_PARAMETER_1 || name == ALLOWED_PARAMETER_2) {
                            parameters.emplace(name, value);
                        }
                        else if (name == CONTACTS_PARAMETER) {
                            transmissionMode = TransmissionMode::CONTACTS;
                        }
                        else if (name == CONTACTLESS_PARAMETER) {
                            transmissionMode = TransmissionMode::CONTACTLESS;
                        }
                        else {
                            throw std::make_shared<KeypleReaderException>("parameter name not supported : " + name);
                        }
                    }

                    std::unordered_map<std::string, std::string> StubReader::getParameters() {
                        return parameters;
                    }

                    TransmissionMode StubReader::getTransmissionMode() {
                        return transmissionMode;
                    }

                    std::shared_ptr<ApduResponse> StubReader::processApduRequestTestProxy(std::shared_ptr<ApduRequest> apduRequest) throw(KeypleReaderException) {
                        return this->processApduRequest(apduRequest);
                    }

                    std::shared_ptr<SeResponseSet> StubReader::processSeRequestSetTestProxy(std::shared_ptr<SeRequestSet> requestSet) throw(KeypleReaderException) {
                        return this->processSeRequestSet(requestSet);
                    }

                    bool StubReader::isLogicalChannelOpenTestProxy() {
                        return this->isPhysicalChannelOpen();
                    }

                    void StubReader::insertSe(std::shared_ptr<StubSecureElement> _se) {
                        /* clean channels status */
                        if (isPhysicalChannelOpen()) {
                            closeLogicalChannel();
                            try {
                                closePhysicalChannel();
                            }
                            catch (const KeypleReaderException &e) {
                                e->printStackTrace();
                            }
                        }
                        se = _se;
                        sePresent = true;
                    }

                    void StubReader::removeSe() {
                        se.reset();
                        sePresent = false;
                    }

                    bool StubReader::waitForCardPresent(long long timeout) throw(NoStackTraceThrowable) {
                        for (int i = 0; i < timeout / 10; i++) {
                            if (sePresent) {
                                break;
                            }
                            try {
                                delay(10);
                            }
                            catch (const InterruptedException &e) {
                                e->printStackTrace();
                            }
                        }
                        return sePresent;
                    }

                    bool StubReader::waitForCardAbsent(long long timeout) throw(NoStackTraceThrowable) {
                        for (int i = 0; i < timeout / 10; i++) {
                            if (!sePresent) {
                                break;
                            }
                            try {
                                delay(10);
                            }
                            catch (const InterruptedException &e) {
                                e->printStackTrace();
                            }
                        }
                        return !sePresent;
                    }
                }
            }
        }
    }
}

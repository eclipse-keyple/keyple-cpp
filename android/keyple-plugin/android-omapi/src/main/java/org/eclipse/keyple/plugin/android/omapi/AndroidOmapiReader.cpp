#include "AndroidOmapiReader.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleApplicationSelectionException.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleChannelStateException.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequest.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleIOReaderException.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocol.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactsProtocols.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace android {
                    namespace omapi {
                        using KeypleApplicationSelectionException = org::eclipse::keyple::seproxy::exception::KeypleApplicationSelectionException;
                        using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                        using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using namespace org::eclipse::keyple::seproxy::message;
                        using AbstractStaticReader = org::eclipse::keyple::seproxy::plugin::AbstractStaticReader;
                        using ContactsProtocols = org::eclipse::keyple::seproxy::protocol::ContactsProtocols;
                        using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                        using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::simalliance::openmobileapi::Channel;
                        using org::simalliance::openmobileapi::Reader;
                        using org::simalliance::openmobileapi::Session;
                        using android::util::Log;
const std::string AndroidOmapiReader::TAG = AndroidOmapiReader::typeid->getSimpleName();

                        AndroidOmapiReader::AndroidOmapiReader(const std::string &pluginName, std::shared_ptr<Reader> omapiReader, const std::string &readerName) : org::eclipse::keyple::seproxy::plugin::AbstractStaticReader(pluginName, readerName) {
                            this->omapiReader = omapiReader;
                        }

                        std::unordered_map<std::string, std::string> AndroidOmapiReader::getParameters() {
                            Log::w(TAG, "No parameters are supported by AndroidOmapiReader");
                            return parameters;
                        }

                        void AndroidOmapiReader::setParameter(const std::string &key, const std::string &value) {
                            Log::w(TAG, "No parameters are supported by AndroidOmapiReader");
                            parameters.emplace(key, value);
                        }

                        TransmissionMode AndroidOmapiReader::getTransmissionMode() {
                            return TransmissionMode::CONTACTLESS;
                        }

                        bool AndroidOmapiReader::isSePresent() throw(NoStackTraceThrowable) {
                            return omapiReader->isSecureElementPresent();
                        }

                        std::shared_ptr<SelectionStatus> AndroidOmapiReader::openLogicalChannelAndSelect(std::shared_ptr<SeRequest::Selector> selector, std::shared_ptr<Set<Integer>> successfulSelectionStatusCodes) throw(KeypleChannelStateException, KeypleApplicationSelectionException) {
                            std::vector<std::vector<char>> atrAndFci(2);
                            std::vector<char> aid = (std::static_pointer_cast<SeRequest::AidSelector>(selector))->getAidToSelect();
                            try {
                                if (openChannel != nullptr && !openChannel->isClosed() && openApplication.size() > 0 && openApplication.equals(aid)) {
                                    Log::i(TAG, "Channel is already open to aid : " + ByteArrayUtils::toHex(aid));

                                    atrAndFci[0] = openChannel->getSession().getATR();
                                    atrAndFci[1] = openChannel->getSelectResponse();


                                }
                                else {

                                    Log::i(TAG, "Opening channel to aid : " + ByteArrayUtils::toHex(aid));

                                    // open physical channel
                                    std::shared_ptr<Session> session = omapiReader->openSession();

                                    // get ATR from session
                                    Log::i(TAG, "Retrieveing ATR from session...");
                                    atrAndFci[0] = session->getATR();

                                    Log::i(TAG, "Create logical openChannel within the session...");
                                    openChannel = session->openLogicalChannel(aid);

                                    // get FCI
                                    atrAndFci[1] = openChannel->getSelectResponse();

                                }
                            }
                            catch (const IOException &e) {
                                throw std::make_shared<KeypleChannelStateException>("Error while opening channel, aid :" + ByteArrayUtils::toHex(aid), e->getCause());
                            }
                            catch (const SecurityException &e) {
                                throw std::make_shared<KeypleChannelStateException>("Error while opening channel, aid :" + ByteArrayUtils::toHex(aid), e->getCause());
                            }
                            catch (const NoSuchElementException &e) {
                                throw std::make_shared<KeypleApplicationSelectionException>("Error while selecting application : " + ByteArrayUtils::toHex(aid), e);
                            }

                            return std::make_shared<SelectionStatus>(std::make_shared<AnswerToReset>(atrAndFci[0]), std::make_shared<ApduResponse>(atrAndFci[1], nullptr), true);
                        }

                        void AndroidOmapiReader::closePhysicalChannel() {
                            // close physical channel if exists
                            if (openApplication.size() > 0) {
                                openChannel->getSession()->close();
                                openChannel.reset();
                                openApplication.clear();
                            }
                        }

                        std::vector<char> AndroidOmapiReader::transmitApdu(std::vector<char> &apduIn) throw(KeypleIOReaderException) {
                            // Initialization
                            Log::d(TAG, "Data Length to be sent to tag : " + std::to_string(apduIn.size()));
                            Log::d(TAG, "Data in : " + ByteArrayUtils::toHex(apduIn));
                            std::vector<char> data = apduIn;
                            std::vector<char> dataOut(0);
                            try {
                                dataOut = openChannel->transmit(data);
                            }
                            catch (const IOException &e) {
                                e->printStackTrace();
                                throw std::make_shared<KeypleIOReaderException>("Error while transmitting APDU", e);
                            }
                            std::vector<char> out = dataOut;
                            Log::d(TAG, "Data out : " + ByteArrayUtils::toHex(out));
                            return out;
                        }

                        bool AndroidOmapiReader::protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) {
                            return protocolFlag->equals(ContactsProtocols::PROTOCOL_ISO7816_3);
                        }
                    }
                }
            }
        }
    }
}
